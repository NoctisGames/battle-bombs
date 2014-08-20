//
//  PlayerDynamicGameObject.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 3/6/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "OverlapTester.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "ResourceConstants.h"
#include "GameListener.h"
#include "Fire.h"
#include "GameEvent.h"

#include <cstring>

PlayerDynamicGameObject::PlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction, float width, float height) : DynamicGridGameObject(gridX, gridY, width, height, 0)
{
    resetBounds(width * 5 / 32, height / 12);

    m_lastBombDropped = nullptr;
    m_fStateTime = 0;
    m_iSpeed = 3;
    m_firePower = 1;
    m_iDirection = direction;
	m_activePowerUp = NONE;

    m_iMaxBombCount = 1;
    m_iCurrentBombCount = 0;

    m_sPlayerIndex = playerIndex;

    m_gameListener = gameListener;

    m_playerState = ALIVE;
    m_playerActionState = IDLE;
    m_isBot = false;
}

void PlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    m_fStateTime += deltaTime;

    if (m_playerState == ALIVE && m_playerActionState != WINNING)
    {
        if(m_playerActionState == PLACING_BOMB || m_playerActionState == PUSHING_BOMB)
        {
            if(m_fStateTime > 0.15f)
            {
                m_playerActionState = IDLE;
                m_fStateTime = 0;
            }
        }
        
        float deltaX = m_velocity->getX() * deltaTime;
        float deltaY = m_velocity->getY() * deltaTime;

        m_position->add(deltaX, deltaY);
        updateBounds();
        
        if(m_lastBombDropped != nullptr && !OverlapTester::doRectanglesOverlap(*m_bounds, m_lastBombDropped->getBounds()))
        {
            m_lastBombDropped = nullptr;
        }
        
        if (isCollision(mapBorders, insideBlocks, breakableBlocks, players, bombs))
        {
            m_position->sub(deltaX, deltaY);
            updateBounds();
        }
        
        updateGrid();
        
        short numPlayersAlive = 0;
        for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
        {
            if((*itr)->getPlayerState() == ALIVE)
            {
                numPlayersAlive++;
            }
        }
        
        if(numPlayersAlive == 1)
        {
            m_fStateTime = 0;
            m_velocity->set(0, 0);
            m_playerActionState = WINNING;
        }
    }
    else if (m_playerState == DYING)
    {
        if (m_fStateTime > 0.6f)
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

        switch (m_iDirection)
        {
            case DIRECTION_RIGHT:
                m_velocity->set(m_iSpeed, 0);
                break;
            case DIRECTION_UP:
                m_velocity->set(0, m_iSpeed);
                break;
            case DIRECTION_LEFT:
                m_velocity->set(-m_iSpeed, 0);
                break;
            case DIRECTION_DOWN:
            default:
                m_velocity->set(0, -m_iSpeed);
                break;
        }
    }
    else
    {
        m_velocity->set(0, 0);
        m_fStateTime = 0;
    }
}

bool PlayerDynamicGameObject::isMoving()
{
    return m_velocity->getX() != 0 || m_velocity->getY() != 0;
}

void PlayerDynamicGameObject::onBombDropped(BombGameObject *bomb)
{
    m_lastBombDropped = bomb;
    
    m_iCurrentBombCount++;

    m_gameListener->playSound(SOUND_PLANT_BOMB);
    
    m_playerActionState = PLACING_BOMB;
    m_fStateTime = 0;
}

void PlayerDynamicGameObject::onBombPushed(BombGameObject *bomb)
{
    m_playerActionState = PUSHING_BOMB;
    m_fStateTime = 0;
}

void PlayerDynamicGameObject::onBombExploded()
{
    m_iCurrentBombCount--;

    m_gameListener->playSound(SOUND_EXPLOSION);
    
    if(m_iCurrentBombCount == 0)
    {
        m_lastBombDropped = nullptr;
    }
}

bool PlayerDynamicGameObject::isHitByExplosion(std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    if (m_playerState == ALIVE && m_playerActionState != WINNING)
    {
        for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
        {
            for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
            {
                if ((*itr2)->isDeadly() && OverlapTester::doRectanglesOverlap(*m_bounds, (*itr2)->getBounds()))
                {
                    return true;
                }
            }
        }
        
        for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
        {
            if ((*itr)->isExploding() && OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
            {
                return true;
            }
        }
    }

    return false;
}

void PlayerDynamicGameObject::handlePowerUps(std::vector<std::unique_ptr<PowerUp >> &powerUps)
{
    for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = powerUps.begin(); itr != powerUps.end(); itr++)
    {
        if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
        {
            int type = (*itr)->getPowerUpFlag();
            switch (type)
            {
                case 1:
                    (*itr)->onPickedUp();
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_BOMB, *this);
                    break;
                case 2:
                    (*itr)->onPickedUp();
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_FIRE, *this);
                    break;
                case 3:
                    (*itr)->onPickedUp();
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_SPEED, *this);
                    break;
                case 4:
                    (*itr)->onPickedUp();
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_PUSH, *this);
                    break;
            }
            
            break;
        }
    }
}

