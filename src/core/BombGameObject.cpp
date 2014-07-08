//
//  BombGameObject.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 4/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "BombGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "PlayerDynamicGameObject.h"
#include "Explosion.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "OverlapTester.h"
#include "Fire.h"

BombGameObject::BombGameObject(PlayerDynamicGameObject *bombOwner, short power, int gridX, int gridY, float width, float height) : DynamicGridGameObject(gridX, gridY, width, height, 0)
{
    m_bombOwner = bombOwner;
    m_fStateTime = 0;
    m_fSizeScalar = 0.78f + 0.1f * power;
    m_sPower = power;
    m_isExploding = false;
    m_isDestroyed = false;
	m_isPushed = false;
	m_fPushSpeed = 0.2;
	m_isPickedUp = false;
	m_isRebounding = false;
	m_acceleration->set(0,0);
	m_velocity->set(0, 0);

	m_gridBounds = std::unique_ptr<Rectangle>(new Rectangle(getPosition().getX() - GRID_CELL_WIDTH * 3 / 10, getPosition().getY() - GRID_CELL_HEIGHT * 3 / 10, GRID_CELL_WIDTH * 3 / 5, GRID_CELL_HEIGHT * 3 / 5));

    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_bombOwner->onBombDropped(this);
}

void BombGameObject::update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    m_fStateTime += deltaTime;
    
    if(!m_isExploding)
    {
        m_fWidth += deltaTime * m_fSizeScalar;
        m_fHeight = m_fWidth;
        if (m_fWidth > GRID_CELL_WIDTH * 10 / 5 || m_fWidth < GRID_CELL_WIDTH * 6 / 5)
        {
            m_fSizeScalar = -m_fSizeScalar;
        }
        
        if(m_isPushed)
        {
            switch(m_iPushedDirection)
            {
                case DIRECTION_UP :
                    if(!willHitBreakableBlock(breakableBlocks) && !willHitInsideBlock(insideBlocks) && !willTravelOffGameField(mapBorders))
                    {
						if (!m_isRebounding)
						{
							m_position->add(0, m_acceleration->getY());
							
						}
						else if (m_isRebounding)
						{
							m_position->sub(0, m_acceleration->getY());
						}
						m_fPushSpeed -= FRICTION_FACTOR;
                    }
                    else
                    {
						m_isRebounding = true;
                        m_isPushed = false;
                    }
                    break;
                case DIRECTION_DOWN :
                    if(!willHitBreakableBlock(breakableBlocks) && !willHitInsideBlock(insideBlocks) && !willTravelOffGameField(mapBorders))
                    {
						m_position->sub(0, m_acceleration->getY());
                        m_fPushSpeed -= FRICTION_FACTOR;
                    }
                    else
                    {
                        for(float i = ((GRID_CELL_HEIGHT/2.0f) + 0.2f); i > 0; i-=0.000005f)
                        {
                            m_position->add(0, 0.000005f);
                        }
                        m_isPushed = false;
                    }
                    break;
                case DIRECTION_RIGHT :
                    if(!willHitBreakableBlock(breakableBlocks) && !willHitInsideBlock(insideBlocks) && !willTravelOffGameField(mapBorders))
                    {
						m_position->add(m_acceleration->getX(), 0);
                        m_fPushSpeed -= FRICTION_FACTOR;
                    }
                    else
                    {
                        for(float i = ((GRID_CELL_WIDTH/1.25f) - 0.2f); i > 0; i-=0.000005f)
                        {
                            m_position->sub(0.000005f, 0);
                        }
                        m_isPushed = false;
                    }
                    break;
                case DIRECTION_LEFT :
                    if(!willHitBreakableBlock(breakableBlocks) && !willHitInsideBlock(insideBlocks) && !willTravelOffGameField(mapBorders))
                    {
						m_position->sub(m_acceleration->getX(), 0);
                        m_fPushSpeed -= FRICTION_FACTOR;
                    }
                    else
                    {
                        for(float i = ((GRID_CELL_WIDTH/1.25f) + 0.2f); i > 0; i-=0.000005f)
                        {
                            m_position->add(0.000005f, 0);
                        }
                        m_isPushed = false;
                    }
                    break;
            }
            
            updateBounds();
            updateGrid();
        }
        
        if (m_fStateTime > 3)
        {
            m_fWidth = GRID_CELL_WIDTH * 8 / 5;
            m_fHeight = GRID_CELL_HEIGHT * 8 / 5;
            m_fStateTime = 0;
            m_isExploding = true;
            
            m_bombOwner->onBombExploded();
        }
    }
    else if(m_isExploding && !m_isDestroyed)
    {
        if (m_fStateTime > 0.2f)
        {
            m_isDestroyed = true;
            explosions.push_back(std::unique_ptr<Explosion>(new Explosion(m_sPower, m_gridX, m_gridY, insideBlocks, breakableBlocks)));
        }
    }
}

float BombGameObject::getStateTime()
{
    return m_fStateTime;
}

bool BombGameObject::isExploding()
{
    return m_isExploding;
}

bool BombGameObject::isDestroyed()
{
    return m_isDestroyed;
}

void BombGameObject::pushed(int direction)
{
	m_isPushed = true;
	m_iPushedDirection = direction;
	m_acceleration->set(m_fPushSpeed, m_fPushSpeed);

	// Allows a bomb to be pushed more than once
	m_fPushSpeed = 0.2f;
    
    m_bombOwner->onBombPushed(this);
}

void BombGameObject::onPickedUp()
{
	m_isPickedUp = true;
}

short BombGameObject::getPower()
{
    return m_sPower;
}

void BombGameObject::updateBounds()
{
	GameObject::updateBounds();

	Vector2D &lowerLeft = m_gridBounds->getLowerLeft();
	lowerLeft.set(m_position->getX() - m_gridBounds->getWidth() / 2, m_position->getY() - m_gridBounds->getHeight() / 2);
}

Rectangle & BombGameObject::getBoundsForGridLogic()
{
	return *m_gridBounds;
}

#pragma mark <Private>

bool BombGameObject::willHitBreakableBlock(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
	for(std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
	{
		if (OverlapTester::isPointInRectangle(*m_position, (*itr)->getBounds()))
		{
			return true;
		}
	}

	return false;
}

bool BombGameObject::willHitInsideBlock(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks)
{
	for(std::vector<std::unique_ptr<InsideBlock>>::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
	{
		if (OverlapTester::isPointInRectangle(*m_position, (*itr)->getBounds()))
		{
			return true;
		}
	}

	return false;
}

bool BombGameObject::willTravelOffGameField(std::vector<std::unique_ptr<MapBorder >> &mapBorders)
{
	for(std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBorders.begin(); itr != mapBorders.end(); itr++)
	{
		if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
		{
			return true;
		}
	}

	return false;
}