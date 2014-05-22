//
//  BombGameObject.h
//  bomberparty
//
//  Created by Stephen Gowen on 4/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__BombGameObject__
#define __bomberparty__BombGameObject__

#include "pch.h"
#include "DynamicGridGameObject.h"

#include <vector>
#include <memory>

class PlayerDynamicGameObject;
class Explosion;
class InsideBlock;
class BreakableBlock;

class BombGameObject : public DynamicGridGameObject
{
public:
    BombGameObject(PlayerDynamicGameObject *bombOwner, short power, int gridX, int gridY, float width = 0.8f, float height = 0.8f);
    
    void update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    float getStateTime();
    
    float isExploding();
    
private:
    PlayerDynamicGameObject *m_bombOwner;
    float m_fStateTime;
    float m_fSizeScalar;
    short m_sPower;
    bool m_isExploding;
    
    bool canExplodeAtPosition(Vector2D &position, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks);
    
    bool willDestroyBlockAtPosition(Vector2D &position, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
};

#endif /* defined(__bomberparty__BombGameObject__) */
