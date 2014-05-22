//
//  PowerUp.cpp
//  bomberparty
//
//  Created by Mikel Adkins on 5/10/2014.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PowerUp.h"
#include "Vector2D.h"
#include "Rectangle.h"

PowerUp::PowerUp(float x, float y, int powerUpIndex) : GameObject(x, y, 1, 1.25f, 0)
{
	m_isPickedUp = false;
	m_powerUpFlag = powerUpIndex;
	switch (powerUpIndex)
	{
		case 0: m_powerUpType = NONE;
				break;
		case 1: m_powerUpType = BOMBS;
				break;
		case 2: m_powerUpType = FIRE;
				break;
		case 3: m_powerUpType = SPEED;
				break;
		case 4: m_powerUpType = KICK;
				break;
		default: m_powerUpType = NONE;
				break;
	}
}

void PowerUp::onPickedUp()
{
	m_isPickedUp = true;
}

bool PowerUp::isPickedUp()
{
	return m_isPickedUp;
}

Power_Up_Type PowerUp::getType()
{
	return m_powerUpType;
}

int PowerUp:: getPowerUpFlag()
{
	return m_powerUpFlag;
}