//
//  DisplayXMovingGameObject.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "DisplayXMovingGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

DisplayXMovingGameObject::DisplayXMovingGameObject(float x, float y, float width, float height, Display_X_Moving_Type type) : GameObject(x, y, width, height, 0)
{
    m_fStateTime = 0;
    m_isAnimating = false;
    m_type = type;
}

void DisplayXMovingGameObject::begin()
{
    m_isAnimating = true;
}

void DisplayXMovingGameObject::update(float deltaTime)
{
    if(m_isAnimating)
    {
        m_fStateTime += deltaTime;
        
        if(m_fStateTime < 0.33f || m_fStateTime > 0.83f)
        {
            m_position->add(SCREEN_WIDTH * deltaTime * 3, 0);
        }
        else
        {
            m_position->set(SCREEN_WIDTH / 2, m_position->getY());
        }
    }
}

Display_X_Moving_Type DisplayXMovingGameObject::getType()
{
    return m_type;
}
