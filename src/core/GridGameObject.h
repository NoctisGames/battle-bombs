//
//  GridGameObject.h
//  bomberparty
//
//  Created by Stephen Gowen on 5/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__GridGameObject__
#define __bomberparty__GridGameObject__

#include "GameObject.h"

class GridGameObject : public GameObject
{
protected:
    int m_gridX;
    int m_gridY;
    
public:
    GridGameObject(int gridX, int gridY, float width, float height, float angle);
    
    int getGridX();
    
    int getGridY();
};

#endif /* defined(__bomberparty__GridGameObject__) */