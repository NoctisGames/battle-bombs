//
//  PowerUp.cpp
//  battlebombs
//
//  Created by Mikel Adkins on 5/10/2014.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PowerUp.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

PowerUp::PowerUp(int gridX, int gridY, int powerUpIndex) : GridGameObject(gridX, gridY, GRID_CELL_WIDTH * 4 / 5, GRID_CELL_HEIGHT * 4 / 5, 0)
{
    m_fStateTime = 0;
	m_isPickedUp = false;
	m_powerUpFlag = powerUpIndex;
	switch (powerUpIndex)
	{
		case 0: m_powerUpType = NONE;
				break;
		case 1: m_powerUpType = BOMB;
				break;
		case 2: m_powerUpType = FIRE;
				break;
		case 3: m_powerUpType = SPEED;
				break;
		case 4: m_powerUpType = PUSH;
				break;
		case 5: m_powerUpType = THROW;
				break;
		default: m_powerUpType = NONE;
				break;
	}
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

Power_Up_Type PowerUp::getType()
{
	return m_powerUpType;
}

int PowerUp:: getPowerUpFlag()
{
	return m_powerUpFlag;
}