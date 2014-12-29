//
//  Landmine.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Landmine.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"

Landmine::Landmine(int gridX, int gridY) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 2, GRID_CELL_HEIGHT * 2, 0)
{
    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_state = LM_NORMAL;
    m_fStateTime = 0;
}

void Landmine::update(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if(m_state == LM_EXPLODING)
    {
        if(m_fStateTime > 0.6f)
        {
            m_state = LM_DESTROYED;
        }
    }
}

void Landmine::trigger()
{
    m_state = LM_EXPLODING;
    m_fStateTime = 0;
}

Landmine_State Landmine::getState()
{
    return m_state;
}

float Landmine::getStateTime()
{
    return m_fStateTime;
}