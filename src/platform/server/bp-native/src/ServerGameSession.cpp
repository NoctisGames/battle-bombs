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

//For RNG purposes
#include <stdlib.h>
#include <time.h>

ServerGameSession::ServerGameSession()
{
    m_gameListener = std::unique_ptr<GameListener>(new GameListener());

    init();
}

void ServerGameSession::initWithNumHumanPlayers(int numHumanPlayers)
{
    init();

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 1 ?
            new PlayerDynamicGameObject(0, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_BOTTOM, m_gameListener.get(), DIRECTION_RIGHT) :
            new BotPlayerDynamicGameObject(0, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_BOTTOM, m_gameListener.get(), DIRECTION_RIGHT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 2 ?
            new PlayerDynamicGameObject(1, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_BOTTOM_HALF_TOP, m_gameListener.get(), DIRECTION_LEFT) :
            new BotPlayerDynamicGameObject(1, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_BOTTOM_HALF_TOP, m_gameListener.get(), DIRECTION_LEFT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 3 ?
            new PlayerDynamicGameObject(2, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_TOP_HALF_BOTTOM, m_gameListener.get(), DIRECTION_RIGHT) :
            new BotPlayerDynamicGameObject(2, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_TOP_HALF_BOTTOM, m_gameListener.get(), DIRECTION_RIGHT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 4 ?
            new PlayerDynamicGameObject(3, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_TOP, m_gameListener.get(), DIRECTION_LEFT) :
            new BotPlayerDynamicGameObject(3, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_TOP, m_gameListener.get(), DIRECTION_LEFT)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 5 ?
            new PlayerDynamicGameObject(4, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_BOTTOM, m_gameListener.get(), DIRECTION_UP) :
            new BotPlayerDynamicGameObject(4, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_BOTTOM, m_gameListener.get(), DIRECTION_UP)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 6 ?
            new PlayerDynamicGameObject(5, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_TOP_HALF_BOTTOM, m_gameListener.get(), DIRECTION_UP) :
            new BotPlayerDynamicGameObject(5, PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_TOP_HALF_BOTTOM, m_gameListener.get(), DIRECTION_UP)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 7 ?
            new PlayerDynamicGameObject(6, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_BOTTOM_HALF_TOP, m_gameListener.get(), DIRECTION_DOWN) :
            new BotPlayerDynamicGameObject(6, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_BOTTOM_HALF_TOP, m_gameListener.get(), DIRECTION_DOWN)));

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 8 ?
            new PlayerDynamicGameObject(7, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_TOP, m_gameListener.get(), DIRECTION_DOWN) :
            new BotPlayerDynamicGameObject(7, PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_TOP, m_gameListener.get(), DIRECTION_DOWN)));

    srand(time(NULL));

    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            // Don't place breakable blocks where inside blocks are
            if (i <= PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP && i % 2 != 0 && j % 2 != 0)
            {
                continue;
            }

            if (i > PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP && i < PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM && j == 0)
            {
                continue;
            }

            if (i > PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP && i < PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM && j % 2 != 0)
            {
                continue;
            }

            if (i > PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP && i < PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM && j == NUM_GRID_CELLS_PER_ROW - 1)
            {
                continue;
            }

            if (i >= PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM && i % 2 == 0 && j % 2 != 0)
            {
                continue;
            }

            if (i == 0 && (j <= 1 || j >= NUM_GRID_CELLS_PER_ROW - 2))
            {
                continue;
            }

            if (i == 1 && (j == 0 || j == NUM_GRID_CELLS_PER_ROW - 1))
            {
                continue;
            }

            if (i == PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP - 1 && (j == 0 || j == NUM_GRID_CELLS_PER_ROW - 1))
            {
                continue;
            }

            if (i == PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP && (j <= 1 || j >= NUM_GRID_CELLS_PER_ROW - 2))
            {
                continue;
            }

            if (i == PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM && (j <= 1 || j >= NUM_GRID_CELLS_PER_ROW - 2))
            {
                continue;
            }

            if (i == PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM + 1 && (j == 0 || j == NUM_GRID_CELLS_PER_ROW - 1))
            {
                continue;
            }

            if (i == GRID_CELL_NUM_ROWS - 1 && (j <= 1 || j >= NUM_GRID_CELLS_PER_ROW - 2))
            {
                continue;
            }

            if (i == GRID_CELL_NUM_ROWS - 2 && (j == 0 || j == NUM_GRID_CELLS_PER_ROW - 1))
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

                // 40% chance of a random passive powerup
                // These all have an equal chance of showing up
                if (flagRange < 41)
                {
                    flag = rand() % 3 + 1;
                }
                else if (flagRange > 40 && flagRange < 61)
                {
                    // 20% chance of an active powerup
                    // We'll determine the weight of these once we get them created and play with them a bit
                    // Right now, it just sets the flag to 4 as a placeholder value for the Kick
                    flag = 4;
                }

                // If these two blocks don't get caught, the flag remains 0 and no powerup will be created
                m_breakableBlocks.push_back(std::unique_ptr<BreakableBlock>(new BreakableBlock(j, i, flag)));
            }
        }
    }
}

void ServerGameSession::init()
{
    m_breakableBlocks.clear();
    m_players.clear();
    m_bombs.clear();
    m_explosions.clear();
    m_powerUps.clear();
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

short ServerGameSession::popOldestEventId()
{
    return m_gameListener->popOldestEventId();
}

// Private Methods

void ServerGameSession::updateRunning(float deltaTime)
{
    std::vector<short> localConsumedEventIds = m_gameListener->freeLocalEventIds();

    for (std::vector<short>::iterator itr = localConsumedEventIds.begin(); itr != localConsumedEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }

    for (std::vector<short>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }

    m_sEventIds.clear();

    updateCommon(deltaTime);
}

void ServerGameSession::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex, bool isBeginGame)
{
    handlePositionAndDirectionUpdate(d, keyX, keyY, keyDirection, playerIndex);

    handleClientEventsArrayInDocument(d);
}