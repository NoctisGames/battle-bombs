//
//  BotPlayerDynamicGameObject.cpp
//  battlebombs
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
#include "OverlapTester.h"

// For Randomness
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

BotPlayerDynamicGameObject::~BotPlayerDynamicGameObject()
{
	m_currentPath.clear();
	m_exploredPath.clear();
	m_badBombEscapeNodes.clear();
}

void BotPlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
	PlayerDynamicGameObject::update(deltaTime, mapBorders, insideBlocks, breakableBlocks, powerUps, explosions, players, bombs);

	if (m_playerState == ALIVE && m_playerActionState != WINNING)
	{
		for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
		{
			if (PathFinder::getInstance().isLocationOccupiedByBombOrExplosionPath(bombs, explosions, m_gridX, m_gridY))
			{
                Node currentNode = Node(m_gridX, m_gridY);
				if (PathFinder::calculateClosestSafeNodeFromStartingNode(bombs, explosions, players, this, m_badBombEscapeNodes, currentNode))
				{
                    if (calculatePathToTarget(currentNode.x, currentNode.y))
					{
						m_badBombEscapeNodes.clear();
						m_exploredPath.clear();
					}
					else
					{
						m_badBombEscapeNodes.push_back(Node(currentNode.x, currentNode.y));
					}

					m_currentPathType = 1;
					m_fWaitTime = 0;
					m_fActionTime = 0;
				}

				break;
			}
		}

		if (m_fWaitTime > 0 && m_fActionTime < m_fWaitTime)
		{
			m_fActionTime += deltaTime;
		}
		else
		{
			if (m_currentPathType == 2 && isProposedNodeUnexplored(m_gridX, m_gridY))
			{
				m_exploredPath.push_back(Node(m_gridX, m_gridY));
			}

			if (m_currentPathType != 1)
			{
				determinePlayerTarget(players);
                
                bool isOnTopOfTarget = m_playerTarget != nullptr && m_playerTarget->getGridX() == m_gridX && m_playerTarget->getGridY() == m_gridY;
				if (m_playerTarget != nullptr && !isOnTopOfTarget && calculatePathToTarget(m_playerTarget->getGridX(), m_playerTarget->getGridY()))
				{
					// Great, we have a path to the player, kick ass
					m_currentPathType = 0;
					m_exploredPath.clear();
					m_badBombEscapeNodes.clear();
				}
				else
				{
					// Let's randomly traverse the map
					explore(players, bombs, breakableBlocks, powerUps);
					m_badBombEscapeNodes.clear();
					m_currentPathType = 2;
				}
			}

			if (m_currentPath.size() > 0)
			{
				if (m_currentPathIndex == m_currentPath.size())
				{
                    if (m_currentPathType == 0)
					{
                        // We were pursuing a player and caught up with them...
						// This shouldn't happen since the bot will drop bombs ahead
						// of time and then reroute itself to dodge the bomb
						if (isAbleToDropAdditionalBomb(players, bombs))
						{
							m_gameListener->addLocalEventForPlayer(PLAYER_PLANT_BOMB, *this);
						}
					}
					else if (m_currentPathType == 1)
					{
                        m_fActionTime = 0;
						m_fWaitTime = 3.2f - (m_currentPath.size() * 0.1f) + (m_firePower * 0.2f);
						m_gameListener->addLocalEventForPlayer(PLAYER_MOVE_STOP, *this);
					}

					m_playerTarget = nullptr;
					m_exploredPath.clear();
					m_currentPath.clear();
					m_currentPathIndex = 0;
					m_currentPathType = 0;
				}
				else if (m_currentPathType != 1 && m_currentPathIndex < (m_currentPath.size() - 1) && PathFinder::getInstance().isLocationOccupiedByBombOrExplosionPath(bombs, explosions, m_currentPath.at(m_currentPathIndex).x, m_currentPath.at(m_currentPathIndex).y))
				{
                    m_fActionTime = 0;
					m_fWaitTime = 1;
					m_gameListener->addLocalEventForPlayer(PLAYER_MOVE_STOP, *this);
				}
				else
				{
					bool bombDropped = false;
					if (PathFinder::shouldPlayerPlantBomb(breakableBlocks, players, this))
					{
                        // TODO, only drop bomb if bot is able to escape it

						if (isAbleToDropAdditionalBomb(players, bombs))
						{
							bombDropped = true;
							m_gameListener->addLocalEventForPlayer(PLAYER_PLANT_BOMB, *this);
						}
					}

					if (m_gridX == m_currentPath.at(m_currentPathIndex).x && m_gridY == m_currentPath.at(m_currentPathIndex).y)
					{
                        m_currentPathIndex++;
					}
					else if (!bombDropped)
					{
						if (m_gridX < m_currentPath.at(m_currentPathIndex).x && m_gridY == m_currentPath.at(m_currentPathIndex).y)
						{
                            moveInDirection(DIRECTION_RIGHT);
						}
						else if (m_gridX == m_currentPath.at(m_currentPathIndex).x && m_gridY < m_currentPath.at(m_currentPathIndex).y)
						{
                            moveInDirection(DIRECTION_UP);
						}
						else if (m_gridX > m_currentPath.at(m_currentPathIndex).x && m_gridY == m_currentPath.at(m_currentPathIndex).y)
						{
                            moveInDirection(DIRECTION_LEFT);
						}
						else if (m_gridX == m_currentPath.at(m_currentPathIndex).x && m_gridY > m_currentPath.at(m_currentPathIndex).y)
						{
                            moveInDirection(DIRECTION_DOWN);
						}
					}
				}
			}
		}
	}
}

