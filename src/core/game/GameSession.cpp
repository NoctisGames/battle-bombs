//
//  GameSession.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 3/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameSession.h"
#include "GameListener.h"
#include "GameState.h"
#include "Map.h"
#include "MapFactory.h"
#include "Vector2D.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "OverlapTester.h"
#include "MapBorder.h"
#include "SpaceTile.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"
#include "Fire.h"
#include "PathFinder.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"

#include <iostream>

#ifdef TECHNE_GAMES_DIRECT_3D
#define strdup _strdup
#endif

GameSession::GameSession()
{
    init();
}

void GameSession::init()
{
    m_gameListener.release();
    m_gameListener = std::unique_ptr<GameListener>(new GameListener());
    
    m_iNumBreakableBlocksAtSpawnTime = 0;
    m_isSuddenDeath = false;
    
    m_breakableBlocks.clear();
    m_players.clear();
    m_bombs.clear();
    m_explosions.clear();
    m_powerUps.clear();
    m_spaceTiles.clear();
    m_craters.clear();
    m_fireBalls.clear();
    m_iceBalls.clear();
    m_icePatches.clear();
    m_mapBorders.clear();
    m_insideBlocks.clear();
    m_spaceTiles.clear();
}

void GameSession::handleServerUpdate(const char *message)
{
    m_gameListener->addServerMessage(message);
}

void GameSession::suddenDeath()
{
    m_isSuddenDeath = true;
    
    m_map->suddenDeath(this);
}

#pragma mark <For ServerGameSession to override>

void GameSession::onBreakableBlockDestroyed(BreakableBlock &breakableBlock)
{
    // Empty
}

void GameSession::onPowerUpPickedUp(PowerUp &powerUp)
{
    // Empty
}

