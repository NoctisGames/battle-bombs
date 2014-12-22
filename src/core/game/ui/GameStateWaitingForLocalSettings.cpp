//
//  GameStateWaitingForLocalSettings.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateWaitingForLocalSettings.h"
#include "GameScreen.h"
#include "WaitingForLocalSettingsInterface.h"
#include "Renderer.h"
#include "ScreenState.h"
#include "TouchEvent.h"
#include "GameEvent.h"

//For RNG purposes
#include <stdlib.h>
#include <time.h>

void GameStateWaitingForLocalSettings::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    gameScreen->getWaitingForLocalSettingsInterface()->update(deltaTime);
}

void GameStateWaitingForLocalSettings::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // No Server Messages in Offline Mode
}

void GameStateWaitingForLocalSettings::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    if(touchEvents.size() > 0)
    {
        srand((int) time(NULL));
        
        int map = (rand() % NUM_MAPS);
        
        gameScreen->beginGameOffline(map, 1, 180);
    }
}

void GameStateWaitingForLocalSettings::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWaitingForLocalSettingsInterface(*gameScreen->getWaitingForLocalSettingsInterface());
    
    gameScreen->getRenderer()->endFrame();
}