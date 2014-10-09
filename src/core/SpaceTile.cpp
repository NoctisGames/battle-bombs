//
//  SpaceTile.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/8/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "SpaceTile.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "PlayerDynamicGameObject.h"

SpaceTile::SpaceTile(int gridX, int gridY) : DynamicGridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 3, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(lowerLeft.getX(), lowerLeft.getY() + GRID_CELL_HEIGHT / 2);
    
    m_velocity->set(0, 0);
    m_acceleration->set(0, -12);
    
    m_spaceTileState = ST_NORMAL;
    m_fStateTime = 0;
    m_fOrigY = m_position->getY();
}

void SpaceTile::update(float deltaTime, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    if(m_spaceTileState == ST_NORMAL)
    {
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
                    m_fallingPlayer = (*itr).get();
                    break;
                }
            }
        }
        else if(m_spaceTileState == ST_FALLING)
        {
            m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
            m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
            
            if(m_position->getY() < (m_fOrigY - 2))
            {
                // TODO, start making player descend as well
            }
            
            if(m_fStateTime >= 0.7f)
            {
                // TODO, dissappear
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