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

class Landmine : public GridGameObject
{
public:
    Landmine(int gridX, int gridY);
    
    void update(float deltaTime);
    
private:
    float m_fStateTime;
};

#endif /* defined(__battlebombs__Landmine__) */