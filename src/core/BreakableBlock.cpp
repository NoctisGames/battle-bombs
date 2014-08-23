//
//  BreakableBlock.cpp
//  battlebombs
//
//  Created by Mikel Adkins on 3/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "BreakableBlock.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

BreakableBlock::BreakableBlock(int gridX, int gridY, int powerUpFlag) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 3, GRID_CELL_HEIGHT * 3, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_breakableBlockState = NORMAL;
    m_fStateTime = 0;
    m_powerUpFlag = powerUpFlag;
}

void BreakableBlock::update(float deltaTime)
{
    if(m_breakableBlockState == EXPLODING)
    {
        m_fStateTime += deltaTime;
        
        if(m_fStateTime > 1)
        {
            m_breakableBlockState = DESTROYED;
        }
    }
}

void BreakableBlock::onDestroy()
{
    m_breakableBlockState = EXPLODING;
}

bool BreakableBlock::hasPowerUp()
{
    return m_powerUpFlag != POWER_UP_TYPE_NONE;
}

float BreakableBlock::getX()
{
    return getPosition().getX();
}

float BreakableBlock::getY()
{
    return getPosition().getY();
}

Breakable_Block_State BreakableBlock::getBreakableBlockState()
{
    return m_breakableBlockState;
}

float BreakableBlock::getStateTime()
{
    return m_fStateTime;
}

int BreakableBlock::getPowerUpFlag()
{
	return m_powerUpFlag;
}