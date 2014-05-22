//
//  Explosion.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 4/30/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "Explosion.h"
#include "OverlapTester.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameEvent.h"

Explosion::Explosion(Explosion_Type explosionType, float x, float y, float angle, float width, float height) : GameObject(x, y, width, height, angle)
{
    m_explosionType = explosionType;
    m_fStateTime = 0;
    m_isComplete = false;
}

void Explosion::update(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if(m_fStateTime > 0.6f)
    {
        m_isComplete = true;
    }
}

float Explosion::getStateTime()
{
    return m_fStateTime;
}

Explosion_Type Explosion::getExplosionType()
{
    return m_explosionType;
}

float Explosion::isComplete()
{
    return m_isComplete;
}