//
//  GameStateRunningOffline.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateRunningOffline.h"
#include "GameScreen.h"
#include "GameStateFactory.h"

void GameStateRunningOffline::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // Ignore
}

void GameStateRunningOffline::updateTimers(GameScreen *gameScreen, float deltaTime)
{
    // Ignore
}

void GameStateRunningOffline::setGameState(GameScreen *gameScreen, int gameState)
{
    gameScreen->setGameState(GameStateFactory::getInstance().createGameState(gameState, true));
}