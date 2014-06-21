//
//  Fire.h
//  bomberparty
//
//  Created by Stephen Gowen on 6/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__Fire__
#define __bomberparty__Fire__

#include "GridGameObject.h"
#include "GameConstants.h"
#include "FireType.h"

class Fire : public GridGameObject
{
public:
    Fire(Fire_Type type, int gridX, int gridY, float angle, float width = GRID_CELL_WIDTH, float height = GRID_CELL_HEIGHT);
    
    void nextFrame(short explosionPowerRemaining);
    
    bool isExhausted();
    
    Fire_Type getFireType();
    
private:
    Fire_Type m_fireType;
    bool m_isExhausted;
};

#endif /* defined(__bomberparty__Fire__) */