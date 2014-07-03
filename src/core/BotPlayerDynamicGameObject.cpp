//
//  BotPlayerDynamicGameObject.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "BotPlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "Explosion.h"
#include "GameListener.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "MapSearchNode.h"
#include "PathFinder.h"

// For Random Event Generation
#include <stdlib.h>
#include <time.h>

#define DEBUG_LISTS 0
#define DEBUG_LIST_LENGTHS_ONLY 0

BotPlayerDynamicGameObject::BotPlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction) : PlayerDynamicGameObject(playerIndex, gridX, gridY, gameListener, direction)
{
    srand((int)time(NULL));
    
    m_currentPathIndex = 0;
    m_currentPathType = 0;
    m_fActionTime = 0;
    m_fWaitTime = 0;
    
    m_playerTarget = nullptr;
}

void BotPlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    PlayerDynamicGameObject::update(deltaTime, mapBorders, insideBlocks, breakableBlocks, powerUps, explosions, players, bombs);

    if (m_playerState == ALIVE && m_playerActionState != WINNING)
    {
        if(m_fWaitTime > 0 && m_fActionTime < m_fWaitTime)
        {
            m_fActionTime += deltaTime;
        }
        else
        {
            if(m_currentPathType != 1)
            {
                determinePlayerTarget(players);
                
                if(calculatePathToTarget(m_playerTarget->getGridX(), m_playerTarget->getGridY()))
                {
                    // Great, we have a path to the player, kick ass
                    m_currentPathType = 0;
                }
                else if(m_playerTarget != nullptr)
                {
                    Node currentNode = Node {m_gridX, m_gridY};
                    if(PathFinder::getInstance().calculateClosestNodeToPlayerTarget(m_playerTarget, currentNode))
                    {
                        if(calculatePathToTarget(currentNode.x, currentNode.y))
                        {
                            m_currentPathType = 2;
                        }
                    }
                }
                else
                {
                    // Let's randomly traverse the map
                }
            }
            
            for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
            {
                if(PathFinder::getInstance().isLocationOccupiedByBombOrExplosionPath(bombs, explosions, m_gridX, m_gridY))
                {
                    Node currentNode = Node {m_gridX, m_gridY};
                    if(PathFinder::calculateClosestSafeNodeFromStartingNode(bombs, explosions, currentNode))
                    {
                        if(calculatePathToTarget(currentNode.x, currentNode.y))
                        {
                            m_currentPathType = 1;
                        }
                        
                        m_fWaitTime = 0;
                        m_fActionTime = 0;
                    }
                    
                    break;
                }
            }
            
            if(m_currentPath.size() > 0)
            {
                if(m_currentPathIndex == m_currentPath.size())
                {
                    m_playerTarget = nullptr;
                    m_currentPath.clear();
                    m_currentPathIndex = 0;
                    m_currentPathType = 0;
                    
                    if(m_currentPathType == 0)
                    {
                        // We were pursuing a player and caught up with them...
                        // This shouldn't happen since the bot will drop bombs ahead
                        // of time and then reroute itself to dodge the bomb
                    }
                    else if(m_currentPathType == 1)
                    {
                        m_fActionTime = 0;
                        m_fWaitTime = 2;
                        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_MOVE_STOP);
                    }
                    else if(m_currentPathType == 2)
                    {
                        if(isAbleToDropAdditionalBomb(players, bombs))
                        {
                            m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
                        }
                    }
                }
                else
                {
                    if(m_gridX == m_currentPath.at(m_currentPathIndex)->x && m_gridY == m_currentPath.at(m_currentPathIndex)->y)
                    {
                        cout << "Node position reached : (" << m_gridX << ", " << m_gridY << ")" << endl;
                        m_currentPathIndex++;
                    }
                    else
                    {
                        if(m_gridX < m_currentPath.at(m_currentPathIndex)->x && m_gridY == m_currentPath.at(m_currentPathIndex)->y)
                        {
                            moveInDirection(DIRECTION_RIGHT);
                        }
                        else if(m_gridX == m_currentPath.at(m_currentPathIndex)->x && m_gridY < m_currentPath.at(m_currentPathIndex)->y)
                        {
                            moveInDirection(DIRECTION_UP);
                        }
                        else if(m_gridX > m_currentPath.at(m_currentPathIndex)->x && m_gridY == m_currentPath.at(m_currentPathIndex)->y)
                        {
                            moveInDirection(DIRECTION_LEFT);
                        }
                        else if(m_gridX == m_currentPath.at(m_currentPathIndex)->x && m_gridY > m_currentPath.at(m_currentPathIndex)->y)
                        {
                            moveInDirection(DIRECTION_DOWN);
                        }
                    }
                }
            }
            
            if(m_currentPathType != 1 && PathFinder::shouldPlayerPlantBomb(breakableBlocks, players, this))
            {
                if(isAbleToDropAdditionalBomb(players, bombs))
                {
                    m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
                }
            }
        }
    }
    
    if (isHitByExplosion(explosions, bombs))
    {
        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_DEATH);
    }
}

