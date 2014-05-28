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
#include "SoundListener.h"

//For RNG purposes
#include <stdlib.h>
#include <time.h>

ServerGameSession::ServerGameSession()
{
    m_soundListener = std::unique_ptr<SoundListener>(new SoundListener());

    init();
}

void ServerGameSession::initWithNumHumanPlayers(int numHumanPlayers)
{
    init();

    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_BOTTOM, m_soundListener.get(), DIRECTION_RIGHT)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_BOTTOM_HALF_TOP, m_soundListener.get(), DIRECTION_LEFT)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_TOP_HALF_BOTTOM, m_soundListener.get(), DIRECTION_RIGHT)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_TOP, m_soundListener.get(), DIRECTION_LEFT)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_BOTTOM, m_soundListener.get(), DIRECTION_UP)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_RIGHT, PLAYER_STARTING_Y_TOP_HALF_BOTTOM, m_soundListener.get(), DIRECTION_UP)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_BOTTOM_HALF_TOP, m_soundListener.get(), DIRECTION_DOWN)));
    m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(PLAYER_STARTING_X_LEFT, PLAYER_STARTING_Y_TOP, m_soundListener.get(), DIRECTION_DOWN)));

    srand(time(NULL));

    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            if (i % 2 != 0 && j % 2 != 0)
            {
                continue;
            }

            if ((i == 0 || i == GRID_CELL_NUM_ROWS - 1 || i == PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP || i == PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM) && (j < 2 || j >= NUM_GRID_CELLS_PER_ROW - 2))
            {
                continue;
            }

            if ((i == 1 || i == GRID_CELL_NUM_ROWS - 2 || i == PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP - 1 || i == PLAYER_STARTING_GRID_CELL_BOTTOM_HALF_TOP + 1 || i == PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM - 1 || i == PLAYER_STARTING_GRID_CELL_TOP_HALF_BOTTOM + 1) && (j == 0 || j == NUM_GRID_CELLS_PER_ROW - 1))
            {
                continue;
            }

            // 60% chance there will be a powerup at all
            if ((rand() % 100 + 1) < 61)
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

                    // 20% chance of an active powerup
                    // We'll determine the weight of these once we get them created and play with them a bit
                    // Right now, it just sets the flag to 4 as a placeholder value for the Kick
                else if (flagRange > 40 && flagRange < 61)
                {
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
    rapidjson::Document d;
    d.Parse<0>(message);

    int eventType = d["eventType"].GetInt();

    if (eventType == CLIENT_UPDATE)
    {
        clientUpdate(d, false);
    }
    else if (eventType == BEGIN_GAME)
    {
        // TODO, create 8 - numPlayers bots and activate them
        m_gameState = RUNNING;
    }
}

void ServerGameSession::update(float deltaTime)
{
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

void ServerGameSession::updateRunning(float deltaTime)
{
    for (std::vector<short>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }

    m_sEventIds.clear();

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
        if ((**itr).isDestroyed())
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
        (**itr).update(deltaTime, m_insideBlocks, m_breakableBlocks, m_powerUps);
    }

    for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
    {
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

void ServerGameSession::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex, bool isBeginGame)
{
    handlePositionAndDirectionUpdate(d, keyX, keyY, keyDirection, playerIndex);

    handleClientEventsArrayInDocument(d);
}