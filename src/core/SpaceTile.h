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

class PlayerDynamicGameObject;

class SpaceTile : public DynamicGridGameObject
{
public:
    SpaceTile(int gridX, int gridY);
    
    void update(float deltaTime, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    Space_Tile_State getSpaceTileState();
    
    float getStateTime();
    
private:
    PlayerDynamicGameObject *m_fallingPlayer;
    Space_Tile_State m_spaceTileState;
    float m_fStateTime;
    float m_fOrigY;
};

#endif /* defined(__battlebombs__SpaceTile__) */
