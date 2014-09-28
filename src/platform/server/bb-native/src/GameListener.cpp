//
//  GameListener.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "string.h"
#include "GameListener.h"
#include "PlayerDynamicGameObject.h"
#include "GameEvent.h"

GameListener::GameListener()
{
    // No setup required
}

void GameListener::addLocalEventForPlayer(int eventId, PlayerDynamicGameObject &player)
{
    if(eventId == PLAYER_PLANT_BOMB)
    {
        eventId += PLAYER_EVENT_MOD_BASE * (player.getFirePower() - 1);
    }
    
    eventId += PLAYER_EVENT_BASE * player.getPlayerIndex();
    eventId += PLAYER_EVENT_DIRECTION_BASE * player.getDirection();
    eventId += PLAYER_EVENT_GRID_X_BASE * player.getGridX();
    eventId += PLAYER_EVENT_GRID_Y_BASE * player.getGridY();

    m_sLocalEventIds.push_back(eventId);
}

int GameListener::popOldestEventId()
{
    if (m_sLocalConsumedEventIds.size() > 0)
    {
        int oldestEvent = m_sLocalConsumedEventIds.front();

        m_sLocalConsumedEventIds.erase(m_sLocalConsumedEventIds.begin());

        return oldestEvent;
    }

    return 0;
}

void GameListener::addServerMessage(const char *serverMessage)
{
    char *copy = strdup(serverMessage);
    m_serverMessagesBuffer.push_back(copy);
}

std::vector<int> & GameListener::freeLocalEventIds()
{
    m_sLocalConsumedEventIds.clear();
    m_sLocalConsumedEventIds.swap(m_sLocalEventIds);
    m_sLocalEventIds.clear();

    return m_sLocalConsumedEventIds;
}

std::vector<const char *> & GameListener::freeServerMessages()
{
    m_serverMessages.clear();
    m_serverMessages.swap(m_serverMessagesBuffer);
    m_serverMessagesBuffer.clear();

    return m_serverMessages;
}

void GameListener::playSound(short soundId)
{
    // No Sound on Server
}