void GameSession::clientUpdate(rapidjson::Document &d, bool isBeginGame)
{
    static const char *playerIndex0Key = "playerIndex0";
    static const char *playerIndex1Key = "playerIndex1";
    static const char *playerIndex2Key = "playerIndex2";
    static const char *playerIndex3Key = "playerIndex3";
    static const char *playerIndex4Key = "playerIndex4";
    static const char *playerIndex5Key = "playerIndex5";
    static const char *playerIndex6Key = "playerIndex6";
    static const char *playerIndex7Key = "playerIndex7";
    
    static const char *playerIndex0IsBotKey = "playerIndex0IsBot";
    static const char *playerIndex1IsBotKey = "playerIndex1IsBot";
    static const char *playerIndex2IsBotKey = "playerIndex2IsBot";
    static const char *playerIndex3IsBotKey = "playerIndex3IsBot";
    static const char *playerIndex4IsBotKey = "playerIndex4IsBot";
    static const char *playerIndex5IsBotKey = "playerIndex5IsBot";
    static const char *playerIndex6IsBotKey = "playerIndex6IsBot";
    static const char *playerIndex7IsBotKey = "playerIndex7IsBot";
    
    static const char *playerIndex0XKey = "playerIndex0X";
    static const char *playerIndex1XKey = "playerIndex1X";
    static const char *playerIndex2XKey = "playerIndex2X";
    static const char *playerIndex3XKey = "playerIndex3X";
    static const char *playerIndex4XKey = "playerIndex4X";
    static const char *playerIndex5XKey = "playerIndex5X";
    static const char *playerIndex6XKey = "playerIndex6X";
    static const char *playerIndex7XKey = "playerIndex7X";
    
    static const char *playerIndex0YKey = "playerIndex0Y";
    static const char *playerIndex1YKey = "playerIndex1Y";
    static const char *playerIndex2YKey = "playerIndex2Y";
    static const char *playerIndex3YKey = "playerIndex3Y";
    static const char *playerIndex4YKey = "playerIndex4Y";
    static const char *playerIndex5YKey = "playerIndex5Y";
    static const char *playerIndex6YKey = "playerIndex6Y";
    static const char *playerIndex7YKey = "playerIndex7Y";
    
    static const char *playerIndex0DirectionKey = "playerIndex0Direction";
    static const char *playerIndex1DirectionKey = "playerIndex1Direction";
    static const char *playerIndex2DirectionKey = "playerIndex2Direction";
    static const char *playerIndex3DirectionKey = "playerIndex3Direction";
    static const char *playerIndex4DirectionKey = "playerIndex4Direction";
    static const char *playerIndex5DirectionKey = "playerIndex5Direction";
    static const char *playerIndex6DirectionKey = "playerIndex6Direction";
    static const char *playerIndex7DirectionKey = "playerIndex7Direction";
    
    static const char *playerIndex0AliveKey = "playerIndex0Alive";
    static const char *playerIndex1AliveKey = "playerIndex1Alive";
    static const char *playerIndex2AliveKey = "playerIndex2Alive";
    static const char *playerIndex3AliveKey = "playerIndex3Alive";
    static const char *playerIndex4AliveKey = "playerIndex4Alive";
    static const char *playerIndex5AliveKey = "playerIndex5Alive";
    static const char *playerIndex6AliveKey = "playerIndex6Alive";
    static const char *playerIndex7AliveKey = "playerIndex7Alive";
    
    clientUpdateForPlayerIndex(d, playerIndex0Key, playerIndex0IsBotKey, playerIndex0XKey, playerIndex0YKey, playerIndex0DirectionKey, playerIndex0AliveKey, 0, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex1Key, playerIndex1IsBotKey, playerIndex1XKey, playerIndex1YKey, playerIndex1DirectionKey, playerIndex1AliveKey, 1, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex2Key, playerIndex2IsBotKey, playerIndex2XKey, playerIndex2YKey, playerIndex2DirectionKey, playerIndex2AliveKey, 2, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex3Key, playerIndex3IsBotKey, playerIndex3XKey, playerIndex3YKey, playerIndex3DirectionKey, playerIndex3AliveKey, 3, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex4Key, playerIndex4IsBotKey, playerIndex4XKey, playerIndex4YKey, playerIndex4DirectionKey, playerIndex4AliveKey, 4, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex5Key, playerIndex5IsBotKey, playerIndex5XKey, playerIndex5YKey, playerIndex5DirectionKey, playerIndex5AliveKey, 5, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex6Key, playerIndex6IsBotKey, playerIndex6XKey, playerIndex6YKey, playerIndex6DirectionKey, playerIndex6AliveKey, 6, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex7Key, playerIndex7IsBotKey, playerIndex7XKey, playerIndex7YKey, playerIndex7DirectionKey, playerIndex7AliveKey, 7, isBeginGame);
    
    handleClientEventsArrayInDocument(d);
}

