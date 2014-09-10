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
#include "PlayerRow.h"

WaitingForServerInterface::WaitingForServerInterface(float x, float y, float width, float height, const char *username) : GameObject(x, y, width, height, 0)
{
    int usernameLength = (int) strlen(username);
    
    m_username = new char[usernameLength];
    
    std::strncpy(m_username, username, usernameLength);
    m_username[usernameLength] = '\0';
    
    float playerRowWidth = width * 0.9f;
    float playerRowHeight = height / 8 * 0.8f;
    float playerRowHeightHalf = playerRowHeight / 2;
    float playerRowMargin = height / 8 * 0.06f;
    float playersListBottomY = y - height / 2 + playerRowMargin;
    float playerRowHeightEighth = height / 8;
    
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 7, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 6, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 5, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 4, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 3, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 2, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 1, playerRowWidth, playerRowHeight)));
    m_playerRows.push_back(std::unique_ptr<PlayerRow>(new PlayerRow(x, playersListBottomY + playerRowHeightHalf + playerRowHeightEighth * 0, playerRowWidth, playerRowHeight)));
    
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
            
            int playerIndex = 0;
            if(d.HasMember(playerIndex0Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex0Key, playerIndex0PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex1Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex1Key, playerIndex1PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex2Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex2Key, playerIndex2PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex3Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex3Key, playerIndex3PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex4Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex4Key, playerIndex4PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex5Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex5Key, playerIndex5PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex6Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex6Key, playerIndex6PlatformKey, playerIndex);
                playerIndex++;
            }
            
            if(d.HasMember(playerIndex7Key))
            {
                m_playerRows.at(playerIndex)->handlePlayerNameAndPlatform(d, playerIndex7Key, playerIndex7PlatformKey, playerIndex);
                playerIndex++;
            }
            
            for(; playerIndex < 8; playerIndex++)
            {
                m_playerRows.at(playerIndex)->reset();
            }
        }
    }
}

char * WaitingForServerInterface::getUsername()
{
    return m_username;
}

std::vector<std::unique_ptr<PlayerRow >> & WaitingForServerInterface::getPlayerRows()
{
    return m_playerRows;
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
