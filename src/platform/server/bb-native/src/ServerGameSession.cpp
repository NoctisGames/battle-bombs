//
//  ServerGameSession.cpp
//  bomber-party
//
//  Created by Stephen Gowen on 3/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "ServerGameSession.h"
#include "Vector2D.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "OverlapTester.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "GameListener.h"
#include "PlayerDynamicGameObject.h"
#include "BotPlayerDynamicGameObject.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h" 
#include "Fire.h"
#include "PathFinder.h"
#include "MapSearchNode.h"
#include "PowerUpType.h"
#include "PowerUp.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"

//For RNG purposes
#include <stdlib.h>
#include <time.h>

ServerGameSession::ServerGameSession()
{
    init();
}

void ServerGameSession::initWithNumHumanPlayersAndMapType(int numHumanPlayers, int mapType)
{
    init();

    initializeMap(mapType);

    int playerStartingPositions[8][2] = {
        {PLAYER_1_GRID_X, PLAYER_1_GRID_Y},
        {PLAYER_2_GRID_X, PLAYER_2_GRID_Y},
        {PLAYER_3_GRID_X, PLAYER_3_GRID_Y},
        {PLAYER_4_GRID_X, PLAYER_4_GRID_Y},
        {PLAYER_5_GRID_X, PLAYER_5_GRID_Y},
        {PLAYER_6_GRID_X, PLAYER_6_GRID_Y},
        {PLAYER_7_GRID_X, PLAYER_7_GRID_Y},
        {PLAYER_8_GRID_X, PLAYER_8_GRID_Y}
    };

    int n = sizeof (playerStartingPositions) / sizeof (playerStartingPositions[0]);
    randomize(playerStartingPositions, n);

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 1 ?
            new PlayerDynamicGameObject(0, playerStartingPositions[0][0], playerStartingPositions[0][1], m_gameListener.get(), DIRECTION_RIGHT) :
            new BotPlayerDynamicGameObject(0, playerStartingPositions[0][0], playerStartingPositions[0][1], m_gameListener.get(), DIRECTION_RIGHT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 2 ?
            new PlayerDynamicGameObject(1, playerStartingPositions[1][0], playerStartingPositions[1][1], m_gameListener.get(), DIRECTION_LEFT) :
            new BotPlayerDynamicGameObject(1, playerStartingPositions[1][0], playerStartingPositions[1][1], m_gameListener.get(), DIRECTION_LEFT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 3 ?
            new PlayerDynamicGameObject(2, playerStartingPositions[2][0], playerStartingPositions[2][1], m_gameListener.get(), DIRECTION_RIGHT) :
            new BotPlayerDynamicGameObject(2, playerStartingPositions[2][0], playerStartingPositions[2][1], m_gameListener.get(), DIRECTION_RIGHT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 4 ?
            new PlayerDynamicGameObject(3, playerStartingPositions[3][0], playerStartingPositions[3][1], m_gameListener.get(), DIRECTION_LEFT) :
            new BotPlayerDynamicGameObject(3, playerStartingPositions[3][0], playerStartingPositions[3][1], m_gameListener.get(), DIRECTION_LEFT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 5 ?
            new PlayerDynamicGameObject(4, playerStartingPositions[4][0], playerStartingPositions[4][1], m_gameListener.get(), DIRECTION_UP) :
            new BotPlayerDynamicGameObject(4, playerStartingPositions[4][0], playerStartingPositions[4][1], m_gameListener.get(), DIRECTION_UP)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 6 ?
            new PlayerDynamicGameObject(5, playerStartingPositions[5][0], playerStartingPositions[5][1], m_gameListener.get(), DIRECTION_DOWN) :
            new BotPlayerDynamicGameObject(5, playerStartingPositions[5][0], playerStartingPositions[5][1], m_gameListener.get(), DIRECTION_DOWN)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 7 ?
            new PlayerDynamicGameObject(6, playerStartingPositions[6][0], playerStartingPositions[6][1], m_gameListener.get(), DIRECTION_UP) :
            new BotPlayerDynamicGameObject(6, playerStartingPositions[6][0], playerStartingPositions[6][1], m_gameListener.get(), DIRECTION_UP)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 8 ?
            new PlayerDynamicGameObject(7, playerStartingPositions[7][0], playerStartingPositions[7][1], m_gameListener.get(), DIRECTION_DOWN) :
            new BotPlayerDynamicGameObject(7, playerStartingPositions[7][0], playerStartingPositions[7][1], m_gameListener.get(), DIRECTION_DOWN)));

    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            // Don't place breakable blocks where inside blocks or players are
            if (PathFinder::isLocationOccupiedByInsideBlock(m_insideBlocks, j, i) || isLocationOccupiedByPlayer(j, i))
            {
                continue;
            }

            // Don't place breakable blocks where map borders are
            if (i <= 2 && (j <= 2 || j >= NUM_GRID_CELLS_PER_ROW - 3))
            {
                continue;
            }

            // The Mountain map has a special DOOR border type
            if (mapType == MAP_MOUNTAINS && i == GRID_CELL_NUM_ROWS - 1 && j >= 6 && j <= 8)
            {
                continue;
            }

            // 80% chance there will be a breakable block at all
            if ((rand() % 100 + 1) < 81)
            {
                int flag = POWER_UP_TYPE_NONE;

                // Generate a random number from 1 - 100
                // This will be used to determine which type of powerups will appear
                int flagRange = (rand() % 100 + 1);

                if (flagRange <= 35)
                {
                    flag = rand() % POWER_UP_TYPE_SPEED + 1;
                }
                else if (flagRange > 35 && flagRange <= 40)
                {
                    flag = POWER_UP_TYPE_FORCE_FIELD;
                }
                else if (flagRange > 40 && flagRange <= 45)
                {
                    flag = POWER_UP_TYPE_PUSH;
                }
                else if (flagRange > 45 && flagRange <= 50)
                {
                    flag = POWER_UP_TYPE_SHIELD;
                }

                // If these two blocks don't get caught, the flag remains 0 and no powerup will be created
                m_breakableBlocks.push_back(std::unique_ptr<BreakableBlock>(new BreakableBlock(j, i, flag)));

                m_iNumBreakableBlocksAtSpawnTime++;
            }
        }
    }

    PathFinder::getInstance().resetGameGrid();
    PathFinder::getInstance().initializeGameGrid(m_insideBlocks, m_breakableBlocks, mapType);
}

