//
//  GameStateSpectatingOffline.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__GameStateSpectatingOffline__
#define __battlebombs__GameStateSpectatingOffline__

#include "GameStateSpectating.h"

class GameStateSpectatingOffline : public GameStateSpectating
{
public:
    virtual void processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType);
    
protected:
    virtual void updateTimers(GameScreen *gameScreen, float deltaTime);
    
    virtual void setGameState(GameScreen *gameScreen, int gameState);
};

#endif /* defined(__battlebombs__GameStateSpectatingOffline__) */