bool PlayerDynamicGameObject::isBombInFrontOfPlayer(BombGameObject &bomb)
{
    switch (m_iDirection)
    {
        case DIRECTION_RIGHT:
            return m_gridY == bomb.getGridY() && (m_gridX == bomb.getGridX() || m_gridX == bomb.getGridX() - 1);
        case DIRECTION_LEFT:
            return m_gridY == bomb.getGridY() && (m_gridX == bomb.getGridX() || m_gridX == bomb.getGridX() + 1);
        case DIRECTION_UP:
            return m_gridX == bomb.getGridX() && (m_gridY == bomb.getGridY() || m_gridY == bomb.getGridY() - 1);
        case DIRECTION_DOWN:
        default:
            return m_gridX == bomb.getGridX() && (m_gridY == bomb.getGridY() || m_gridY == bomb.getGridY() + 1);
    }
}

void PlayerDynamicGameObject::onDeath()
{
    m_playerState = DYING;
    m_fStateTime = 0;

    m_gameListener->playSound(SOUND_DEATH);
}

bool PlayerDynamicGameObject::isAbleToDropAdditionalBomb(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    if(m_playerState == ALIVE && m_playerActionState != WINNING && m_iCurrentBombCount < m_iMaxBombCount)
    {
        for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
        {
            if((**itr).getPlayerState() == Player_State::ALIVE && (*itr).get() != this)
            {
                if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
                {
                    return false;
                }
            }
        }
        
        for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
        {
            if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
            {
                return false;
            }
        }
        
        return true;
    }
    
    return false;
}

short PlayerDynamicGameObject::getFirePower()
{
    return m_firePower;
}

int PlayerDynamicGameObject::getMaxBombCount()
{
    return m_iMaxBombCount;
}

int PlayerDynamicGameObject::getSpeed()
{
    return m_iSpeed;
}

void PlayerDynamicGameObject::collectPowerUp(int powerUpFlag)
{
    switch (powerUpFlag)
    {
        case 1:
            m_iMaxBombCount++;
            break;
        case 2:
            m_firePower++;
            break;
        case 3:
            m_iSpeed++;
            break;
        case 4:
            m_activePowerUp = PUSH;
            break;
    }
}

void PlayerDynamicGameObject::setPlayerState(Player_State playerState)
{
    m_playerState = playerState;
}

Player_State PlayerDynamicGameObject::getPlayerState()
{
    return m_playerState;
}

Player_Action_State PlayerDynamicGameObject::getPlayerActionState()
{
    return m_playerActionState;
}

Power_Up_Type PlayerDynamicGameObject::getActivePowerUp()
{
    return m_activePowerUp;
}

short PlayerDynamicGameObject::getPlayerIndex()
{
    return m_sPlayerIndex;
}

void PlayerDynamicGameObject::resetBounds(float width, float height)
{
    m_bounds->setWidth(width);
    m_bounds->setHeight(height);
    
    updateBounds();
}

void PlayerDynamicGameObject::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(getPosition().getX() - getWidth() * 5 / 64, getPosition().getY() - getHeight() / 4);
}

Rectangle & PlayerDynamicGameObject::getBoundsForGridLogic()
{
    return *m_bounds;
}

bool PlayerDynamicGameObject::isBot()
{
    return m_isBot;
}

void PlayerDynamicGameObject::setIsBot(bool isBot)
{
    m_isBot = isBot;
}

char * PlayerDynamicGameObject::getUsername()
{
    return m_username;
}

void PlayerDynamicGameObject::setUsername(const char *username)
{
    int usernameLength = (int) strlen(username);
    
    m_username = new char[usernameLength];
    
    std::strncpy(m_username, username, usernameLength);
    m_username[usernameLength] = '\0';
}

// Private

bool PlayerDynamicGameObject::isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        bool isOnTopOfBomb = m_gridX == (*itr)->getGridX() && m_gridY == (*itr)->getGridY();
        bool ism_lastBombDropped = m_lastBombDropped != nullptr && (*itr).get() == m_lastBombDropped;
        if (!isOnTopOfBomb && !ism_lastBombDropped && OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < MapBorder >> ::iterator itr = mapBorders.begin(); itr != mapBorders.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    return false;
}