void ServerGameSession::init()
{
    GameSession::init();
    
    m_deletedBreakableBlockXValues.clear();
    m_deletedBreakableBlockYValues.clear();
    m_deletedPowerUpsXValues.clear();
    m_deletedPowerUpsYValues.clear();

    srand((int) time(NULL));

    m_fCountDownTimeLeft = 4;
}

void ServerGameSession::handleServerUpdate(const char *message)
{
    m_gameListener->addServerMessage(message);
}

void ServerGameSession::update(float deltaTime)
{
    std::vector<const char *> serverMessages = m_gameListener->freeServerMessages();

    for (std::vector<const char *>::iterator itr = serverMessages.begin(); itr != serverMessages.end(); itr++)
    {
        static const char *eventTypeKey = "eventType";

        rapidjson::Document d;
        d.Parse<0>(*itr);
        if (d.HasMember(eventTypeKey))
        {
            int eventType = d[eventTypeKey].GetInt();

            if (eventType == CLIENT_UPDATE)
            {
                clientUpdate(d, false);
            }
            else if (eventType == BEGIN_GAME)
            {
                m_gameState = COUNTING_DOWN;
            }
            else if (eventType == GAME_OVER)
            {
                static const char *hasWinnerKey = "hasWinner";

                if (d.HasMember(hasWinnerKey))
                {
                    bool hasWinner = d[hasWinnerKey].GetBool();

                    if (hasWinner)
                    {
                        static const char *winningPlayerIndexKey = "winningPlayerIndex";
                        int winningPlayerIndex = d[winningPlayerIndexKey].GetInt();
                        m_players.at(winningPlayerIndex).get()->onWin();
                    }
                }
            }
            else if (eventType == SUDDEN_DEATH)
            {
                suddenDeath();
            }
        }
    }

    switch (m_gameState)
    {
        case COUNTING_DOWN:
            m_fCountDownTimeLeft -= deltaTime;
            if (m_fCountDownTimeLeft < 0)
            {
                m_gameState = RUNNING;

                for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
                {
                    (*itr)->reset();
                }
            }
            break;
        case RUNNING:
            updateRunning(deltaTime);
            break;
        default:
            break;
    }
}

