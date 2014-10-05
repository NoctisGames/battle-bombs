//
//  IceBall.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/3/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define TIME_BETWEEN_ICE_BALLS 0.2156862745098f

#include "IceBall.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "FallingObjectShadow.h"

IceBall::IceBall(int gridX, int gridY, int index, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks) : DynamicGridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_fTargetY = m_position->getY();
    m_fTimeUntilAppearance = TIME_BETWEEN_ICE_BALLS * (float)index;
    
    m_position->set(m_position->getX(), SCREEN_HEIGHT_X2 + GRID_CELL_HEIGHT);
    m_velocity->set(0, -8);
    m_acceleration->set(0, -12);
    
    m_fallingObjectShadow = std::unique_ptr<FallingObjectShadow>(new FallingObjectShadow(gridX, gridY, insideBlocks, breakableBlocks));
    
    m_fStateTime = 0;
    m_isVisible = false;
    m_isDescending = false;
    m_isTargetReached = false;
}

void IceBall::update(float deltaTime, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    if(m_isDescending)
    {
        m_fStateTime += deltaTime;
        
        m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
        m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
        
        if(m_position->getY() <= m_fTargetY)
        {
            m_isTargetReached = true;
        }
    }
    else if(m_isVisible)
    {
        m_fallingObjectShadow->update(deltaTime, breakableBlocks);
        
        if(m_fallingObjectShadow->getFallingObjectShadowState() == CLOSEST)
        {
            m_isDescending = true;
        }
    }
    else
    {
        m_fStateTime += deltaTime;
        
        if(m_fStateTime >= m_fTimeUntilAppearance)
        {
            m_isVisible = true;
             m_fStateTime = 0;
        }
    }
}

float IceBall::getStateTime()
{
    return m_fStateTime;
}

bool IceBall::isVisible()
{
    return m_isVisible;
}

bool IceBall::isTargetReached()
{
    return m_isTargetReached;
}

FallingObjectShadow & IceBall::getShadow()
{
    return *m_fallingObjectShadow;
}