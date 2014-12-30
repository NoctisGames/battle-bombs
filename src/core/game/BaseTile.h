//
//  BaseTile.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__BaseTile__
#define __battlebombs__BaseTile__

#include "GridGameObject.h"
#include "BaseTileState.h"

#include <vector>
#include <memory>

class GameListener;
class PlayerDynamicGameObject;
class BombGameObject;
class BreakableBlock;
class PowerUp;
class RegeneratingDoor;

class BaseTile : public GridGameObject
{
public:
    BaseTile(int gridX, int gridY, int index, GameListener *gameListener);
    
    void update(float deltaTime, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject>> &bombs, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<RegeneratingDoor>> &doors, std::vector<std::unique_ptr<PowerUp>> &powerUps);
    
    float getStateTime();
    
    Base_Tile_State getState();
    
    void handleTimeSinceSuddenDeathModeBegan(float timeSinceSuddenDeath);
    
private:
    GameListener *m_gameListener;
    Base_Tile_State m_state;
    float m_fStateTime;
    float m_fTimeUntilActivation;
};

#endif /* defined(__battlebombs__BaseTile__) */