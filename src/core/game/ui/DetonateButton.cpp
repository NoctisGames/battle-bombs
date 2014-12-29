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
    m_isDeactivating = false;
}

void DetonateButton::update(float deltaTime)
{
    if(m_isActivating)
    {
        m_fStateTime += deltaTime;
        if(m_fStateTime > 0.4f)
        {
            m_state = DB_ON;
            m_isActivating = false;
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
    else if(m_isDeactivating)
    {
        m_fStateTime += deltaTime;
        if(m_fStateTime > 0.4f)
        {
            m_state = DB_OFF;
            m_isDeactivating = false;
        }
        else if(m_fStateTime > 0.3f)
        {
            m_state = DB_FRAME_1;
        }
        else if(m_fStateTime > 0.2f)
        {
            m_state = DB_FRAME_2;
        }
        else if(m_fStateTime > 0.1f)
        {
            m_state = DB_FRAME_3;
        }
    }
}

float DetonateButton::getStateTime()
{
    return m_fStateTime;
}

void DetonateButton::setNumRemoteBombsDeployed(int numRemoteBombsDeployed)
{
    if(m_iNumRemoteBombsDeployed == 0 && numRemoteBombsDeployed > 0)
    {
        m_isActivating = true;
        m_fStateTime = 0;
    }
    else if(m_iNumRemoteBombsDeployed > 0 && numRemoteBombsDeployed == 0)
    {
        m_isDeactivating = false;
        m_fStateTime = 0;
    }
    
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