void GameSession::hardUpdate(rapidjson::Document &d)
{
    static const char *numDeletedBreakableBlocksKey = "numDeletedBreakableBlocks";
    
    if(d.HasMember(numDeletedBreakableBlocksKey))
    {
        static const char *deletedBreakableBlockXValuesKey = "deletedBreakableBlockXValues";
        static const char *deletedBreakableBlockYValuesKey = "deletedBreakableBlockYValues";
        
        std::vector<int> deletedBreakableBlockXValues;
        std::vector<int> deletedBreakableBlockYValues;
        
        handleIntArrayInDocument(d, deletedBreakableBlockXValuesKey, deletedBreakableBlockXValues, -1);
        handleIntArrayInDocument(d, deletedBreakableBlockYValuesKey, deletedBreakableBlockYValues, -1);
        
        int numDeletedBreakableBlocks = d[numDeletedBreakableBlocksKey].GetInt();
        for(int i = 0; i < numDeletedBreakableBlocks; i++)
        {
            for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = m_breakableBlocks.begin(); itr != m_breakableBlocks.end(); )
            {
                bool isMatch = (*itr)->getGridX() == deletedBreakableBlockXValues.at(i) && (*itr)->getGridY() == deletedBreakableBlockYValues.at(i);
                if (isMatch && (**itr).getBreakableBlockState() == BB_NORMAL)
                {
                    if ((**itr).hasPowerUp())
                    {
                        m_powerUps.push_back(std::unique_ptr<PowerUp>(new PowerUp((**itr).getGridX(), (**itr).getGridY(), (**itr).getPowerUpFlag())));
                    }
                    
                    PathFinder::getInstance().freeGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
                    
                    itr = m_breakableBlocks.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
        }
    }
    
    static const char *numDeletedPowerUpsKey = "numDeletedPowerUps";
    
    if(d.HasMember(numDeletedPowerUpsKey))
    {
        static const char *deletedPowerUpsXValuesKey = "deletedPowerUpsXValues";
        static const char *deletedPowerUpsYValuesKey = "deletedPowerUpsYValues";
        
        std::vector<int> deletedPowerUpsXValues;
        std::vector<int> deletedPowerUpsYValues;
        
        handleIntArrayInDocument(d, deletedPowerUpsXValuesKey, deletedPowerUpsXValues, -1);
        handleIntArrayInDocument(d, deletedPowerUpsYValuesKey, deletedPowerUpsYValues, -1);
        
        int numDeletedPowerUps = d[numDeletedPowerUpsKey].GetInt();
        for(int i = 0; i < numDeletedPowerUps; i++)
        {
            for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
            {
                bool isMatch = (*itr)->getGridX() == deletedPowerUpsXValues.at(i) && (*itr)->getGridY() == deletedPowerUpsYValues.at(i);
                
                if (isMatch)
                {
                    itr = m_powerUps.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
        }
    }
}

int GameSession::getNumPlayers()
{
    return (int) m_players.size();
}

bool GameSession::isPlayerBotAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->isBot();
}

float GameSession::getPlayerXAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPosition().getX();
}

float GameSession::getPlayerYAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPosition().getY();
}

int GameSession::getPlayerDirectionAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getDirection();
}

bool GameSession::isPlayerAliveAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPlayerState() == Player_State::ALIVE;
}

int GameSession::popOldestEventId()
{
    return m_gameListener->popOldestEventId();
}

void GameSession::readCharArrayIntoIntArray(const char *charArray, std::vector<int> &intArray, int sentinelValue)
{
    char *copy = strdup(charArray);
    
    char *value = std::strtok(copy, ",");
    
    int i = 0;
    
    while (value != NULL)
    {
        int intValue = atoi(value);
        if (intValue != sentinelValue)
        {
            intArray.push_back(intValue);
        }
        
        value = strtok(NULL, ","); // Get next event
        i++;
    }
    
    free(copy);
    free(value);
}

std::vector<std::unique_ptr<MapBorder>> & GameSession::getMapBorders()
{
    return m_mapBorders;
}

std::vector<std::unique_ptr<InsideBlock>> & GameSession::getInsideBlocks()
{
    return m_insideBlocks;
}

std::vector<std::unique_ptr<BreakableBlock>> & GameSession::getBreakableBlocks()
{
    return m_breakableBlocks;
}

std::vector<std::unique_ptr<SpaceTile>> & GameSession::getSpaceTiles()
{
    return m_spaceTiles;
}

std::vector<std::unique_ptr<IcePatch>> & GameSession::getIcePatches()
{
    return m_icePatches;
}

std::vector<std::unique_ptr<Crater>> & GameSession::getCraters()
{
    return m_craters;
}

std::vector<std::unique_ptr<IceBall>> & GameSession::getIceBalls()
{
    return m_iceBalls;
}

std::vector<std::unique_ptr<FireBall>> & GameSession::getFireBalls()
{
    return m_fireBalls;
}

std::vector<std::unique_ptr<PlayerDynamicGameObject>> & GameSession::getPlayers()
{
    return m_players;
}

std::vector<std::unique_ptr<BombGameObject>> & GameSession::getBombs()
{
    return m_bombs;
}

