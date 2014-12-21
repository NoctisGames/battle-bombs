//
//  Map.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__Map__
#define __battlebombs__Map__

#include "GameConstants.h"
#include <vector>
#include <memory>

class PlayerDynamicGameObject;
class GameSession;
class InsideBlock;

class Map
{
public:
    // Return true if the player is going to be no longer in a valid ALIVE state
    virtual bool updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player) = 0;
    
    virtual short getMusicId() = 0;
    
    virtual void handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan) = 0;
    
    virtual void suddenDeath(GameSession *gameSession) = 0;
    
    virtual void initializeMap(GameSession *gameSession);
    
    virtual void initializeGameGrid(GameSession *gameSession, int (&gameGrid)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]);
    
    virtual void initializeMiniMap(GameSession *gameSession, int (&miniMap)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]);
    
    virtual void update(GameSession *gameSession, float deltaTime, bool isSuddenDeath);
    
    void updateMiniMap(int (&miniMap)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS], int x, int y, int miniMapGridType);
    
protected:
    void addCommonInsideBlocks(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks);
};

#endif /* defined(__battlebombs__Map__) */