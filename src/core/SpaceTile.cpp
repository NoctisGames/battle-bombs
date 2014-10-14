//
//  SpaceTile.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/8/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define TIME_BETWEEN_FALLING_SPACE_TILES 0.416f

#include "SpaceTile.h"
#include "GameConstants.h"
#include "ResourceConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameListener.h"
#include "PlayerDynamicGameObject.h"
#include "BombGameObject.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "PowerUp.h"
#include "PathFinder.h"

SpaceTile::SpaceTile(int gridX, int gridY, int index, GameListener *gameListener) : DynamicGridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 3, 0)
{
    m_position->add(0, GRID_CELL_HEIGHT / 2);
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    m_velocity->set(0, 0);
    m_acceleration->set(0, -12);
    
    m_gameListener = gameListener;
    m_fallingPlayer = nullptr;
    m_spaceTileState = ST_NORMAL;
    m_fStateTime = 0;
    m_fOrigY = m_position->getY();
    m_fTimeUntilDislodging = TIME_BETWEEN_FALLING_SPACE_TILES * (float)index;
    m_shouldPlayerStartFalling = false;
}

void SpaceTile::update(float deltaTime, bool isSuddenDeath, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject>> &bombs, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    if(m_spaceTileState == ST_NORMAL)
    {
        m_fStateTime += deltaTime;
        if(m_fStateTime >= m_fTimeUntilDislodging)
        {
            m_spaceTileState = ST_DISLODGING;
            m_fStateTime = 0;
            
            m_gameListener->playSound(SOUND_DISLODGING_SPACE_TILE);
        }
        
        return;
    }
    else
    {
        m_fStateTime += deltaTime;
        
        if(m_spaceTileState == ST_DISLODGING)
        {
            for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr2 = bombs.begin(); itr2 != bombs.end(); )
            {
                if (m_gridX == (*itr2)->getGridX() && m_gridY == (*itr2)->getGridY())
                {
                    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr3 = players.begin(); itr3 != players.end(); itr3++)
                    {
                        (*itr3)->handleBombErasure((*itr2).get());
                    }
                    
                    itr2 = bombs.erase(itr2);
                }
                else
                {
                    itr2++;
                }
            }
            
            for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr2 = insideBlocks.begin(); itr2 != insideBlocks.end(); )
            {
                if (m_gridX == (*itr2)->getGridX() && m_gridY == (*itr2)->getGridY())
                {
                    itr2 = insideBlocks.erase(itr2);
                }
                else
                {
                    itr2++;
                }
            }
            
            for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr2 = breakableBlocks.begin(); itr2 != breakableBlocks.end(); )
            {
                if (m_gridX == (*itr2)->getGridX() && m_gridY == (*itr2)->getGridY())
                {
                    itr2 = breakableBlocks.erase(itr2);
                }
                else
                {
                    itr2++;
                }
            }
            
            for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr2 = powerUps.begin(); itr2 != powerUps.end(); )
            {
                if (m_gridX == (*itr2)->getGridX() && m_gridY == (*itr2)->getGridY())
                {
                    itr2 = powerUps.erase(itr2);
                }
                else
                {
                    itr2++;
                }
            }
            
            if(m_fStateTime >= 0.8f)
            {
                m_spaceTileState = ST_FALLING;
                m_fStateTime = 0;
                
                PathFinder::getInstance().occupyGameGridCell(m_gridX, m_gridY);
                
                m_gameListener->playSound(SOUND_FALLING_SPACE_TILE);
            }
        }
        else if(m_spaceTileState == ST_FALLING)
        {
            m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
            m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
            
            updateBounds();
            
            if(m_position->getY() < (m_fOrigY - 1))
            {
                if(m_fallingPlayer != nullptr)
                {
                    m_shouldPlayerStartFalling = true;
                }
            }
            
            if(m_position->getY() < 0)
            {
                m_spaceTileState = ST_ENTERING_ATMOSPHERE;
            }
        }
    }
}

void SpaceTile::setFallingPlayer(PlayerDynamicGameObject *fallingPlayer)
{
    m_fallingPlayer = fallingPlayer;
}

Space_Tile_State SpaceTile::getSpaceTileState()
{
    return m_spaceTileState;
}

float SpaceTile::getStateTime()
{
    return m_fStateTime;
}

bool SpaceTile::shouldPlayerStartFalling()
{
    return m_shouldPlayerStartFalling;
}

bool SpaceTile::isRemove()
{
    return m_spaceTileState == ST_ENTERING_ATMOSPHERE;
}
