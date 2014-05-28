//
//  PlayerDynamicGameObject.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 3/6/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "OverlapTester.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "ResourceConstants.h"
#include "GameListener.h"

PlayerDynamicGameObject::PlayerDynamicGameObject(short playerIndex, float x, float y, GameListener *gameListener, int direction, bool isBot, float width, float height) : DynamicGameObject(x, y, width, height, 0)
{
    m_bounds->getLowerLeft().set(x - width / 4, y - height / 2);
    m_bounds->setWidth(width / 2);
    m_bounds->setHeight(height * 7 / 16);

    m_fStateTime = 0;
    m_fSpeed = 3;
    m_firePower = 1;
    m_iDirection = direction;
    m_isMoving = false;
    m_hasActivePowerUp = false;

    m_iMaxBombCount = 1;
    m_iCurrentBombCount = 0;

    m_sPlayerIndex = playerIndex;
    m_isBot = isBot;

    m_gameListener = gameListener;

    m_playerState = ALIVE;
}

void PlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps)
{
    m_fStateTime += deltaTime;

    if (m_playerState == ALIVE)
    {
        float deltaX = m_velocity->getX() * deltaTime;
        float deltaY = m_velocity->getY() * deltaTime;

        m_position->add(deltaX, deltaY);
        m_bounds->getLowerLeft().set(getPosition().getX() - getWidth() / 4, getPosition().getY() - getHeight() / 2);

        bool isCollision = false;

        if (m_position->getX() < PLAYER_STARTING_X_LEFT || m_position->getX() > PLAYER_STARTING_X_RIGHT || m_position->getY() < PLAYER_STARTING_Y_BOTTOM || m_position->getY() > PLAYER_STARTING_Y_TOP)
        {
            isCollision = true;
        }
        else
        {
            for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
            {
                if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
                {
                    isCollision = true;
                    break;
                }
            }

            for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
            {
                if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
                {
                    isCollision = true;
                    break;
                }
            }

            for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = powerUps.begin(); itr != powerUps.end(); itr++)
            {
                if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
                {
                    int type = (*itr)->getPowerUpFlag();
                    switch (type)
                    {
                        case 1:
                            (*itr)->onPickedUp();
                            ++m_iMaxBombCount;
                            break;
                        case 2:
                            (*itr)->onPickedUp();
                            ++m_firePower;
                            break;
                        case 3:
                            (*itr)->onPickedUp();
                            ++m_fSpeed;
                            break;
                        case 4:
                            (*itr)->onPickedUp();
                            m_hasActivePowerUp = true;
                            m_activePowerUpIndex = type;
                            break;
                        default:
                            (*itr)->onPickedUp();
                            break;
                    }
                    break;
                }
            }
        }

        if (isCollision)
        {
            m_position->sub(deltaX, deltaY);
            m_bounds->getLowerLeft().set(getPosition().getX() - getWidth() / 4, getPosition().getY() - getHeight() / 2);
        }
    }
    else if (m_playerState == DYING)
    {
        if (m_fStateTime > 0.98f)
        {
            m_playerState = DEAD;
        }
    }
}

float PlayerDynamicGameObject::getStateTime()
{
    return m_fStateTime;
}

int PlayerDynamicGameObject::getDirection()
{
    return m_iDirection;
}

void PlayerDynamicGameObject::setDirection(int direction)
{
    if (direction >= 0)
    {
        m_iDirection = direction;
    }
}

void PlayerDynamicGameObject::moveInDirection(int direction)
{
    if (direction >= 0)
    {
        m_iDirection = direction;

        m_isMoving = true;

        switch (m_iDirection)
        {
            case DIRECTION_RIGHT:
                m_velocity->set(m_fSpeed, 0);
                break;
            case DIRECTION_UP:
                m_velocity->set(0, m_fSpeed);
                break;
            case DIRECTION_LEFT:
                m_velocity->set(-m_fSpeed, 0);
                break;
            case DIRECTION_DOWN:
            default:
                m_velocity->set(0, -m_fSpeed);
                break;
        }
    }
    else
    {
        m_velocity->set(0, 0);
        m_isMoving = false;
    }
}

bool PlayerDynamicGameObject::isMoving()
{
    return m_isMoving;
}

void PlayerDynamicGameObject::onBombDropped()
{
    m_iCurrentBombCount++;

    m_gameListener->playSound(SOUND_PLANT_BOMB);
}

void PlayerDynamicGameObject::onBombExploded()
{
    m_iCurrentBombCount--;

    m_gameListener->playSound(SOUND_EXPLOSION);
}

bool PlayerDynamicGameObject::isHitByExplosion(std::vector<std::unique_ptr<Explosion >> &explosions)
{
    if (m_playerState == ALIVE)
    {
        for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
            {
                return true;
            }
        }
    }

    return false;
}

void PlayerDynamicGameObject::onDeath()
{
    m_playerState = DYING;
    m_fStateTime = 0;

    m_gameListener->playSound(SOUND_DEATH);
}

bool PlayerDynamicGameObject::isAbleToDropAdditionalBomb()
{
    return m_playerState == ALIVE && m_iCurrentBombCount < m_iMaxBombCount;
}

short PlayerDynamicGameObject::getFirePower()
{
    return m_firePower;
}

Player_State PlayerDynamicGameObject::getPlayerState()
{
    return m_playerState;
}

Power_Up_Type PlayerDynamicGameObject::getActivePowerUp()
{
    return m_activePowerUp;
}

short PlayerDynamicGameObject::getPlayerIndex()
{
    return m_sPlayerIndex;
}

bool PlayerDynamicGameObject::isBot()
{
    return m_isBot;
}