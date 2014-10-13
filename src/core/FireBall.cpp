//
//  FireBall.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/13/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define TIME_BETWEEN_FIRE_BALLS 0.23206751054852f

#include "FireBall.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "FallingObjectShadow.h"

FireBall::FireBall(int gridX, int gridY, int index, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks) : DynamicGridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_fTargetY = m_position->getY();
    m_fTimeUntilAppearance = TIME_BETWEEN_FIRE_BALLS * (float)index;
    
    m_position->set(m_position->getX(), SCREEN_HEIGHT_X2 + GRID_CELL_HEIGHT);
    m_velocity->set(0, -8);
    m_acceleration->set(0, -12);
    
    m_fallingObjectShadow = std::unique_ptr<FallingObjectShadow>(new FallingObjectShadow(gridX, gridY, insideBlocks, breakableBlocks));
    
    m_fStateTime = 0;
    m_isVisible = false;
    m_isDescending = false;
    m_isTargetReached = false;
    m_isExplosionCompleted = false;
}

void FireBall::update(float deltaTime, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    if(m_isTargetReached)
    {
        if(m_fStateTime >= 0.5f)
        {
            m_isExplosionCompleted = true;
        }
    }
    else
    {
        if(m_isDescending)
        {
            m_fStateTime += deltaTime;
            
            m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
            m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
            
            if(m_position->getY() <= m_fTargetY)
            {
                m_isTargetReached = true;
                m_isVisible = false;
                m_fStateTime = 0;
            }
        }
        
        if(m_isVisible)
        {
            m_fallingObjectShadow->update(deltaTime, breakableBlocks);
            
            if(m_fallingObjectShadow->getFallingObjectShadowState() == CLOSER)
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
}

float FireBall::getStateTime()
{
    return m_fStateTime;
}

bool FireBall::isVisible()
{
    return m_isVisible;
}

bool FireBall::isDescending()
{
    return m_isDescending;
}

bool FireBall::isTargetReached()
{
    return m_isTargetReached;
}

bool FireBall::isExplosionCompleted()
{
    return m_isExplosionCompleted;
}

FallingObjectShadow & FireBall::getShadow()
{
    return *m_fallingObjectShadow;
}