//
//  BaseTile.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "BaseTile.h"
#include "GameConstants.h"
#include "ResourceConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameListener.h"
#include "PlayerDynamicGameObject.h"
#include "BombGameObject.h"
#include "BreakableBlock.h"
#include "PowerUp.h"
#include "PathFinder.h"
#include "RegeneratingDoor.h"

#define TIME_BETWEEN_ACTIVATING_BASE_TILES 0.3f

BaseTile::BaseTile(int gridX, int gridY, int index, GameListener *gameListener) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_gameListener = gameListener;
    m_state = BT_NORMAL;
    m_fStateTime = 0;
    m_fTimeUntilActivation = TIME_BETWEEN_ACTIVATING_BASE_TILES * (float)index;
}

void BaseTile::update(float deltaTime, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject>> &bombs, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<RegeneratingDoor>> &doors, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    m_fStateTime += deltaTime;
    
    if(m_state == BT_NORMAL)
    {
        if(m_fStateTime >= m_fTimeUntilActivation)
        {
            m_state = BT_ACTIVATED;
            m_fStateTime -= m_fTimeUntilActivation;
            
            m_gameListener->playSound(SOUND_BASE_TILE_TRIGGER);
        }
        
        return;
    }
    else
    {
        if(m_state == BT_ACTIVATED)
        {
            for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); )
            {
                if (m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY())
                {
                    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr2 = players.begin(); itr2 != players.end(); itr2++)
                    {
                        (*itr2)->handleBombErasure((*itr).get());
                    }
                    
                    itr = bombs.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
            
            for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); )
            {
                if (m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY())
                {
                    itr = breakableBlocks.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
            
            for (std::vector < std::unique_ptr < RegeneratingDoor >> ::iterator itr = doors.begin(); itr != doors.end(); )
            {
                if (m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY())
                {
                    itr = doors.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
            
            for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = powerUps.begin(); itr != powerUps.end(); )
            {
                if (m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY())
                {
                    itr = powerUps.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
            
            if(m_fStateTime >= 0.7f)
            {
                m_state = BT_EXPLODING;
                
                PathFinder::getInstance().occupyGameGridCell(m_gridX, m_gridY);
                
                m_gameListener->playSound(SOUND_MINI_EXPLOSION);
            }
        }
        else if(m_state == BT_EXPLODING)
        {
            if(m_fStateTime >= 1.5f)
            {
                m_state = BT_DESTROYED;
            }
        }
    }
}

float BaseTile::getStateTime()
{
    return m_fStateTime;
}

Base_Tile_State BaseTile::getState()
{
    return m_state;
}

void BaseTile::handleTimeSinceSuddenDeathModeBegan(float timeSinceSuddenDeath)
{
    float delta = timeSinceSuddenDeath - m_fTimeUntilActivation;
    
    if(delta > 0.7f)
    {
        m_state = BT_EXPLODING;
        m_fStateTime = delta;
        
        PathFinder::getInstance().occupyGameGridCell(m_gridX, m_gridY);
        
        if(delta > 1.5f)
        {
            m_state = BT_DESTROYED;
        }
    }
    else if(delta > 0)
    {
        m_state = BT_ACTIVATED;
        m_fStateTime = delta;
    }
    else
    {
        m_fStateTime = timeSinceSuddenDeath;
    }
}