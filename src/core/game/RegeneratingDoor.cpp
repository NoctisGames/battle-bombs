//
//  RegeneratingDoor.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "RegeneratingDoor.h"
#include "GameConstants.h"
#include "PathFinder.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "BombGameObject.h"
#include "PlayerDynamicGameObject.h"

RegeneratingDoor::RegeneratingDoor(int gridX, int gridY) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, 0)
{
    m_fStateTime = 0;
    
    m_isExploding = false;
    m_isDestroyed = false;
}

void RegeneratingDoor::update(float deltaTime, std::vector<std::unique_ptr<BombGameObject>> &bombs, std::vector<std::unique_ptr<Explosion>> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    if(m_isExploding)
    {
        m_fStateTime += deltaTime;
        if(m_fStateTime > 0.8f)
        {
            m_fStateTime = 0;
            m_isDestroyed = true;
            m_isExploding = false;
            
            PathFinder::getInstance().freeGameGridCell(m_gridX, m_gridY);
        }
    }
    else if(m_isDestroyed)
    {
        m_fStateTime += deltaTime;
        if(m_fStateTime > 20 && !PathFinder::isLocationOccupiedByBombOrExplosionPath(bombs, explosions, m_gridX, m_gridY, false) && !PathFinder::isLocationOccupiedByPlayer(players, m_gridX, m_gridY))
        {
            m_fStateTime = 0;
            m_isDestroyed = false;
            
            PathFinder::getInstance().occupyGameGridCell(m_gridX, m_gridY);
        }
    }
}

void RegeneratingDoor::onDestroy()
{
    m_isExploding = true;
}

float RegeneratingDoor::getStateTime()
{
    return m_fStateTime;
}

bool RegeneratingDoor::isExploding()
{
    return m_isExploding;
}

bool RegeneratingDoor::isDestroyed()
{
    return m_isDestroyed;
}