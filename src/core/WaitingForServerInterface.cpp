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

void WaitingForServerInterface::update(float deltaTime, Game_State gameState)
{
    m_renderPlayersList = gameState == WAITING_FOR_SERVER;
    
    if(m_fTimeToNextRound > 0)
    {
        m_fTimeToNextRound -= deltaTime;
    }
}

char * WaitingForServerInterface::getUsername()
{
    return m_username;
}

void WaitingForServerInterface::setTimeToNextRound(float timeToNextRound)
{
    m_fTimeToNextRound = timeToNextRound;
    m_renderTimeToNextRound = m_fTimeToNextRound > 0;
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
