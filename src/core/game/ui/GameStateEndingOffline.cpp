//
//  GameStateEndingOffline.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateEndingOffline.h"
#include "GameScreen.h"
#include "GameStateFactory.h"

void GameStateEndingOffline::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // Ignore
}

void GameStateEndingOffline::updateTimers(GameScreen *gameScreen, float deltaTime)
{
    // Ignore
}

void GameStateEndingOffline::setGameState(GameScreen *gameScreen, int gameState)
{
    gameScreen->setGameState(GameStateFactory::getInstance().createGameState(gameState, true));
}