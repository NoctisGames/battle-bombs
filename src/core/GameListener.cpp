//
//  GameListener.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "GameListener.h"
#include "Assets.h"

GameListener::GameListener()
{
    // No setup required
}

void GameListener::addLocalEvent(short eventId)
{
    m_sLocalEventIds.push_back(eventId);
}

short GameListener::popOldestEventId()
{
    if(m_sLocalConsumedEventIds.size() > 0)
    {
		short oldestEvent = m_sLocalConsumedEventIds.front();
        
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

std::vector<short> & GameListener::freeLocalEventIds()
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
    Assets::getInstance()->addSoundIdToPlayQueue(soundId);
}