//
//  RemoteBomb.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__RemoteBomb__
#define __battlebombs__RemoteBomb__

#include "BombGameObject.h"
#include "RemoteBombState.h"

class RemoteBomb : public BombGameObject
{
public:
    RemoteBomb(PlayerDynamicGameObject *bombOwner, short power, int gridX, int gridY);
    
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<RegeneratingDoor>> &doors, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    void detonate();
    
    Remote_Bomb_State getState();
    
private:
    Remote_Bomb_State m_state;
};

#endif /* defined(__battlebombs__RemoteBomb__) */