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

// For Random Event Generation
#include <stdlib.h>
#include <time.h>

BotPlayerDynamicGameObject::BotPlayerDynamicGameObject(short playerIndex, float x, float y, GameListener *gameListener, int direction) : PlayerDynamicGameObject(playerIndex, x, y, gameListener, direction)
{
    srand((int)time(NULL));
    
    m_playerTarget = nullptr;
}

void BotPlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    PlayerDynamicGameObject::update(deltaTime, mapBorders, insideBlocks, breakableBlocks, powerUps, explosions, players, bombs);

    // BEGIN TEMPORARY AI
    if (m_playerState == ALIVE && m_playerActionState != WINNING)
    {
        if(m_playerTarget == nullptr || m_playerTarget->getPlayerState() != ALIVE)
        {
            determinePlayerTarget(players);
        }
        
        bool isSafeFromBombs = true;
        
        bool shouldPlantBomb = false;
        
        for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
        {
            if(m_gridX == (*itr)->getGridX() && (m_gridY >= (*itr)->getGridY() - (*itr)->getPower() || m_gridY <= (*itr)->getGridY() + (*itr)->getPower()))
            {
                isSafeFromBombs = false;
                if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                {
                    if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                    {
                        if(m_gridY > (*itr)->getGridY())
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                            }
                        }
                        else
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                            }
                        }
                    }
                }
            }
            else if(m_gridY == (*itr)->getGridY() && (m_gridX >= (*itr)->getGridX() - (*itr)->getPower() || m_gridX <= (*itr)->getGridX() + (*itr)->getPower()))
            {
                isSafeFromBombs = false;
                if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                {
                    if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                    {
                        if(m_gridX > (*itr)->getGridX())
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                            }
                        }
                        else
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                            }
                        }
                    }
                }
            }
        }
        
        if(isSafeFromBombs)
        {
            m_firePower = 0;
            
            if(m_gridX == m_playerTarget->getGridX() && m_gridY <= m_playerTarget->getGridY() + m_firePower && m_gridY >= m_playerTarget->getGridY() - m_firePower)
            {
                if(isAbleToDropAdditionalBomb(players, bombs))
                {
                    m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
                }
            }
            else if(m_gridY == m_playerTarget->getGridY() && m_gridX <= m_playerTarget->getGridX() + m_firePower && m_gridX >= m_playerTarget->getGridX() - m_firePower)
            {
                if(isAbleToDropAdditionalBomb(players, bombs))
                {
                    m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
                }
            }
            
            int gridXDistanceToTarget = abs(m_playerTarget->getGridX() - m_gridX);
            int gridYDistanceToTarget = abs(m_playerTarget->getGridY() - m_gridY);
            if(gridXDistanceToTarget > gridYDistanceToTarget)
            {
                if(m_gridX < m_playerTarget->getGridX())
                {
                    if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                    {
                        if(m_gridY < m_playerTarget->getGridY())
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                        else
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                    {
                        if(m_gridY < m_playerTarget->getGridY())
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                        else
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(m_gridY < m_playerTarget->getGridY())
                {
                    if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                    {
                        if(m_gridX < m_playerTarget->getGridX())
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                        else
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(!moveInDirectionIfPossible(DIRECTION_DOWN, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                    {
                        if(m_gridX < m_playerTarget->getGridX())
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                        else
                        {
                            if(!moveInDirectionIfPossible(DIRECTION_LEFT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                            {
                                if(!moveInDirectionIfPossible(DIRECTION_UP, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
                                {
                                    moveInDirectionIfPossible(DIRECTION_RIGHT, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks);
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if(shouldPlantBomb && isAbleToDropAdditionalBomb(players, bombs))
        {
            m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
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
            else if(m_playerTarget != nullptr)
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

bool BotPlayerDynamicGameObject::isAbleToMoveDirection(short direction, bool &shouldPlantBomb, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    int rightGridX = m_gridX + 1;
    int upGridY = m_gridY + 1;
    int leftGridX = m_gridX - 1;
    int downGridY = m_gridY - 1;
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if(direction == DIRECTION_RIGHT && (*itr)->getGridX() == rightGridX && (*itr)->getGridY() == m_gridY)
        {
            shouldPlantBomb = true;
            return false;
        }
        else if(direction == DIRECTION_UP && (*itr)->getGridX() == m_gridX && (*itr)->getGridY() == upGridY)
        {
            shouldPlantBomb = true;
            return false;
        }
        else if(direction == DIRECTION_LEFT && (*itr)->getGridX() == leftGridX && (*itr)->getGridY() == m_gridY)
        {
            shouldPlantBomb = true;
            return false;
        }
        else if(direction == DIRECTION_DOWN && (*itr)->getGridX() == m_gridX && (*itr)->getGridY() == downGridY)
        {
            shouldPlantBomb = true;
            return false;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if(direction == DIRECTION_RIGHT && (*itr)->getGridX() == rightGridX && (*itr)->getGridY() == m_gridY)
        {
            return false;
        }
        else if(direction == DIRECTION_UP && (*itr)->getGridX() == m_gridX && (*itr)->getGridY() == upGridY)
        {
            return false;
        }
        else if(direction == DIRECTION_LEFT && (*itr)->getGridX() == leftGridX && (*itr)->getGridY() == m_gridY)
        {
            return false;
        }
        else if(direction == DIRECTION_DOWN && (*itr)->getGridX() == m_gridX && (*itr)->getGridY() == downGridY)
        {
            return false;
        }
    }
    
    if(direction == DIRECTION_RIGHT && rightGridX >= NUM_GRID_CELLS_PER_ROW)
    {
        return false;
    }
    
    if(direction == DIRECTION_UP && upGridY >= GRID_CELL_NUM_ROWS)
    {
        return false;
    }
    
    if(direction == DIRECTION_LEFT && leftGridX < 0)
    {
        return false;
    }
    
    if(direction == DIRECTION_DOWN && downGridY < 0)
    {
        return false;
    }
    
    if(direction == DIRECTION_RIGHT && m_gridY <= 2 && rightGridX >= NUM_GRID_CELLS_PER_ROW - 3)
    {
        return false;
    }
    
    if(direction == DIRECTION_DOWN && downGridY <= 2 && (m_gridX <= 2 || m_gridX >= NUM_GRID_CELLS_PER_ROW - 3))
    {
        return false;
    }
    
    if(direction == DIRECTION_LEFT && m_gridY <= 2 && leftGridX <= 2)
    {
        return false;
    }
    
    return true;
}

bool BotPlayerDynamicGameObject::moveInDirectionIfPossible(short direction, bool &shouldPlantBomb, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    if(isAbleToMoveDirection(direction, shouldPlantBomb, mapBorders, insideBlocks, breakableBlocks))
    {
        if(getDirection() != direction || !isMoving())
        {
            m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + direction + 1);
        }
        
        return true;
    }
    
    return false;
}