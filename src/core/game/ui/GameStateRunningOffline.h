//
//  GameStateRunningOffline.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__GameStateRunningOffline__
#define __battlebombs__GameStateRunningOffline__

#include "GameStateRunning.h"

class GameStateRunningOffline : public GameStateRunning
{
public:
    virtual void processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType);
    
protected:
    virtual void updateTimers(GameScreen *gameScreen, float deltaTime);
    
    virtual void setGameState(GameScreen *gameScreen, int gameState);
};

#endif /* defined(__battlebombs__GameStateRunningOffline__) */