//
//  GameStateWaitingForConnection.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateWaitingForConnection.h"
#include "GameScreen.h"
#include "Renderer.h"
#include "GameEvent.h"
#include "WaitingForServerInterface.h"

void GameStateWaitingForConnection::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    // Nothing to do here
}

void GameStateWaitingForConnection::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    if (eventType == PRE_GAME)
    {
        static const char *phaseKey = "phase";
        
        if(d.HasMember(phaseKey))
        {
            int phase = d[phaseKey].GetInt();
            gameScreen->getWaitingForServerInterface()->setPreGamePhase(phase);
            
            if(phase == ROOM_JOINED_WAITING_FOR_SERVER)
            {
                setGameState(gameScreen, GAME_STATE_WAITING_FOR_SERVER);
            }
            else if (phase == CONNECTION_ERROR)
            {
                setGameState(gameScreen, GAME_STATE_CONNECTION_ERROR_WAITING_FOR_INPUT);
            }
        }
    }
}

void GameStateWaitingForConnection::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    // TODO
}

void GameStateWaitingForConnection::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWaitingForServerInterface(*gameScreen->getWaitingForServerInterface(), false, true);
    
    gameScreen->getRenderer()->endFrame();
}