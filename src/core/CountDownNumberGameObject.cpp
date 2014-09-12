//
//  CountDownNumberGameObject.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "CountDownNumberGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

static float targetWidth = GRID_CELL_WIDTH * 3.25f;
static float targetHeight = GRID_CELL_HEIGHT * 2.5f;

CountDownNumberGameObject::CountDownNumberGameObject(float x, float y, float width, float height, Count_Down_Number_Type type) : GameObject(x, y, width, height, 0)
{
    m_fStateTime = 0;
    
    m_fOriginalWidthToTargetWidthDifference = width - targetWidth;
    m_fOriginalHeightToTargetHeightDifference = height - targetHeight;
    
    m_type = type;
    m_remove = false;
}

void CountDownNumberGameObject::update(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if(m_fStateTime < 0.5f)
    {
        m_fWidth -= m_fOriginalWidthToTargetWidthDifference * deltaTime * 2;
        m_fHeight -= m_fOriginalHeightToTargetHeightDifference * deltaTime * 2;
    }
    else
    {
        m_fWidth = targetWidth;
        m_fHeight = targetHeight;
    }
    
    if(m_fStateTime > 1)
    {
        m_remove = true;
    }
    else if(m_fStateTime > 0.9f)
    {
        m_fAngle -= deltaTime * 180;
    }
    else if(m_fStateTime > 0.7f)
    {
        m_fAngle += deltaTime * 180;
    }
    else if(m_fStateTime > 0.6f)
    {
        m_fAngle -= deltaTime * 180;
    }
}

Count_Down_Number_Type CountDownNumberGameObject::getType()
{
    return m_type;
}

bool CountDownNumberGameObject::remove()
{
    return m_remove;
}