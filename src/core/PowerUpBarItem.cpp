//
//  PowerUpBarItem.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 7/12/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PowerUpBarItem.h"
#include "Vector2D.h"
#include "Rectangle.h"

PowerUpBarItem::PowerUpBarItem(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_powerUpType = NONE;
}

void PowerUpBarItem::setPowerUpType(Power_Up_Type powerUpType)
{
    m_powerUpType = powerUpType;
}

Power_Up_Type PowerUpBarItem::getPowerUpType()
{
    return m_powerUpType;
}

void PowerUpBarItem::setLevel(int level)
{
    m_iLevel = level;
}

int PowerUpBarItem::getLevel()
{
    return m_iLevel;
}