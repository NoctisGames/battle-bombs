//
//  InsideBlock.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 3/24/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "InsideBlock.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"

InsideBlock::InsideBlock(int gridX, int gridY) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 3, GRID_CELL_HEIGHT * 3, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_insideBlockState = IB_NORMAL;
    m_fStateTime = 0;
}

void InsideBlock::update(float deltaTime)
{
    if(m_insideBlockState == IB_STRUCK_BY_METEOR)
    {
        m_fStateTime += deltaTime;
        
        if(m_fStateTime > 0.4f)
        {
            m_insideBlockState = IB_GONE;
        }
    }
    else if(m_insideBlockState == IB_FREEZING)
    {
        m_fStateTime += deltaTime;
        
        if(m_fStateTime > 0.6f)
        {
            m_insideBlockState = IB_FROZEN;
        }
    }
}

void InsideBlock::onHitByIceBall()
{
    m_insideBlockState = IB_FREEZING;
}

void InsideBlock::onDestroy()
{
    m_insideBlockState = IB_STRUCK_BY_METEOR;
}

Inside_Block_State InsideBlock::getInsideBlockState()
{
    return m_insideBlockState;
}

float InsideBlock::getStateTime()
{
    return m_fStateTime;
}