//
//  DisplayBattleGameObject.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "DisplayBattleGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

DisplayBattleGameObject::DisplayBattleGameObject(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_fStateTime = 0;
}

void DisplayBattleGameObject::update(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if(m_fStateTime < 0.5f || m_fStateTime > 2.5f)
    {
        m_position->add(SCREEN_WIDTH * deltaTime * 2, 0);
    }
}
