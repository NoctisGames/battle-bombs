//
//  GameStateConnectionErrorWaitingForInput.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateConnectionErrorWaitingForInput.h"
#include "GameScreen.h"
#include "Renderer.h"
#include "ScreenState.h"
#include "TouchEvent.h"

void GameStateConnectionErrorWaitingForInput::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    // Nothing to do
}

void GameStateConnectionErrorWaitingForInput::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // We are only waiting for the user to click the screen so that we can exit, so don't process any more messages
}

void GameStateConnectionErrorWaitingForInput::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
    {
        gameScreen->setScreenState(SCREEN_STATE_EXIT);
    }
}

void GameStateConnectionErrorWaitingForInput::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWaitingForServerInterface(*gameScreen->getWaitingForServerInterface(), false, true);
    
    gameScreen->getRenderer()->endFrame();
}