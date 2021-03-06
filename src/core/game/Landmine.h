//
//  Landmine.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__Landmine__
#define __battlebombs__Landmine__

#include "GridGameObject.h"
#include "LandmineState.h"

class Landmine : public GridGameObject
{
public:
    Landmine(int gridX, int gridY);
    
    void update(float deltaTime);
    
    void trigger();
    
    Landmine_State getState();
    
    float getStateTime();
    
private:
    Landmine_State m_state;
    float m_fStateTime;
};

#endif /* defined(__battlebombs__Landmine__) */