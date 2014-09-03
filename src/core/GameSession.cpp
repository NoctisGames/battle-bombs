//
//  GameSession.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 3/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "GameSession.h"
#include "Vector2D.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Assets.h"
#include "OverlapTester.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"
#include "Fire.h"
#include "PathFinder.h"
#include <iostream>

GameSession::GameSession()
{
    // I guess this is empty now
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

void GameSession::initializeInsideBlocksAndMapBordersForMapType(int mapType)
{
    m_iMapType = mapType;
    
    m_mapBorders.clear();
    m_insideBlocks.clear();
    
    // BEGIN MAP BORDER FAR
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_TOP, SCREEN_WIDTH / 2, SCREEN_HEIGHT_X2 - 0.73880597023028f, SCREEN_WIDTH, 1.47761194046056f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_LEFT, 0.7388059701492f, 15.76119403157902f, 1.4776119402984f, 20.0597014947369f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_RIGHT, 23.44029850746264f, 15.76119403157902f, 1.11940298507472f, 20.0597014947369f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM_LEFT, 2.865671641791f, 3.58208955263162f, 5.731343283582f, 4.29850746315789f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM_RIGHT, 21.31343283582084f, 3.58208955263162f, 5.37313432835832f, 4.29850746315789f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM, SCREEN_WIDTH / 2, 0.71641791052634f, SCREEN_WIDTH, 1.43283582105267f)));
    
    if(mapType == MAP_MOUNTAINS)
    {
        m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(MOUNTAINS_DOOR, GAME_X + GRID_CELL_WIDTH * 7 + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * GRID_CELL_NUM_ROWS + GRID_CELL_HEIGHT / 2.0f, GRID_CELL_WIDTH * 3, GRID_CELL_HEIGHT * 2)));
    }
    
    // BEGIN BOTTOM CENTER
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 5, 0)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 7, 0)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 9, 0)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 4, 2)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 6, 2)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 8, 2)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 2)));
    
    // BEGIN BOTTOM
    for (int i = 4; i < BOTTOM_HALF_TOP_GRID_Y; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
    
    // BEGIN MIDDLE
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, BOTTOM_HALF_TOP_GRID_Y + 1)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, TOP_HALF_BOTTOM_GRID_Y - 1)));
    
    for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, BOTTOM_HALF_TOP_GRID_Y + 1)));
        m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, TOP_HALF_BOTTOM_GRID_Y - 1)));
    }
    
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, BOTTOM_HALF_TOP_GRID_Y + 1)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, TOP_HALF_BOTTOM_GRID_Y - 1)));
    
    // BEGIN TOP
    for (int i = TOP_HALF_BOTTOM_GRID_Y + 1; i < GRID_CELL_NUM_ROWS; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            if(mapType == MAP_MOUNTAINS)
            {
                if(i >= GRID_CELL_NUM_ROWS - 2 && j >= 5 && j <= 9)
                {
                    continue;
                }
            }
            
            m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
}

void GameSession::updateCommon(float deltaTime)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = m_bombs.begin(); itr != m_bombs.end();)
    {
        (**itr).update(deltaTime, m_explosions, m_mapBorders, m_insideBlocks, m_breakableBlocks, m_players, m_bombs);

        if ((**itr).isDestroyed())
        {
            itr = m_bombs.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = m_breakableBlocks.begin(); itr != m_breakableBlocks.end();)
    {
        (**itr).update(deltaTime);
        
        if ((**itr).getBreakableBlockState() == DESTROYED)
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
            if ((**itr).getBreakableBlockState() == EXPLODING)
            {
                PathFinder::getInstance().freeGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
            }
            
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = m_explosions.begin(); itr != m_explosions.end();)
    {
        (**itr).update(deltaTime, m_insideBlocks, m_breakableBlocks);

        if ((**itr).isComplete())
        {
            itr = m_explosions.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        (**itr).update(deltaTime, m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps, m_explosions, m_players, m_bombs);
    }

    for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
    {
        (**itr).update(deltaTime);
        
        if ((**itr).isPickedUp())
        {
            itr = m_powerUps.erase(itr);
        }
        else
        {
            itr++;
        }
    }
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

void GameSession::handlePlayerDataUpdate(rapidjson::Document& d, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex)
{
    if (d.HasMember(keyX) && d.HasMember(keyY) && d.HasMember(keyDirection))
    {
        float playerX = d[keyX].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setX(playerX);

        float playerY = d[keyY].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setY(playerY);

        int playerDirection = d[keyDirection].GetInt();
        m_players.at(playerIndex).get()->setDirection(playerDirection);
        
        m_players.at(playerIndex).get()->updateGrid();
    }
    
    if(d.HasMember(keyAlive))
    {
        bool isPlayerAlive = d[keyAlive].GetBool();
        m_players.at(playerIndex).get()->setPlayerState(isPlayerAlive ? ALIVE : DEAD);
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
            layBombForPlayer(m_players.at(playerIndex).get(), eventMod);
            break;
        case PLAYER_PUSH_BOMB:
            pushBombForPlayer(m_players.at(playerIndex).get());
            break;
        case PLAYER_FORCE_FIELD_HIT:
            m_players.at(playerIndex).get()->onForceFieldHit();
            break;
        case PLAYER_DEATH:
            m_players.at(playerIndex).get()->onDeath();
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
