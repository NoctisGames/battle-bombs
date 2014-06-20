//
//  GameSession.cpp
//  bomber-party
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
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"

GameSession::GameSession()
{
    for (int i = 1; i < GRID_CELL_NUM_ROWS / 2 - 2; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
    
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP + 1)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM - 1)));
    
    for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP + 1)));
        m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM - 1)));
    }
    
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP + 1)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM - 1)));
    
    for (int i = GRID_CELL_NUM_ROWS / 2 + 2; i < GRID_CELL_NUM_ROWS; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
}

int GameSession::getNumPlayers()
{
    return m_players.size();
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

void GameSession::updateCommon(float deltaTime)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = m_bombs.begin(); itr != m_bombs.end();)
    {
        (**itr).update(deltaTime, m_explosions, m_insideBlocks, m_breakableBlocks);

        if ((**itr).isExploding())
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
                m_powerUps.push_back(std::unique_ptr<PowerUp>(new PowerUp((**itr).getX(), (**itr).getY(), (**itr).getPowerUpFlag())));
            }

            itr = m_breakableBlocks.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = m_explosions.begin(); itr != m_explosions.end();)
    {
        (**itr).update(deltaTime);

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
        (**itr).update(deltaTime, m_insideBlocks, m_breakableBlocks, m_powerUps, m_explosions);
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

    clientUpdateForPlayerIndex(d, playerIndex0Key, playerIndex0XKey, playerIndex0YKey, playerIndex0DirectionKey, 0, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex1Key, playerIndex1XKey, playerIndex1YKey, playerIndex1DirectionKey, 1, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex2Key, playerIndex2XKey, playerIndex2YKey, playerIndex2DirectionKey, 2, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex3Key, playerIndex3XKey, playerIndex3YKey, playerIndex3DirectionKey, 3, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex4Key, playerIndex4XKey, playerIndex4YKey, playerIndex4DirectionKey, 4, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex5Key, playerIndex5XKey, playerIndex5YKey, playerIndex5DirectionKey, 5, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex6Key, playerIndex6XKey, playerIndex6YKey, playerIndex6DirectionKey, 6, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex7Key, playerIndex7XKey, playerIndex7YKey, playerIndex7DirectionKey, 7, isBeginGame);
}

void GameSession::handlePositionAndDirectionUpdate(rapidjson::Document& d, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex)
{
    if (d.HasMember(keyX) && d.HasMember(keyY) && d.HasMember(keyDirection))
    {
        float playerX = d[keyX].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setX(playerX);

        float playerY = d[keyY].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setY(playerY);

        int playerDirection = d[keyDirection].GetInt();
        m_players.at(playerIndex).get()->setDirection(playerDirection);
    }
}

void GameSession::handleClientEventsArrayInDocument(rapidjson::Document &d)
{
    static const char *eventsKey = "events";

    handleShortArrayInDocument(d, eventsKey, m_sEventIds, 0);
}

void GameSession::handleShortArrayInDocument(rapidjson::Document &d, const char *shortArrayKey, std::vector<short> &shortArray, short sentinelValue)
{
    if (d.HasMember(shortArrayKey))
    {
        const char *charArray = d[shortArrayKey].GetString();

        char *copy = strdup(charArray);

        char *value = std::strtok(copy, ",");

        while (value != NULL)
        {
            short shortValue = atoi(value);
            if (shortValue != sentinelValue)
            {
                shortArray.push_back(shortValue);
            }

            value = strtok(NULL, ","); // Get next event
        }

        free(copy);
        free(value);
    }
}