std::vector<std::unique_ptr<PowerUp>> & GameSession::getPowerUps()
{
    return m_powerUps;
}

std::vector<std::unique_ptr<Explosion>> & GameSession::getExplosions()
{
    return m_explosions;
}

GameListener * GameSession::getGameListener()
{
    return m_gameListener.get();
}

Map * GameSession::getMap()
{
    return m_map.get();
}

float GameSession::getCountDownTimeLeft()
{
    return m_fCountDownTimeLeft;
}

void GameSession::setCountDownTimeLeft(float countDownTimeLeft)
{
    m_fCountDownTimeLeft = countDownTimeLeft;
}

int GameSession::getNumBreakableBlocksAtSpawnTime()
{
    return m_iNumBreakableBlocksAtSpawnTime;
}

void GameSession::setNumBreakableBlocksAtSpawnTime(int numBreakableBlocksAtSpawnTime)
{
    m_iNumBreakableBlocksAtSpawnTime = numBreakableBlocksAtSpawnTime;
}

#pragma mark <Protected>

void GameSession::initializeMap(int mapType)
{
    m_map = std::unique_ptr<Map>(MapFactory::getInstance().createMap(mapType));
    
    m_map->initializeMap(this);
}

void GameSession::updateMap(float deltaTime)
{
    m_map->update(this, deltaTime, m_isSuddenDeath);
}

void GameSession::updateBots()
{
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        if ((*itr)->isBot())
        {
            (*itr)->handlePowerUps(m_powerUps);
            
            if ((*itr)->isHitByExplosion(m_explosions, m_bombs))
            {
                m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, (**itr));
            }
            
            if(m_isSuddenDeath)
            {
                m_map->updatePlayerForSuddenDeath(this, (*itr).get());
            }
        }
    }
}

void GameSession::handlePlayerDataUpdate(rapidjson::Document& d, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame)
{
    if (d.HasMember(keyX) && d.HasMember(keyY) && d.HasMember(keyDirection))
    {
        float playerX = d[keyX].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setX(playerX);

        float playerY = d[keyY].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setY(playerY);

        int playerDirection = d[keyDirection].GetInt();
        m_players.at(playerIndex).get()->setDirection(playerDirection);
        
        m_players.at(playerIndex).get()->updateBounds();
        m_players.at(playerIndex).get()->updateGrid();
    }
    
    if(d.HasMember(keyAlive))
    {
        bool isPlayerAlive = d[keyAlive].GetBool();
        Player_State playerState = m_players.at(playerIndex).get()->getPlayerState();
        if(playerState == ALIVE && !isPlayerAlive)
        {
            m_players.at(playerIndex).get()->setPlayerState(DEAD);
        }
    }
    
    if(d.HasMember(keyIsBot))
    {
        bool isBot = d[keyIsBot].GetBool();
        m_players.at(playerIndex).get()->setIsBot(isBot);
    }
}

void GameSession::handleClientEventsArrayInDocument(rapidjson::Document &d)
{
    static const char *eventsKey = "events";

    handleIntArrayInDocument(d, eventsKey, m_sEventIds, 0);
}

void GameSession::handleIntArrayInDocument(rapidjson::Document &d, const char *intArrayKey, std::vector<int> &intArray, int sentinelValue)
{
    if (d.HasMember(intArrayKey))
    {
        const char *charArray = d[intArrayKey].GetString();

        readCharArrayIntoIntArray(charArray, intArray, sentinelValue);
    }
}

