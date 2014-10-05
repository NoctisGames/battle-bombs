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

IcePatch::IcePatch(int gridX, int gridY) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_fStateTime = 0;
}

void IcePatch::update(float deltaTime)
{
    m_fStateTime += deltaTime;
}

float IcePatch::getStateTime()
{
    return m_fStateTime;
}