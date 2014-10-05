//
//  IcePatch.h
//  battlebombs
//
//  Created by Stephen Gowen on 10/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__IcePatch__
#define __battlebombs__IcePatch__

#include "GridGameObject.h"

#include <memory>
#include <vector>

class InsideBlock;
class BreakableBlock;

class IcePatch : public GridGameObject
{
public:
    IcePatch(int gridX, int gridY);
    
    void update(float deltaTime);
    
    float getStateTime();
    
private:
    float m_fStateTime;
};

#endif /* defined(__battlebombs__IcePatch__) */
