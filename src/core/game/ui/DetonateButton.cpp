//
//  DetonateButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "DetonateButton.h"
#include "Vector2D.h"
#include "Rectangle.h"

DetonateButton::DetonateButton(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_fStateTime = 0;
    m_iNumRemoteBombsDeployed = 0;
    m_state = DB_OFF;
    m_isActivating = false;
    m_isActivated = false;
}

void DetonateButton::update(float deltaTime)
{
    if(!m_isActivated && m_isActivating)
    {
        m_fStateTime += deltaTime;
        if(m_fStateTime > 0.4f)
        {
            m_state = DB_ON;
            m_isActivating = false;
            m_isActivated = true;
        }
        else if(m_fStateTime > 0.3f)
        {
            m_state = DB_FRAME_3;
        }
        else if(m_fStateTime > 0.2f)
        {
            m_state = DB_FRAME_2;
        }
        else if(m_fStateTime > 0.1f)
        {
            m_state = DB_FRAME_1;
        }
    }
}

float DetonateButton::getStateTime()
{
    return m_fStateTime;
}

void DetonateButton::setActivating(bool isActivating)
{
    if(!m_isActivating && isActivating)
    {
        m_fStateTime = 0;
    }
    
    m_isActivating = isActivating;
}

void DetonateButton::setNumRemoteBombsDeployed(int numRemoteBombsDeployed)
{
    m_iNumRemoteBombsDeployed = numRemoteBombsDeployed;
}

int DetonateButton::getNumRemoteBombsDeployed()
{
    return m_iNumRemoteBombsDeployed;
}

Detonate_Button_State DetonateButton::getState()
{
    return m_state;
}