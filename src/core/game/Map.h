//
//  Map.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
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
    
    virtual void update(GameSession *gameSession, float deltaTime, bool isSuddenDeath);
    
    virtual bool isValidLocationForBreakableBlock(GameSession *gameSession, int j, int i);
    
    // Called by the Server or the Client in Offline Mode
    void populateMapWithPlayersAndBreakableBlocks(GameSession *gameSession, int numHumanPlayers, int chosenBotFlags, int chosenPowerUpFlags);
    
protected:
    void addCommonInsideBlocks(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks);
    
private:
    bool isLocationOccupiedByDynamicGridGameObject(GameSession *gameSession, int gridX, int gridY);
    
    // A utility function to swap to integers
    void swap(int *a, int *b);
    
    // A function to generate a random permutation of arr[]
    void randomize(int arr[][2], int n);
};

#endif /* defined(__battlebombs__Map__) */