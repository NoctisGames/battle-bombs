//
//  SpaceTile.h
//  battlebombs
//
//  Created by Stephen Gowen on 10/8/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__SpaceTile__
#define __battlebombs__SpaceTile__

#include "DynamicGridGameObject.h"
#include "SpaceTileState.h"

#include <vector>
#include <memory>

class GameListener;
class PlayerDynamicGameObject;
class BombGameObject;
class InsideBlock;
class BreakableBlock;
class PowerUp;

class SpaceTile : public DynamicGridGameObject
{
public:
    SpaceTile(int gridX, int gridY, int index, GameListener *gameListener);
    
    void update(float deltaTime, bool isSuddenDeath, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject>> &bombs, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);
    
    void setFallingPlayer(PlayerDynamicGameObject *fallingPlayer);
    
    Space_Tile_State getSpaceTileState();
    
    float getStateTime();
    
    bool shouldPlayerStartFalling();
    
    bool isRemove();
    
private:
    GameListener *m_gameListener;
    PlayerDynamicGameObject *m_fallingPlayer;
    Space_Tile_State m_spaceTileState;
    float m_fStateTime;
    float m_fOrigY;
    float m_fTimeUntilDislodging;
    bool m_shouldPlayerStartFalling;
};

#endif /* defined(__battlebombs__SpaceTile__) */
