//
//  GameStateCountingDownOffline.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__GameStateCountingDownOffline__
#define __battlebombs__GameStateCountingDownOffline__

#include "GameStateCountingDown.h"

class GameStateCountingDownOffline : public GameStateCountingDown
{
public:
    virtual void processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType);
    
protected:
    virtual void updateTimers(GameScreen *gameScreen, float deltaTime);
    
    virtual void setGameState(GameScreen *gameScreen, int gameState);
};

#endif /* defined(__battlebombs__GameStateCountingDownOffline__) */