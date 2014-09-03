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
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Assets.h"
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

//For RNG purposes
#include <stdlib.h>
#include <time.h>

ServerGameSession::ServerGameSession()
{
    m_gameListener = std::unique_ptr<GameListener>(new GameListener());

    init();
}

void ServerGameSession::initWithNumHumanPlayersAndMapType(int numHumanPlayers, int mapType)
{
    init();

    initializeInsideBlocksAndMapBordersForMapType(mapType);

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

            // 70% chance there will be a breakable block at all
            if ((rand() % 100 + 1) < 71)
            {
                int flag = 0;

                // Generate a random number from 1 - 100
                // This will be used to determine which type of powerups will appear
                int flagRange = (rand() % 100 + 1);

                // TODO, generate a random power up flag
                // Some Power Up are more rare than others,
                // so that should be reflected here

                // 50% chance of a random passive powerup
                // These all have an equal chance of showing up
                if (flagRange <= 50)
                {
                    flag = rand() % 4 + 1;
                }
                else if (flagRange > 50 && flagRange < 70)
                {
                    // 20% chance of an active powerup
                    // We'll determine the weight of these once we get them created and play with them a bit
                    // Right now, it just sets the flag to 5 as a placeholder value for the Push
                    flag = 5;
                }

                // If these two blocks don't get caught, the flag remains 0 and no powerup will be created
                m_breakableBlocks.push_back(std::unique_ptr<BreakableBlock>(new BreakableBlock(j, i, flag)));
            }
        }
    }

    PathFinder::getInstance().resetGameGrid();
    PathFinder::getInstance().initializeGameGrid(m_insideBlocks, m_breakableBlocks, mapType);
}

void ServerGameSession::init()
{
    m_breakableBlocks.clear();
    m_players.clear();
    m_bombs.clear();
    m_explosions.clear();
    m_powerUps.clear();

    srand((int) time(NULL));
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
                m_gameState = RUNNING;
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
        }
    }

    switch (m_gameState)
    {
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

int ServerGameSession::getBreakableBlockGridX(short breakableBlockIndex)
{
    return m_breakableBlocks.at(breakableBlockIndex).get()->getGridX();
}

int ServerGameSession::getBreakableBlockGridY(short breakableBlockIndex)
{
    return m_breakableBlocks.at(breakableBlockIndex).get()->getGridY();
}

int ServerGameSession::getBreakableBlockPowerUpFlag(short breakableBlockIndex)
{
    return m_breakableBlocks.at(breakableBlockIndex).get()->getPowerUpFlag();
}

int ServerGameSession::popOldestEventId()
{
    return m_gameListener->popOldestEventId();
}

// Private Methods

void ServerGameSession::updateRunning(float deltaTime)
{
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

    updateCommon(deltaTime);
}

void ServerGameSession::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame)
{
    handlePlayerDataUpdate(d, keyIsBot, keyX, keyY, keyDirection, keyAlive, playerIndex);

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