void GameSession::handlePlayerEvent(short event)
{
    switch (event)
    {
        case PLAYER_0_MOVE_RIGHT:
            m_players.at(0).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_0_MOVE_UP:
            m_players.at(0).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_0_MOVE_LEFT:
            m_players.at(0).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_0_MOVE_DOWN:
            m_players.at(0).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_0_MOVE_STOP:
            m_players.at(0).get()->moveInDirection(-1);
            break;
        case PLAYER_0_PLANT_BOMB:
            layBombForPlayer(m_players.at(0).get());
            break;
        case PLAYER_0_DEATH:
            m_players.at(0).get()->onDeath();
            break;
        case PLAYER_1_MOVE_RIGHT:
            m_players.at(1).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_1_MOVE_UP:
            m_players.at(1).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_1_MOVE_LEFT:
            m_players.at(1).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_1_MOVE_DOWN:
            m_players.at(1).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_1_MOVE_STOP:
            m_players.at(1).get()->moveInDirection(-1);
            break;
        case PLAYER_1_PLANT_BOMB:
            layBombForPlayer(m_players.at(1).get());
            break;
        case PLAYER_1_DEATH:
            m_players.at(1).get()->onDeath();
            break;
        case PLAYER_2_MOVE_RIGHT:
            m_players.at(2).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_2_MOVE_UP:
            m_players.at(2).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_2_MOVE_LEFT:
            m_players.at(2).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_2_MOVE_DOWN:
            m_players.at(2).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_2_MOVE_STOP:
            m_players.at(2).get()->moveInDirection(-1);
            break;
        case PLAYER_2_PLANT_BOMB:
            layBombForPlayer(m_players.at(2).get());
            break;
        case PLAYER_2_DEATH:
            m_players.at(2).get()->onDeath();
            break;
        case PLAYER_3_MOVE_RIGHT:
            m_players.at(3).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_3_MOVE_UP:
            m_players.at(3).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_3_MOVE_LEFT:
            m_players.at(3).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_3_MOVE_DOWN:
            m_players.at(3).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_3_MOVE_STOP:
            m_players.at(3).get()->moveInDirection(-1);
            break;
        case PLAYER_3_PLANT_BOMB:
            layBombForPlayer(m_players.at(3).get());
            break;
        case PLAYER_3_DEATH:
            m_players.at(3).get()->onDeath();
            break;
        case PLAYER_4_MOVE_RIGHT:
            m_players.at(4).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_4_MOVE_UP:
            m_players.at(4).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_4_MOVE_LEFT:
            m_players.at(4).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_4_MOVE_DOWN:
            m_players.at(4).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_4_MOVE_STOP:
            m_players.at(4).get()->moveInDirection(-1);
            break;
        case PLAYER_4_PLANT_BOMB:
            layBombForPlayer(m_players.at(4).get());
            break;
        case PLAYER_4_DEATH:
            m_players.at(4).get()->onDeath();
            break;
        case PLAYER_5_MOVE_RIGHT:
            m_players.at(5).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_5_MOVE_UP:
            m_players.at(5).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_5_MOVE_LEFT:
            m_players.at(5).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_5_MOVE_DOWN:
            m_players.at(5).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_5_MOVE_STOP:
            m_players.at(5).get()->moveInDirection(-1);
            break;
        case PLAYER_5_PLANT_BOMB:
            layBombForPlayer(m_players.at(5).get());
            break;
        case PLAYER_5_DEATH:
            m_players.at(5).get()->onDeath();
            break;
        case PLAYER_6_MOVE_RIGHT:
            m_players.at(6).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_6_MOVE_UP:
            m_players.at(6).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_6_MOVE_LEFT:
            m_players.at(6).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_6_MOVE_DOWN:
            m_players.at(6).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_6_MOVE_STOP:
            m_players.at(6).get()->moveInDirection(-1);
            break;
        case PLAYER_6_PLANT_BOMB:
            layBombForPlayer(m_players.at(6).get());
            break;
        case PLAYER_6_DEATH:
            m_players.at(6).get()->onDeath();
            break;
        case PLAYER_7_MOVE_RIGHT:
            m_players.at(7).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_7_MOVE_UP:
            m_players.at(7).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_7_MOVE_LEFT:
            m_players.at(7).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_7_MOVE_DOWN:
            m_players.at(7).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_7_MOVE_STOP:
            m_players.at(7).get()->moveInDirection(-1);
            break;
        case PLAYER_7_PLANT_BOMB:
            layBombForPlayer(m_players.at(7).get());
            break;
        case PLAYER_7_DEATH:
            m_players.at(7).get()->onDeath();
            break;
        default:
            break;
    }
}

void GameSession::layBombForPlayer(PlayerDynamicGameObject *player)
{
    float playerBoundsLowerLeftX = player->getBounds().getLowerLeft().getX() + player->getBounds().getWidth() / 2;
    float playerBoundsLowerLeftY = player->getBounds().getLowerLeft().getY() + player->getBounds().getHeight() / 2;

    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            float leftX = GAME_X + GRID_CELL_WIDTH * j;
            float rightX = GAME_X + GRID_CELL_WIDTH * (j + 1);
            float bottomY = GAME_Y + GRID_CELL_HEIGHT * i;
            float topY = GAME_Y + GRID_CELL_HEIGHT * (i + 1);

            if (playerBoundsLowerLeftX > leftX && playerBoundsLowerLeftX < rightX && playerBoundsLowerLeftY > bottomY && playerBoundsLowerLeftY < topY)
            {
                m_bombs.push_back(std::unique_ptr<BombGameObject>(new BombGameObject(player, player->getFirePower(), j, i)));
                return;
            }
        }
    }
}