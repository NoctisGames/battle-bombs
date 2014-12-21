//
//  MapBase.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__MapBase__
#define __battlebombs__MapBase__

#include "Map.h"

class MapBase : public Map
{
public:
    virtual bool updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player);
    
    virtual short getMusicId();
    
    virtual void handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan);
    
    virtual void suddenDeath(GameSession *gameSession);
    
    virtual void initializeMap(GameSession *gameSession);
    
    virtual void update(GameSession *gameSession, float deltaTime, bool isSuddenDeath);
};

#endif /* defined(__battlebombs__MapBase__) */