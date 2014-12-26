//
//  PowerUp.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 5/10/2014.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "PowerUp.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

PowerUp::PowerUp(int gridX, int gridY, int powerUpType) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 4 / 5, GRID_CELL_HEIGHT * 4 / 5, 0)
{
    m_position->add(0, GRID_CELL_HEIGHT / 4);
    
    m_fStateTime = 0;
	m_isPickedUp = false;
	m_powerUpType = powerUpType;
}

void PowerUp::update(float deltaTime)
{
    m_fStateTime += deltaTime;
}

void PowerUp::onPickedUp()
{
	m_isPickedUp = true;
}

bool PowerUp::isPickedUp()
{
	return m_isPickedUp;
}

float PowerUp::getStateTime()
{
    return m_fStateTime;
}

int PowerUp::getType()
{
	return m_powerUpType;
}