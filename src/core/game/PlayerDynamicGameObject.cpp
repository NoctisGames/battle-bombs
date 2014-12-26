//
//  PlayerDynamicGameObject.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 3/6/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "MapBorder.h"
#include "SpaceTile.h"
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
#include "PlayerForceFieldState.h"
#include "Crater.h"
#include "IcePatch.h"

#include <cstring>

PlayerDynamicGameObject::PlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction, float width, float height) : DynamicGridGameObject(gridX, gridY, width, height, 0)
{
    m_position->add(0, GRID_CELL_HEIGHT / 4);
    resetBounds(width * 5 / 32, height / 12);

	m_username = std::unique_ptr<char>(new char[1]);

    m_sPlayerIndex = playerIndex;
    m_iDirection = direction;
    m_gameListener = gameListener;
    m_isClientPlayer = false;
    m_isBot = false;
    m_fDisplayingPointerStateTime = 0;
    
    reset();
}

void PlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<SpaceTile>> &spaceTiles, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<Crater >> &craters, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    m_fStateTime += deltaTime;
    
    if(m_iPlayerForceFieldState != PLAYER_FORCE_FIELD_STATE_OFF)
    {
        m_fPlayerForceFieldStateTime += deltaTime;
        
        if(m_iPlayerForceFieldState == PLAYER_FORCE_FIELD_STATE_TURNING_ON)
        {
            if(m_fPlayerForceFieldStateTime > 0.5f)
            {
                m_iPlayerForceFieldState = PLAYER_FORCE_FIELD_STATE_ON;
            }
        }
        else if(m_iPlayerForceFieldState == PLAYER_FORCE_FIELD_STATE_BREAKING_DOWN)
        {
            if(m_fPlayerForceFieldStateTime > 0.7f)
            {
                m_iPlayerForceFieldState = PLAYER_FORCE_FIELD_STATE_OFF;
            }
        }
    }
    
    if(m_isDisplayingPointer)
    {
        m_fDisplayingPointerStateTime += deltaTime;
        if(m_fDisplayingPointerStateTime > 1)
        {
            m_isDisplayingPointer = false;
        }
    }

    if (m_playerState == ALIVE)
    {
        if(m_playerActionState == PLACING_BOMB || m_playerActionState == PUSHING_BOMB)
        {
            if(m_fStateTime > 0.15f)
            {
                m_playerActionState = IDLE;
                m_fStateTime = 0;
            }
        }
        else if(m_playerActionState == RAISING_SHIELD)
        {
            if(m_fStateTime > 0.30f)
            {
                m_playerActionState = SHIELD_RAISED;
                m_fStateTime = 0;
            }
        }
        
        float deltaX = m_velocity->getX() * deltaTime;
        float deltaY = m_velocity->getY() * deltaTime;
        
        m_position->add(deltaX, deltaY);
        updateBounds();
        updateGrid();
        
        if(m_lastBombDropped != nullptr && !OverlapTester::doRectanglesOverlap(*m_bounds, m_lastBombDropped->getBounds()))
        {
            m_lastBombDropped = nullptr;
        }
        
        if (isCollision(mapBorders, spaceTiles, insideBlocks, breakableBlocks, craters, players, bombs))
        {
            m_position->sub(deltaX, deltaY);
            updateBounds();
        }
        
        updateGrid();
    }
    else if (m_playerState == DYING)
    {
        if (m_fStateTime > 0.6f)
        {
            m_playerState = DEAD;
        }
    }
    else if(m_playerState == ABOUT_TO_FALL)
    {
        m_fAngle += 180 * deltaTime;
    }
    else if(m_playerState == FALLING)
    {
        m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
        m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
        
        m_fAngle += 180 * deltaTime;
        
        if(m_position->getY() < 0)
        {
            m_playerState = DEAD;
        }
    }
    else if(m_playerState == FREEZING)
    {
        if (m_fStateTime > 0.6f)
        {
            m_playerState = FROZEN;
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

float PlayerDynamicGameObject::getPlayerForceFieldStateTime()
{
    return m_fPlayerForceFieldStateTime;
}

int PlayerDynamicGameObject::getPlayerForceFieldState()
{
    return m_iPlayerForceFieldState;
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
    if (direction >= DIRECTION_RIGHT)
    {
        if(m_playerActionState == RAISING_SHIELD || m_playerActionState == SHIELD_RAISED)
        {
            m_gameListener->addLocalEventForPlayer(PLAYER_LOWER_SHIELD, *this);
        }
        
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

void PlayerDynamicGameObject::raiseShield()
{
    m_playerActionState = RAISING_SHIELD;
    m_fStateTime = 0;
    
    m_gameListener->playSound(SOUND_RAISE_SHIELD);
}

void PlayerDynamicGameObject::lowerShield()
{
    m_playerActionState = IDLE;
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
    if (m_playerState == ALIVE)
    {
        bool isHitByExplosion = false;
        
        for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
        {
            for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
            {
                if ((*itr2)->isDeadly() && m_gridX == (*itr2)->getGridX() && m_gridY == (*itr2)->getGridY())
                {
                    isHitByExplosion = true;
                    
                    if(m_playerActionState == SHIELD_RAISED)
                    {
                        if((*itr2)->getDirection() == DIRECTION_RIGHT && m_iDirection == DIRECTION_LEFT)
                        {
                            isHitByExplosion = false;
                        }
                        else if((*itr2)->getDirection() == DIRECTION_UP && m_iDirection == DIRECTION_DOWN)
                        {
                            isHitByExplosion = false;
                        }
                        else if((*itr2)->getDirection() == DIRECTION_LEFT && m_iDirection == DIRECTION_RIGHT)
                        {
                            isHitByExplosion = false;
                        }
                        else if((*itr2)->getDirection() == DIRECTION_DOWN && m_iDirection == DIRECTION_UP)
                        {
                            isHitByExplosion = false;
                        }
                    }
                }
            }
        }
        
        for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
        {
            if ((*itr)->isExploding() && OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
            {
                isHitByExplosion = true;
            }
        }
        
        if(isHitByExplosion && m_iPlayerForceFieldState != PLAYER_FORCE_FIELD_STATE_OFF)
        {
            m_gameListener->addLocalEventForPlayer(PLAYER_FORCE_FIELD_HIT, *this);
            isHitByExplosion = false;
        }
        
        return isHitByExplosion;
    }

    return false;
}

bool PlayerDynamicGameObject::isTrappedOnFallingSpaceTile(std::vector<std::unique_ptr<SpaceTile>> &spaceTiles)
{
    if(m_playerState == ALIVE)
    {
        for (std::vector <std::unique_ptr<SpaceTile>> ::iterator itr = spaceTiles.begin(); itr != spaceTiles.end(); itr++)
        {
            if((*itr)->getSpaceTileState() == ST_FALLING)
            {
                if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool PlayerDynamicGameObject::isFallingDueToSpaceTile(std::vector<std::unique_ptr<SpaceTile>> &spaceTiles)
{
    if(m_playerState == ALIVE || m_playerState == ABOUT_TO_FALL)
    {
        for (std::vector <std::unique_ptr<SpaceTile>> ::iterator itr = spaceTiles.begin(); itr != spaceTiles.end(); itr++)
        {
            if((*itr)->getSpaceTileState() == ST_FALLING && (*itr)->shouldPlayerStartFalling())
            {
                if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool PlayerDynamicGameObject::isHitByFireBall(std::vector<std::unique_ptr<Crater >> &craters)
{
    if(m_playerState == ALIVE)
    {
        for (std::vector <std::unique_ptr<Crater>> ::iterator itr = craters.begin(); itr != craters.end(); itr++)
        {
            if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool PlayerDynamicGameObject::isHitByIce(std::vector<std::unique_ptr<IcePatch >> &icePatches)
{
    if(m_playerState == ALIVE)
    {
        for (std::vector <std::unique_ptr<IcePatch>> ::iterator itr = icePatches.begin(); itr != icePatches.end(); itr++)
        {
            if(OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
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
            (*itr)->onPickedUp();
            
            int type = (*itr)->getType();
            switch (type)
            {
                case POWER_UP_TYPE_BOMB:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_BOMB, *this);
                    break;
                case POWER_UP_TYPE_FIRE:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_FIRE, *this);
                    break;
                case POWER_UP_TYPE_FORCE_FIELD:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_FORCE_FIELD, *this);
                    break;
                case POWER_UP_TYPE_SPEED:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_SPEED, *this);
                    break;
                case POWER_UP_TYPE_PUSH:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_PUSH, *this);
                    break;
                case POWER_UP_TYPE_SHIELD:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_SHIELD, *this);
                    break;
                case POWER_UP_TYPE_MEGA_FIRE:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_MEGA_FIRE, *this);
                    break;
                case POWER_UP_TYPE_REMOTE_BOMB:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_REMOTE_BOMB, *this);
                    break;
                case POWER_UP_TYPE_LAND_MINE:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_LAND_MINE, *this);
                    break;
                case POWER_UP_TYPE_CURSE:
                    m_gameListener->addLocalEventForPlayer(PLAYER_PU_CURSE, *this);
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

void PlayerDynamicGameObject::onForceFieldHit()
{
    if(m_iPlayerForceFieldState != PLAYER_FORCE_FIELD_STATE_BREAKING_DOWN)
    {
        m_gameListener->playSound(SOUND_FORCE_FIELD_DOWN);
    }
    
    setPlayerForceFieldState(PLAYER_FORCE_FIELD_STATE_BREAKING_DOWN);
}

void PlayerDynamicGameObject::onTrappedOnFallingSpaceTile(std::vector<std::unique_ptr<SpaceTile>> &spaceTiles)
{
    reset();
    
    m_playerState = ABOUT_TO_FALL;
    
    for (std::vector <std::unique_ptr<SpaceTile>> ::iterator itr = spaceTiles.begin(); itr != spaceTiles.end(); itr++)
    {
        if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_gridY)
        {
            (*itr)->setFallingPlayer(this);
            return;
        }
    }
}

void PlayerDynamicGameObject::onFall()
{
    reset();
    
    m_playerState = FALLING;
    
    m_velocity->set(0, 0);
    m_acceleration->set(0, -12);
    
    m_gameListener->playSound(SOUND_DEATH);
}

void PlayerDynamicGameObject::onFreeze()
{
    reset();
    
    m_playerState = FREEZING;
    
    m_gameListener->playSound(SOUND_DEATH);
}

void PlayerDynamicGameObject::onDeath()
{
    reset();
    
    m_playerState = DYING;

    m_gameListener->playSound(SOUND_DEATH);
}

void PlayerDynamicGameObject::onWin()
{
    m_fStateTime = 0;
    m_velocity->set(0, 0);
    m_playerActionState = WINNING;
}

bool PlayerDynamicGameObject::isAbleToDropAdditionalBomb(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    if(m_playerState == ALIVE && m_iCurrentBombCount < m_iMaxBombCount)
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
        case POWER_UP_TYPE_BOMB:
            if(m_iMaxBombCount < 10)
            {
                m_iMaxBombCount++;
            }
            break;
        case POWER_UP_TYPE_FIRE:
            if(m_firePower < 10)
            {
                m_firePower++;
            }
            break;
        case POWER_UP_TYPE_FORCE_FIELD:
            setPlayerForceFieldState(PLAYER_FORCE_FIELD_STATE_TURNING_ON);
            break;
        case POWER_UP_TYPE_SPEED:
            if(m_iSpeed < 12)
            {
                m_iSpeed++;
            }
            break;
        case POWER_UP_TYPE_PUSH:
            m_activePowerUp = POWER_UP_TYPE_PUSH;
            break;
        case POWER_UP_TYPE_SHIELD:
            m_activePowerUp = POWER_UP_TYPE_SHIELD;
            break;
        case POWER_UP_TYPE_MEGA_FIRE:
            m_firePower = 10;
            break;
        case POWER_UP_TYPE_REMOTE_BOMB:
            // TODO, change bomb type
            break;
        case POWER_UP_TYPE_LAND_MINE:
            m_activePowerUp = POWER_UP_TYPE_LAND_MINE;
            break;
        case POWER_UP_TYPE_CURSE:
            // TODO, begin 8 frame curse animation for say... 3.2 seconds (4 loops)
            break;
    }
    
    if(m_isClientPlayer)
    {
        switch (powerUpFlag)
        {
            case POWER_UP_TYPE_BOMB:
                m_gameListener->playSound(SOUND_PU_BOMB);
                break;
            case POWER_UP_TYPE_FIRE:
                m_gameListener->playSound(SOUND_PU_FIRE);
                break;
            case POWER_UP_TYPE_FORCE_FIELD:
                m_gameListener->playSound(SOUND_PU_FORCE_FIELD);
                break;
            case POWER_UP_TYPE_SPEED:
                m_gameListener->playSound(SOUND_PU_SPEED);
                break;
            case POWER_UP_TYPE_PUSH:
                m_gameListener->playSound(SOUND_PU_PUSH);
                break;
            case POWER_UP_TYPE_SHIELD:
                m_gameListener->playSound(SOUND_PU_SHIELD);
                break;
            case POWER_UP_TYPE_MEGA_FIRE:
                m_gameListener->playSound(SOUND_PU_FIRE);
                break;
            case POWER_UP_TYPE_REMOTE_BOMB:
                m_gameListener->playSound(SOUND_PU_FIRE);
                break;
            case POWER_UP_TYPE_LAND_MINE:
                m_gameListener->playSound(SOUND_PU_FIRE);
                break;
            case POWER_UP_TYPE_CURSE:
                m_gameListener->playSound(SOUND_PU_FIRE);
                break;
        }
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

int PlayerDynamicGameObject::getActivePowerUp()
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
    return m_username.get();
}

void PlayerDynamicGameObject::setUsername(const char *username)
{
    int usernameLength = (int) strlen(username);

	m_username.release();
    
    m_username = std::unique_ptr<char>(new char[usernameLength + 1]);
    
    std::strncpy(m_username.get(), username, usernameLength);
	m_username.get()[usernameLength] = '\0';
}

void PlayerDynamicGameObject::setClientPlayer(bool isClientPlayer)
{
    m_isClientPlayer = isClientPlayer;
}

void PlayerDynamicGameObject::setIsDisplayingName(bool isDisplayingName)
{
    m_isDisplayingName = isDisplayingName;
}

bool PlayerDynamicGameObject::isDisplayingName()
{
    return m_isDisplayingName;
}

void PlayerDynamicGameObject::setIsDisplayingPointer(bool isDisplayingPointer)
{
    m_isDisplayingPointer = isDisplayingPointer;
    m_fDisplayingPointerStateTime = 0;
}

bool PlayerDynamicGameObject::isDisplayingPointer()
{
    return m_isDisplayingPointer;
}

void PlayerDynamicGameObject::reset()
{
    m_lastBombDropped = nullptr;
    m_fStateTime = 0;
    m_iSpeed = 3;
    m_firePower = 1;
    m_activePowerUp = POWER_UP_TYPE_NONE;
    
    m_iMaxBombCount = 1;
    m_iCurrentBombCount = 0;
    setPlayerForceFieldState(PLAYER_FORCE_FIELD_STATE_OFF);
    
    m_playerState = ALIVE;
    m_playerActionState = IDLE;
    
    m_isDisplayingName = false;
    m_isDisplayingPointer = false;
}

void PlayerDynamicGameObject::handleBombErasure(BombGameObject *bomb)
{
    if(bomb == m_lastBombDropped)
    {
        m_lastBombDropped = nullptr;
    }
}

// Private

bool PlayerDynamicGameObject::isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<SpaceTile>> &spaceTiles, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<Crater >> &craters, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
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
    
    for (std::vector < std::unique_ptr < Crater >> ::iterator itr = craters.begin(); itr != craters.end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr)->getBounds()))
        {
            return true;
        }
    }
    
    return false;
}

void PlayerDynamicGameObject::setPlayerForceFieldState(int playerForceFieldState)
{
    m_iPlayerForceFieldState = playerForceFieldState;
    m_fPlayerForceFieldStateTime = 0;
}