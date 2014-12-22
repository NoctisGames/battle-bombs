//
//  GameState.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameState.h"
#include "GameScreen.h"
#include "GameListener.h"
#include "GameEvent.h"
#include "ScreenState.h"
#include "GameStateFactory.h"

void GameState::update(GameScreen *gameScreen, float deltaTime)
{
    processServerMessages(gameScreen);
}

void GameState::processServerMessages(GameScreen *gameScreen)
{
    std::vector<const char *> serverMessages = gameScreen->getGameListener()->freeServerMessages();
    
    for (std::vector<const char *>::iterator itr = serverMessages.begin(); itr != serverMessages.end(); itr++)
    {
        rapidjson::Document d;
        d.Parse<0>(*itr);
        if(d.IsObject())
        {
            static const char *eventTypeKey = "eventType";
            
            if(d.HasMember(eventTypeKey))
            {
                int eventType = d[eventTypeKey].GetInt();
                
                processServerMessage(gameScreen, d, eventType);
            }
        }
    }
}

bool GameState::shouldUpdateGameInterface()
{
    return false;
}

bool GameState::shouldUpdateSpectatorInterface()
{
    return false;
}

bool GameState::shouldHandlePlayerDataUpdate(GameScreen *gameScreen, short playerIndex, bool isBeginGame)
{
    return isBeginGame || playerIndex != gameScreen->getPlayerIndex();
}

#pragma mark <Protected>

void GameState::updateTimers(GameScreen *gameScreen, float deltaTime)
{
    gameScreen->setTimeSinceLastClientEvent(gameScreen->getTimeSinceLastClientEvent() + deltaTime);
    
    gameScreen->setTimeSinceLastServerUpdate(gameScreen->getTimeSinceLastServerUpdate() + deltaTime);
    
    if(gameScreen->getTimeSinceLastServerUpdate() > 8)
    {
        gameScreen->setScreenState(SCREEN_STATE_CONNECTION_ERROR);
    }
}

void GameState::setGameState(GameScreen *gameScreen, int gameState)
{
    gameScreen->setGameState(GameStateFactory::getInstance().createGameState(gameState, false));
}