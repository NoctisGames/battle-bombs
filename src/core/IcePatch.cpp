//
//  IcePatch.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "IcePatch.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "BombGameObject.h"

IcePatch::IcePatch(int gridX, int gridY) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_fStateTime = 0;
}

void IcePatch::update(float deltaTime, std::vector<std::unique_ptr<BombGameObject>> &bombs)
{
    m_fStateTime += deltaTime;
    
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
        {
            // If an Ice Ball lands on a Bomb, destroy that bomb
            (*itr)->destroy();
        }
    }
}

float IcePatch::getStateTime()
{
    return m_fStateTime;
}