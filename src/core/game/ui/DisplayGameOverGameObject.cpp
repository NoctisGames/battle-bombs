//
//  DisplayGameOverGameObject.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "DisplayGameOverGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"

DisplayGameOverGameObject::DisplayGameOverGameObject(float x, float y, float width, float height, Display_Game_Over_Type type) : GameObject(x, y, width, height, 0)
{
    m_fStateTime = 0;
    m_fOrigWidth = width;
    m_fOrigHeight = height;
    m_fSizeScalar = 1;
    m_type = type;
    m_isSizeIncreasing = true;
}

void DisplayGameOverGameObject::update(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if(m_isSizeIncreasing)
    {
        m_fSizeScalar += deltaTime * 0.25f;
        if(m_fSizeScalar > 1.1f)
        {
            m_isSizeIncreasing = false;
        }
    }
    else
    {
        m_fSizeScalar -= deltaTime * 0.1f;
        if(m_fSizeScalar < 0.9f)
        {
            m_isSizeIncreasing = true;
        }
    }
    
    m_fWidth = m_fOrigWidth * m_fSizeScalar;
    m_fHeight = m_fOrigHeight * m_fSizeScalar;
}

Display_Game_Over_Type DisplayGameOverGameObject::getType()
{
    return m_type;
}