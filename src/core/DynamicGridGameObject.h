//
//  DynamicGridGameObject.h
//  bomberparty
//
//  Created by Stephen Gowen on 5/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__DynamicGridGameObject__
#define __bomberparty__DynamicGridGameObject__

#include "DynamicGameObject.h"

class DynamicGridGameObject : public DynamicGameObject
{
protected:
    int m_gridX;
    int m_gridY;
    
public:
    DynamicGridGameObject(int gridX, int gridY, float width, float height, float angle);
    
    virtual Rectangle & getBoundsForGridLogic();
    
    void updateGrid();
    
    int getGridX();
    
    int getGridY();
};

#endif /* defined(__bomberparty__DynamicGridGameObject__) */