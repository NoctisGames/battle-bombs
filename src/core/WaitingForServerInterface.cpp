//
//  WaitingForServerInterface.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "WaitingForServerInterface.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameEvent.h"

WaitingForServerInterface::WaitingForServerInterface(float x, float y, float width, float height, const char *username) : GameObject(x, y, width, height, 0)
{
    int usernameLength = (int) strlen(username);
    
    m_username = new char[usernameLength];
    
    std::strncpy(m_username, username, usernameLength);
    m_username[usernameLength] = '\0';
    
    m_fTimeToNextRound = 0;
    m_iPreGamePhase = DEFAULT;
    m_renderTimeToNextRound = false;
    m_renderPlayersList = false;
    m_renderMessage = true;
}

WaitingForServerInterface::~WaitingForServerInterface()
{
    delete m_username;
    m_username = NULL;
    
    for(char *playerName : m_playerNames)
    {
        delete playerName;
    }
    m_playerNames.clear();
    
    m_playerPlatforms.clear();
}

void WaitingForServerInterface::update(float deltaTime, Game_State gameState)
{
    m_renderPlayersList = gameState == WAITING_FOR_SERVER;
    
    if(m_fTimeToNextRound > 0)
    {
        m_fTimeToNextRound -= deltaTime;
    }
    
    m_renderTimeToNextRound = (int)m_fTimeToNextRound > 0;
}

void WaitingForServerInterface::handlePreGameServerUpdate(rapidjson::Document &d)
{
    static const char *timeToNextRoundKey = "timeToNextRound";
    
    if(d.HasMember(timeToNextRoundKey))
    {
        m_fTimeToNextRound = d[timeToNextRoundKey].GetInt();
    }
    
    static const char *numPlayersKey = "numPlayers";
    
    if(d.HasMember(numPlayersKey))
    {
        int numPlayers = d[numPlayersKey].GetInt();
        
        if(numPlayers > 0)
        {
            static const char *playerIndex0Key = "playerIndex0";
            static const char *playerIndex1Key = "playerIndex1";
            static const char *playerIndex2Key = "playerIndex2";
            static const char *playerIndex3Key = "playerIndex3";
            static const char *playerIndex4Key = "playerIndex4";
            static const char *playerIndex5Key = "playerIndex5";
            static const char *playerIndex6Key = "playerIndex6";
            static const char *playerIndex7Key = "playerIndex7";
            
            static const char *playerIndex0PlatformKey = "playerIndex0Platform";
            static const char *playerIndex1PlatformKey = "playerIndex1Platform";
            static const char *playerIndex2PlatformKey = "playerIndex2Platform";
            static const char *playerIndex3PlatformKey = "playerIndex3Platform";
            static const char *playerIndex4PlatformKey = "playerIndex4Platform";
            static const char *playerIndex5PlatformKey = "playerIndex5Platform";
            static const char *playerIndex6PlatformKey = "playerIndex6Platform";
            static const char *playerIndex7PlatformKey = "playerIndex7Platform";
            
            m_playerNames.clear();
            
            handlePlayerNameAndPlatform(d, playerIndex0Key, playerIndex0PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex1Key, playerIndex1PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex2Key, playerIndex2PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex3Key, playerIndex3PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex4Key, playerIndex4PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex5Key, playerIndex5PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex6Key, playerIndex6PlatformKey);
            handlePlayerNameAndPlatform(d, playerIndex7Key, playerIndex7PlatformKey);
        }
    }
}

char * WaitingForServerInterface::getUsername()
{
    return m_username;
}

std::vector<char *> & WaitingForServerInterface::getPlayerNames()
{
    return m_playerNames;
}

std::vector<int> & WaitingForServerInterface::getPlayerPlatforms()
{
    return m_playerPlatforms;
}

int WaitingForServerInterface::getTimeToNextRound()
{
    return m_fTimeToNextRound;
}

void WaitingForServerInterface::setPreGamePhase(int preGamePhase)
{
    m_iPreGamePhase = preGamePhase;
}

int WaitingForServerInterface::getPreGamePhase()
{
    return m_iPreGamePhase;
}

bool WaitingForServerInterface::renderTimeToNextRound()
{
    return m_renderTimeToNextRound;
}

bool WaitingForServerInterface::renderPlayersList()
{
    return m_renderPlayersList;
}

bool WaitingForServerInterface::renderMessage()
{
    return m_renderMessage;
}

#pragma mark <Private>

void WaitingForServerInterface::handlePlayerNameAndPlatform(rapidjson::Document &d, const char *keyName, const char *keyPlatform)
{
    if(d.HasMember(keyName) && d.HasMember(keyPlatform))
    {
        const char *username = d[keyName].GetString();
        int usernameLength = (int) strlen(username);
        
        char *newUsername = new char[usernameLength];
        
        std::strncpy(newUsername, username, usernameLength);
        newUsername[usernameLength] = '\0';
        
        m_playerNames.push_back(newUsername);
        
        int playerPlatform = d[keyPlatform].GetInt();
        m_playerPlatforms.push_back(playerPlatform);
    }
}