void GameSession::handlePlayerEvent(int event)
{
    int playerIndex = 0;
    
    while(event >= PLAYER_EVENT_BASE)
    {
        event -= PLAYER_EVENT_BASE;
        playerIndex++;
    }
    
    int direction = 0;
    while (event >= PLAYER_EVENT_DIRECTION_BASE)
    {
        event -= PLAYER_EVENT_DIRECTION_BASE;
        direction++;
    }
    m_players.at(playerIndex).get()->setDirection(direction);
    
    int gridX = 0;
    while (event >= PLAYER_EVENT_GRID_X_BASE)
    {
        event -= PLAYER_EVENT_GRID_X_BASE;
        gridX++;
    }
    m_players.at(playerIndex).get()->setGridX(gridX);
    
    int gridY = 0;
    while (event >= PLAYER_EVENT_GRID_Y_BASE)
    {
        event -= PLAYER_EVENT_GRID_Y_BASE;
        gridY++;
    }
    m_players.at(playerIndex).get()->setGridY(gridY);
    
    int eventMod = 0;
    while (event >= PLAYER_EVENT_MOD_BASE)
    {
        event -= PLAYER_EVENT_MOD_BASE;
        eventMod++;
    }
    
    switch (event)
    {
        case PLAYER_MOVE_RIGHT:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_MOVE_UP:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_MOVE_LEFT:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_MOVE_DOWN:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_MOVE_STOP:
            m_players.at(playerIndex).get()->moveInDirection(-1);
            break;
        case PLAYER_PLANT_BOMB:
            // eventMod reflects the number shown on the power up bar, hence the + 1 here (fire power is 1 by default)
            layBombForPlayer(m_players.at(playerIndex).get(), eventMod + 1);
            break;
        case PLAYER_PUSH_BOMB:
            pushBombForPlayer(m_players.at(playerIndex).get());
            break;
        case PLAYER_RAISE_SHIELD:
            m_players.at(playerIndex).get()->raiseShield();
            break;
        case PLAYER_LOWER_SHIELD:
            m_players.at(playerIndex).get()->lowerShield();
            break;
        case PLAYER_FORCE_FIELD_HIT:
            m_players.at(playerIndex).get()->onForceFieldHit();
            break;
        case PLAYER_DEATH:
            m_players.at(playerIndex).get()->onDeath();
            break;
        case PLAYER_ABOUT_TO_FALL:
            m_players.at(playerIndex).get()->onTrappedOnFallingSpaceTile(m_spaceTiles);
            break;
        case PLAYER_FALL:
            m_players.at(playerIndex).get()->onFall();
            break;
        case PLAYER_FREEZE:
            m_players.at(playerIndex).get()->onFreeze();
            break;
        case PLAYER_PU_BOMB:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_BOMB);
            break;
        case PLAYER_PU_FIRE:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_FIRE);
            break;
        case PLAYER_PU_FORCE_FIELD:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_FORCE_FIELD);
            break;
        case PLAYER_PU_SPEED:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_SPEED);
            break;
        case PLAYER_PU_PUSH:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_PUSH);
            break;
        case PLAYER_PU_SHIELD:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_SHIELD);
            break;
        default:
            break;
    }
    
    if(event >= PLAYER_PU_BOMB)
    {
        for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
        {
            if((*itr)->getGridX() == gridX && (*itr)->getGridY() == gridY)
            {
                itr = m_powerUps.erase(itr);
            }
            else
            {
                itr++;
            }
        }
    }
}

#pragma mark <Private>

void GameSession::layBombForPlayer(PlayerDynamicGameObject *player, int firePower)
{
    BombGameObject *bomb = new BombGameObject(player, firePower, player->getGridX(), player->getGridY());
    m_bombs.push_back(std::unique_ptr<BombGameObject>(bomb));
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        if(OverlapTester::doRectanglesOverlap(bomb->getBounds(), (*itr)->getBounds()))
        {
            (*itr)->setGridX(bomb->getGridX());
            (*itr)->setGridY(bomb->getGridY());
        }
    }
}

void GameSession::pushBombForPlayer(PlayerDynamicGameObject *player)
{
    for(std::vector<std::unique_ptr<BombGameObject>>::iterator itr = m_bombs.begin(); itr != m_bombs.end(); itr++)
    {
        if(player->isBombInFrontOfPlayer(**itr))
        {
            (*itr)->pushed(player->getDirection());
            player->onBombPushed((*itr).get());
        }
    }
}
