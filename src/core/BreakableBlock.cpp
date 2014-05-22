//
//  BreakableBlock.cpp
//  bomberparty
//
//  Created by Mikel Adkins on 3/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "BreakableBlock.h"
#include "Vector2D.h"
#include "Rectangle.h"

BreakableBlock::BreakableBlock(int gridX, int gridY, int powerUpFlag) : GridGameObject(gridX, gridY, 1, 1.25f, 0)
{
    m_isDestroyed = false;
	m_hasPowerUp = false;
	if (powerUpFlag > 0)
	{
		m_hasPowerUp = true;
	}
	m_powerUpFlag = powerUpFlag;
}

void BreakableBlock::onDestroy()
{
    m_isDestroyed = true;
}

bool BreakableBlock::isDestroyed()
{
    return m_isDestroyed;
}

bool BreakableBlock::hasPowerUp()
{
    return m_hasPowerUp;
}

float BreakableBlock::getX()
{
    return getPosition().getX();
}

float BreakableBlock::getY()
{
    return getPosition().getY();
}

int BreakableBlock::getPowerUpFlag()
{
	return m_powerUpFlag;
}