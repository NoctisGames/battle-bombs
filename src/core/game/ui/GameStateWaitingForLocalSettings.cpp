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
#include "WaitingForLocalSettingsInterface.h"

void GameStateWaitingForLocalSettings::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // No Server Messages in Offline Mode
}

void GameStateWaitingForLocalSettings::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    gameScreen->getWaitingForLocalSettingsInterface()->updateInput(gameScreen, touchEvents);
    
    if(gameScreen->getWaitingForLocalSettingsInterface()->isStartingGame())
    {
        int map = gameScreen->getWaitingForLocalSettingsInterface()->getChosenMapType();
        int chosenBotFlags = gameScreen->getWaitingForLocalSettingsInterface()->getChosenBotFlags();
        int chosenPowerUpFlags = gameScreen->getWaitingForLocalSettingsInterface()->getChosenPowerUpFlags();
        
        gameScreen->beginGameOffline(map, 1, 180, chosenBotFlags, chosenPowerUpFlags);
    }
}

void GameStateWaitingForLocalSettings::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWaitingForLocalSettingsInterface(*gameScreen->getWaitingForLocalSettingsInterface());
    
    gameScreen->getRenderer()->endFrame();
}