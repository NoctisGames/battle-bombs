//
//  GameStateWaitingForServer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateWaitingForServer.h"
#include "GameScreen.h"
#include "WaitingForServerInterface.h"
#include "Renderer.h"
#include "GameEvent.h"

void GameStateWaitingForServer::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    updateTimers(gameScreen, deltaTime);
    
    gameScreen->getWaitingForServerInterface()->update(deltaTime);
}

void GameStateWaitingForServer::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    switch (eventType)
    {
        case PRE_GAME_SERVER_UPDATE:
            gameScreen->getWaitingForServerInterface()->handlePreGameServerUpdate(d);
            break;
        case BEGIN_SPECTATE:
            gameScreen->beginSpectate(d);
            break;
        case BEGIN_GAME:
            gameScreen->beginGame(d);
            break;
    }
}

void GameStateWaitingForServer::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    // TODO
}

void GameStateWaitingForServer::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWaitingForServerInterface(*gameScreen->getWaitingForServerInterface(), true, false);
    
    gameScreen->getRenderer()->endFrame();
}