int ServerGameSession::getNumBreakableBlocks()
{
    return m_breakableBlocks.size();
}

int ServerGameSession::getBreakableBlockGridX(int breakableBlockIndex)
{
    return m_breakableBlocks.at(breakableBlockIndex).get()->getGridX();
}

int ServerGameSession::getBreakableBlockGridY(int breakableBlockIndex)
{
    return m_breakableBlocks.at(breakableBlockIndex).get()->getGridY();
}

int ServerGameSession::getBreakableBlockPowerUpFlag(int breakableBlockIndex)
{
    return m_breakableBlocks.at(breakableBlockIndex).get()->getPowerUpFlag();
}

int ServerGameSession::getNumDeletedBreakableBlocks()
{
    return m_deletedBreakableBlockXValues.size();
}

int ServerGameSession::getDeletedBreakableBlockGridX(int breakableBlockIndex)
{
    return m_deletedBreakableBlockXValues.at(breakableBlockIndex);
}

int ServerGameSession::getDeletedBreakableBlockGridY(int breakableBlockIndex)
{
    return m_deletedBreakableBlockYValues.at(breakableBlockIndex);
}

int ServerGameSession::getNumDeletedPowerUps()
{
    return m_deletedPowerUpsXValues.size();
}

int ServerGameSession::getDeletedPowerUpGridX(int powerUpIndex)
{
    return m_deletedPowerUpsXValues.at(powerUpIndex);
}

int ServerGameSession::getDeletedPowerUpGridY(int powerUpIndex)
{
    return m_deletedPowerUpsYValues.at(powerUpIndex);
}

int ServerGameSession::getNumBreakableBlocksAtSpawnTime()
{
    return m_iNumBreakableBlocksAtSpawnTime;
}

int ServerGameSession::popOldestEventId()
{
    return m_gameListener->popOldestEventId();
}

// Protected Methods

void ServerGameSession::onBreakableBlockDestroyed(BreakableBlock &breakableBlock)
{
    m_deletedBreakableBlockXValues.push_back(breakableBlock.getGridX());
    m_deletedBreakableBlockYValues.push_back(breakableBlock.getGridY());
}

void ServerGameSession::onPowerUpPickedUp(PowerUp &powerUp)
{
    m_deletedPowerUpsXValues.push_back(powerUp.getGridX());
    m_deletedPowerUpsYValues.push_back(powerUp.getGridY());
}

// Private Methods

void ServerGameSession::updateRunning(float deltaTime)
{
    updateBots();

    std::vector<int> localConsumedEventIds = m_gameListener->freeLocalEventIds();

    for (std::vector<int>::iterator itr = localConsumedEventIds.begin(); itr != localConsumedEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }

    for (std::vector<int>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }

    m_sEventIds.clear();

    updateMap(deltaTime);
}

void ServerGameSession::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame)
{
    handlePlayerDataUpdate(d, keyIsBot, keyX, keyY, keyDirection, keyAlive, playerIndex, isBeginGame);

    handleClientEventsArrayInDocument(d);
}

bool ServerGameSession::isLocationOccupiedByPlayer(int gridX, int gridY)
{
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        if ((gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY()) || (gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY() + 1) || (gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY() - 1) || (gridX == (*itr)->getGridX() + 1 && gridY == (*itr)->getGridY()) || (gridX == (*itr)->getGridX() - 1 && gridY == (*itr)->getGridY()))
        {
            return true;
        }
    }

    return false;
}

void ServerGameSession::swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void ServerGameSession::randomize(int arr[][2], int n)
{
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n - 1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);

        // Swap arr[i] with the element at random index
        swap(&arr[i][0], &arr[j][0]);
        swap(&arr[i][1], &arr[j][1]);
    }
}