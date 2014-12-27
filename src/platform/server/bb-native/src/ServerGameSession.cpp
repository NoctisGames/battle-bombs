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
#include "Map.h"
#include "GameConstants.h"

ServerGameSession::ServerGameSession()
{
    init();
}

void ServerGameSession::initWithNumHumanPlayersAndMapType(int numHumanPlayers, int mapType)
{
    init();

    initializeMap(mapType);
    
    m_map->populateMapWithPlayersAndBreakableBlocks(this, numHumanPlayers);
}

void ServerGameSession::init()
{
    GameSession::init();
    
    m_deletedBreakableBlockXValues.clear();
    m_deletedBreakableBlockYValues.clear();
    m_deletedPowerUpsXValues.clear();
    m_deletedPowerUpsYValues.clear();

    m_fCountDownTimeLeft = 4;
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
                m_gameState = GAME_STATE_COUNTING_DOWN;
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
                    
                    m_gameState = GAME_STATE_ENDING;
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
        case GAME_STATE_COUNTING_DOWN:
            m_fCountDownTimeLeft -= deltaTime;
            if (m_fCountDownTimeLeft < 0)
            {
                m_gameState = GAME_STATE_RUNNING;

                for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
                {
                    (*itr)->reset();
                }
            }
            break;
        case GAME_STATE_RUNNING:
        case GAME_STATE_ENDING:
            updateRunning(deltaTime);
            break;
        default:
            break;
    }
}

int ServerGameSession::getNumPlayers()
{
    return (int) m_players.size();
}

const char * ServerGameSession::getPlayerNameAtIndex(short playerIndex)
{
    return m_botNames.at(playerIndex);
}

bool ServerGameSession::isPlayerBotAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->isBot();
}

bool ServerGameSession::isPlayerAliveAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPlayerState() == Player_State::ALIVE;
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