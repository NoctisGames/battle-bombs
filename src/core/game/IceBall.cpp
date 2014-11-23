//
//  IceBall.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 10/3/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#define TIME_BETWEEN_ICE_BALLS 0.23628691983122f

#include "IceBall.h"
#include "GameConstants.h"
#include "ResourceConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameListener.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "FallingObjectShadow.h"

IceBall::IceBall(int gridX, int gridY, int index, GameListener *gameListener, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks) : DynamicGridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_gameListener = gameListener;
    
    m_fTargetY = m_position->getY();
    m_fTimeUntilAppearance = TIME_BETWEEN_ICE_BALLS * (float)index;
    
    m_position->set(m_position->getX(), WORLD_HEIGHT + GRID_CELL_HEIGHT);
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
            
            m_gameListener->playSound(SOUND_CRASHING_ICE_BALL);
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
            
            m_gameListener->playSound(SOUND_FALLING_OBJECT);
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

bool IceBall::isDescending()
{
    return m_isDescending;
}

bool IceBall::isTargetReached()
{
    return m_isTargetReached;
}

FallingObjectShadow & IceBall::getShadow()
{
    return *m_fallingObjectShadow;
}

void IceBall::handleTimeSinceSuddenDeathModeBegan(float timeSinceSuddenDeath)
{
    float delta = timeSinceSuddenDeath - m_fTimeUntilAppearance;
    
    if(timeSinceSuddenDeath > (m_fTimeUntilAppearance + 1.5f))
    {
        m_isVisible = true;
        
        m_fallingObjectShadow->handleTimeSinceSuddenDeathModeBegan(delta);
        
        m_isDescending = true;
        
        m_fStateTime = delta - 1.5f;
        
        m_velocity->add(m_acceleration->getX() * m_fStateTime, m_acceleration->getY() * m_fStateTime);
        m_position->add(m_velocity->getX() * m_fStateTime, m_velocity->getY() * m_fStateTime);
        
        if(m_position->getY() <= m_fTargetY)
        {
            m_isTargetReached = true;
            
            m_position->setY(m_fTargetY);
        }
    }
    else if(timeSinceSuddenDeath > m_fTimeUntilAppearance)
    {
        m_isVisible = true;
        m_fStateTime = 0;
        
        m_fallingObjectShadow->handleTimeSinceSuddenDeathModeBegan(delta);
    }
    else
    {
        m_fStateTime = timeSinceSuddenDeath;
    }
}