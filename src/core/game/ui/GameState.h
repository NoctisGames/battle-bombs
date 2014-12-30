//
//  GameState.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__GameState__
#define __battlebombs__GameState__

#include "rapidjson/document.h"

#include <vector>

class GameScreen;
class TouchEvent;

class GameState
{
public:
    virtual void update(GameScreen *gameScreen, float deltaTime);
    
    virtual void processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType) = 0;
    
    virtual void updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents) = 0;
    
    virtual void present(GameScreen *gameScreen) = 0;
    
    virtual bool shouldUpdateGameInterface();
    
    virtual bool shouldUpdateSpectatorInterface();
    
    virtual bool shouldHandlePlayerDataUpdate(GameScreen *gameScreen, short playerIndex, bool isBeginGame);
    
    void processServerMessages(GameScreen *gameScreen);
    
protected:
    virtual void updateTimers(GameScreen *gameScreen, float deltaTime);
    
    virtual void setGameState(GameScreen *gameScreen, int gameState);
};

#endif /* defined(__battlebombs__GameState__) */