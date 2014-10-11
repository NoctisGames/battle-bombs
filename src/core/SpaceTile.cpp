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
#include "Vector2D.h"
#include "Rectangle.h"
#include "PlayerDynamicGameObject.h"
#include "PathFinder.h"

SpaceTile::SpaceTile(int gridX, int gridY, int index) : DynamicGridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 3, 0)
{
    m_position->add(0, GRID_CELL_HEIGHT / 2);
    m_velocity->set(0, 0);
    m_acceleration->set(0, -12);
    
    m_spaceTileState = ST_NORMAL;
    m_fStateTime = 0;
    m_fOrigY = m_position->getY();
    
    m_fTimeUntilDislodging = TIME_BETWEEN_FALLING_SPACE_TILES * (float)index;
}

void SpaceTile::update(float deltaTime, bool isSuddenDeath, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    if(m_spaceTileState == ST_NORMAL)
    {
        if(isSuddenDeath)
        {
            m_fStateTime += deltaTime;
            if(m_fStateTime >= m_fTimeUntilDislodging)
            {
                m_spaceTileState = ST_DISLODGING;
                m_fStateTime = 0;
            }
        }
        
        return;
    }
    else
    {
        m_fStateTime += deltaTime;
        
        if(m_spaceTileState == ST_DISLODGING)
        {
            if(m_fStateTime >= 0.8f)
            {
                m_spaceTileState = ST_FALLING;
                m_fStateTime = 0;
                
                for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
                {
                    if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
                    {
                        m_fallingPlayer = (*itr).get();
                        m_fallingPlayer->onTrappedOnFallingSpaceTile();
                        PathFinder::getInstance().occupyGameGridCell(m_gridX, m_gridY);
                        break;
                    }
                }
            }
        }
        else if(m_spaceTileState == ST_FALLING)
        {
            m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
            m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
            
            if(m_position->getY() < (m_fOrigY - 1))
            {
                m_fallingPlayer->onFall();
            }
            
            if(m_position->getY() < 0)
            {
                m_spaceTileState = ST_ENTERING_ATMOSPHERE;
            }
        }
    }
}

Space_Tile_State SpaceTile::getSpaceTileState()
{
    return m_spaceTileState;
}

float SpaceTile::getStateTime()
{
    return m_fStateTime;
}

bool SpaceTile::isRemove()
{
    return m_spaceTileState == ST_ENTERING_ATMOSPHERE;
}