bool BotPlayerDynamicGameObject::isBot()
{
	return true;
}

PlayerDynamicGameObject * BotPlayerDynamicGameObject::getTarget()
{
	return m_playerTarget;
}

void BotPlayerDynamicGameObject::determinePlayerTarget(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	m_playerTarget = nullptr;

	float shortestPlayerTargetDistance;
	for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
	{
		if ((*itr).get() != this && (*itr)->getPlayerState() == ALIVE)
		{
			float playerTargetDistance = m_position->distSquared((*itr)->getPosition());

			if (m_playerTarget == nullptr)
			{
				m_playerTarget = (*itr).get();
				shortestPlayerTargetDistance = playerTargetDistance;
			}
			else
			{
				if (playerTargetDistance < shortestPlayerTargetDistance)
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
	astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

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
	} while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

	bool isSuccess = false;
	if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
	{

		MapSearchNode *node = astarsearch.GetSolutionStart();

#if DISPLAY_SOLUTION
		cout << "Displaying solution\n";
#endif
		int steps = 0;

		for (;;)
		{
			node = astarsearch.GetSolutionNext();

			if (!node)
			{
				break;
			}
            
			m_currentPath.push_back(MapSearchNode(node->x, node->y));
			steps++;
		};

		// Once you're done with the solution you can free the nodes up
		astarsearch.FreeSolutionNodes();

		isSuccess = steps > 0;
	}
	else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED)
	{
        isSuccess = false;
	}

	astarsearch.EnsureMemoryFreed();

	return isSuccess;
}

void BotPlayerDynamicGameObject::moveInDirection(int direction)
{
	if (getDirection() != direction || !isMoving())
	{
		m_gameListener->addLocalEventForPlayer(direction + 1, *this);
	}
}

void BotPlayerDynamicGameObject::explore(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps)
{
	int gridRightX = m_gridX + 1;
	int gridLeftX = m_gridX - 1;
	int gridTopY = m_gridY + 1;
	int gridBottomY = m_gridY - 1;

	short bestDirection = -1;
	float shortestDistanceToPlayerTarget = 9000;
    Vector2D vectorTarget;
    if(m_playerTarget == nullptr)
    {
        for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = powerUps.begin(); itr != powerUps.end(); itr++)
        {
            vectorTarget = Vector2D((*itr)->getGridX(), (*itr)->getGridY());
            break;
        }
    }
    else
    {
        vectorTarget = Vector2D(m_playerTarget->getGridX(), m_playerTarget->getGridY());
    }

	Vector2D vector = Vector2D(gridRightX, m_gridY);
	float distance = vector.distSquared(vectorTarget);
    
	if (distance < shortestDistanceToPlayerTarget || (distance == shortestDistanceToPlayerTarget && rand() % 2 == 1))
	{
		if (PathFinder::getInstance().getGridCellCost(gridRightX, m_gridY) == 1 && isProposedNodeUnexplored(gridRightX, m_gridY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = DIRECTION_RIGHT;
		}
		else if (PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, gridRightX, m_gridY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = -1;
		}
	}

	vector = Vector2D(gridLeftX, m_gridY);
	distance = vector.distSquared(vectorTarget);
    
	if (distance < shortestDistanceToPlayerTarget || (distance == shortestDistanceToPlayerTarget && rand() % 2 == 1))
	{
		if (PathFinder::getInstance().getGridCellCost(gridLeftX, m_gridY) == 1 && isProposedNodeUnexplored(gridLeftX, m_gridY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = DIRECTION_LEFT;
		}
		else if (PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, gridLeftX, m_gridY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = -1;
		}
	}

	vector = Vector2D(m_gridX, gridTopY);
	distance = vector.distSquared(vectorTarget);
    
	if (distance < shortestDistanceToPlayerTarget || (distance == shortestDistanceToPlayerTarget && rand() % 2 == 1))
	{
		if (PathFinder::getInstance().getGridCellCost(m_gridX, gridTopY) == 1 && isProposedNodeUnexplored(m_gridX, gridTopY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = DIRECTION_UP;
		}
		else if (PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, m_gridX, gridTopY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = -1;
		}
	}

	vector = Vector2D(m_gridX, gridBottomY);
	distance = vector.distSquared(vectorTarget);
    
	if (distance < shortestDistanceToPlayerTarget || (distance == shortestDistanceToPlayerTarget && rand() % 2 == 1))
	{
		if (PathFinder::getInstance().getGridCellCost(m_gridX, gridBottomY) == 1 && isProposedNodeUnexplored(m_gridX, gridBottomY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = DIRECTION_DOWN;
		}
		else if (PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, m_gridX, gridBottomY))
		{
			shortestDistanceToPlayerTarget = distance;
			bestDirection = -1;
		}
	}

	if (bestDirection == -1)
	{
        m_exploredPath.clear();

		if (isAbleToDropAdditionalBomb(players, bombs))
		{
			m_gameListener->addLocalEventForPlayer(PLAYER_PLANT_BOMB, *this);
		}
	}
	else
	{
		moveInDirection(bestDirection);
	}
}

bool BotPlayerDynamicGameObject::isProposedNodeUnexplored(int x, int y)
{
	for (std::vector<Node>::iterator itr = m_exploredPath.begin(); itr != m_exploredPath.end(); itr++)
	{
		if ((*itr).x == x && (*itr).y == y)
		{
			return false;
		}
	}

	return true;
}
