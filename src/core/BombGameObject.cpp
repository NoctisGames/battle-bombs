//
//  BombGameObject.cpp
//  battlebombs
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
	m_isPickedUp = false;
	m_isRebounding = false;

	m_gridBounds = std::unique_ptr<Rectangle>(new Rectangle(getPosition().getX() - GRID_CELL_WIDTH * 3 / 10, getPosition().getY() - GRID_CELL_HEIGHT * 3 / 10, GRID_CELL_WIDTH * 3 / 5, GRID_CELL_HEIGHT * 3 / 5));

    resetBounds(GRID_CELL_WIDTH, GRID_CELL_HEIGHT);
    
    m_bombOwner->onBombDropped(this);
}

void BombGameObject::update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
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
            m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
            
            switch(m_iPushedDirection)
            {
                case DIRECTION_RIGHT:
                    if(m_velocity->getX() < 0)
                    {
                        m_isPushed = false;
                    }
                    break;
                case DIRECTION_UP:
                    if(m_velocity->getY() < 0)
                    {
                        m_isPushed = false;
                    }
                    break;
                case DIRECTION_LEFT:
                    if(m_velocity->getX() > 0)
                    {
                        m_isPushed = false;
                    }
                    break;
                case DIRECTION_DOWN:
                    if(m_velocity->getY() > 0)
                    {
                        m_isPushed = false;
                    }
                    break;
            }
            
            m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
            
            if (isCollision(mapBorders, insideBlocks, breakableBlocks, players, bombs))
            {
                m_isPushed = false;
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
            explosions.push_back(std::unique_ptr<Explosion>(new Explosion(m_sPower, m_gridX, m_gridY, insideBlocks, breakableBlocks, players)));
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
    
    switch (direction)
    {
        case DIRECTION_RIGHT:
            m_velocity->set(9, 0);
            m_acceleration->set(-FRICTION_FACTOR, 0);
            break;
        case DIRECTION_UP:
            m_velocity->set(0, 9);
            m_acceleration->set(0, -FRICTION_FACTOR);
            break;
        case DIRECTION_LEFT:
            m_velocity->set(-9, 0);
            m_acceleration->set(FRICTION_FACTOR, 0);
            break;
        case DIRECTION_DOWN:
            m_velocity->set(0, -9);
            m_acceleration->set(0, FRICTION_FACTOR);
            break;
        default:
            break;
    }
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

bool BombGameObject::isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if ((*itr).get() != m_bombOwner && OverlapTester::doRectanglesOverlap(*m_gridBounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        bool isOnTopOfBomb = m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY();
        if (!isOnTopOfBomb && OverlapTester::doRectanglesOverlap(*m_gridBounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < MapBorder >> ::iterator itr = mapBorders.begin(); itr != mapBorders.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_gridBounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_gridBounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_gridBounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    return false;
}
