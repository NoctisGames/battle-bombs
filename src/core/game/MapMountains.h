//
//  MapMountains.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__MapMountains__
#define __battlebombs__MapMountains__

#include "Map.h"

class MapMountains : public Map
{
public:
    virtual bool updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player);
    
    virtual short getMusicId();
    
    virtual void handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan);
    
    virtual void suddenDeath(GameSession *gameSession);
    
    virtual void initializeMap(GameSession *gameSession);
    
    virtual void initializeGameGrid(GameSession *gameSession, int (&gameGrid)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]);
    
    virtual void update(GameSession *gameSession, float deltaTime, bool isSuddenDeath);
};

#endif /* defined(__battlebombs__MapMountains__) */