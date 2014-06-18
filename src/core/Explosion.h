//
//  Explosion.h
//  bomberparty
//
//  Created by Stephen Gowen on 4/30/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__Explosion__
#define __bomberparty__Explosion__

#include "pch.h"
#include "GameObject.h"
#include "ExplosionType.h"
#include "GameConstants.h"

class Explosion : public GameObject
{
public:
    Explosion(Explosion_Type explosionType, float x, float y, float angle, float width = GRID_CELL_WIDTH, float height = GRID_CELL_HEIGHT);
    
    void update(float deltaTime);
    
    float getStateTime();
    
    Explosion_Type getExplosionType();
    
    float isComplete();
    
private:
    Explosion_Type m_explosionType;
    float m_fStateTime;
    bool m_isComplete;
};

#endif /* defined(__bomberparty__Explosion__) */