bool BotPlayerDynamicGameObject::isBot()
{
    return true;
}

void BotPlayerDynamicGameObject::determinePlayerTarget(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    m_playerTarget = nullptr;
    
    float shortestPlayerTargetDistance;
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((*itr).get() != this && (*itr)->getPlayerState() == ALIVE)
        {
            float playerTargetDistance = m_position->dist((*itr)->getPosition());
            
            if(m_playerTarget == nullptr)
            {
                m_playerTarget = (*itr).get();
                shortestPlayerTargetDistance = playerTargetDistance;
            }
            else
            {
                if(playerTargetDistance < shortestPlayerTargetDistance)
                {
                    m_playerTarget = (*itr).get();
                    shortestPlayerTargetDistance = playerTargetDistance;
                }
            }
        }
    }
}

bool BotPlayerDynamicGameObject::calculatePathToTarget(int x, int y)
{
    m_currentPath.clear();
    m_currentPathIndex = 0;
    
    // Create a start state
    MapSearchNode nodeStart;
    nodeStart.x = m_gridX;
    nodeStart.y = m_gridY;
    
    // Define the goal state
    MapSearchNode nodeEnd;
    nodeEnd.x = x;
    nodeEnd.y = y;
    
    // Set Start and goal states
    
    AStarSearch<MapSearchNode> astarsearch;
    astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );
    
    unsigned int SearchState;
    unsigned int SearchSteps = 0;
    
    do
    {
        SearchState = astarsearch.SearchStep();
        
        SearchSteps++;
        
#if DEBUG_LISTS
        
        cout << "Steps:" << SearchSteps << "\n";
        
        int len = 0;
        
        cout << "Open:\n";
        MapSearchNode *p = astarsearch.GetOpenListStart();
        while(p)
        {
            len++;
#if !DEBUG_LIST_LENGTHS_ONLY
            ((MapSearchNode *)p)->PrintNodeInfo();
#endif
            p = astarsearch.GetOpenListNext();
        }
        
        cout << "Open list has " << len << " nodes\n";
        
        len = 0;
        
        cout << "Closed:\n";
        p = astarsearch.GetClosedListStart();
        while(p)
        {
            len++;
#if !DEBUG_LIST_LENGTHS_ONLY
            p->PrintNodeInfo();
#endif
            p = astarsearch.GetClosedListNext();
        }
        
        cout << "Closed list has " << len << " nodes\n";
#endif
    }
    while(SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);
    
    if(SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
    {
        cout << "Search found goal state\n";
        
        MapSearchNode *node = astarsearch.GetSolutionStart();
        
#if DISPLAY_SOLUTION
        cout << "Displaying solution\n";
#endif
        int steps = 0;
        
        node->PrintNodeInfo();
        for(;;)
        {
            node = astarsearch.GetSolutionNext();
            
            if(!node)
            {
                break;
            }
            
            node->PrintNodeInfo();
            m_currentPath.push_back(std::unique_ptr<MapSearchNode>(new MapSearchNode(node->x, node->y)));
            steps++;
        };
        
        cout << "Solution steps " << steps << endl;
        
        // Once you're done with the solution you can free the nodes up
        astarsearch.FreeSolutionNodes();
    }
    else if(SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED)
    {
        cout << "Search terminated. Did not find goal state\n";
    }
    
    // Display the number of loops the search went through
    cout << "SearchSteps : " << SearchSteps << "\n";
    
    astarsearch.EnsureMemoryFreed();
    
    return SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED;
}

void BotPlayerDynamicGameObject::moveInDirection(int direction)
{
    if(getDirection() != direction || !isMoving())
    {
        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + direction + 1);
    }
}
