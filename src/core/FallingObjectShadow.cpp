//
//  FallingObjectShadow.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "FallingObjectShadow.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"

FallingObjectShadow::FallingObjectShadow(int gridX, int gridY, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_fStateTime = 0;
    m_fallingObjectShadowState = FARTHEST;
    m_isTargetInsideBlock = isTargetInsideBlock(insideBlocks);
    m_isTargetBreakableBlock = isTargetBreakableBlock(breakableBlocks);
}

void FallingObjectShadow::update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    m_fStateTime += deltaTime;
    
    if(m_fStateTime > 2.0f)
    {
        m_fallingObjectShadowState = CLOSEST;
    }
    else if(m_fStateTime > 1.5f)
    {
        m_fallingObjectShadowState = CLOSE;
    }
    else if(m_fStateTime > 1.0f)
    {
        m_fallingObjectShadowState = HALFWAY;
    }
    else if(m_fStateTime > 0.5f)
    {
        m_fallingObjectShadowState = FAR;
    }
    
    if(m_isTargetBreakableBlock)
    {
        m_isTargetBreakableBlock = m_targetBreakableBlock->getBreakableBlockState() == BB_NORMAL;
    }
    
    if(m_isTargetInsideBlock || m_isTargetBreakableBlock)
    {
        m_position->set(m_position->getX(), GAME_Y + GRID_CELL_HEIGHT * m_gridY + GRID_CELL_HEIGHT / 2.0f + GRID_CELL_HEIGHT / 2.0f);
    }
    else
    {
        m_position->set(m_position->getX(), GAME_Y + GRID_CELL_HEIGHT * m_gridY + GRID_CELL_HEIGHT / 2.0f);
    }
}

float FallingObjectShadow::getStateTime()
{
    return m_fStateTime;
}

Falling_Object_Shadow_State FallingObjectShadow::getFallingObjectShadowState()
{
    return m_fallingObjectShadowState;
}

InsideBlock * FallingObjectShadow::getTargetInsideBlock()
{
    return m_targetInsideBlock;
}

BreakableBlock * FallingObjectShadow::getTargetBreakableBlock()
{
    return m_targetBreakableBlock;
}

bool FallingObjectShadow::isTargetOccupiedByInsideBlock()
{
    return m_isTargetInsideBlock;
}

bool FallingObjectShadow::isTargetOccupiedByBreakableBlock()
{
    return m_isTargetBreakableBlock;
}

#pragma mark <Private>

bool FallingObjectShadow::isTargetInsideBlock(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks)
{
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if(m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY())
        {
            m_targetInsideBlock = (*itr).get();
            return true;
        }
    }
    
    return false;
}

bool FallingObjectShadow::isTargetBreakableBlock(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if(m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY())
        {
            m_targetBreakableBlock = (*itr).get();
            return true;
        }
    }
    
    return false;
}