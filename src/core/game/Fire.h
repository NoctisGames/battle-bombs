//
//  Fire.h
//  battlebombs
//
//  Created by Stephen Gowen on 6/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__Fire__
#define __battlebombs__Fire__

#include "GridGameObject.h"
#include "GameConstants.h"
#include "FireType.h"

class Fire : public GridGameObject
{
public:
    Fire(Fire_Type type, int gridX, int gridY, int direction, float width = GRID_CELL_WIDTH, float height = GRID_CELL_HEIGHT);
    
    void nextFrame(short explosionPowerRemaining);
    
    int getDirection();
    
    bool isExhausted();
    
    bool isDeadly();
    
    bool isEdge();
    
    Fire_Type getFireType();
    
private:
    Fire_Type m_fireType;
    int m_iDirection;
    bool m_isExhausted;
};

#endif /* defined(__battlebombs__Fire__) */