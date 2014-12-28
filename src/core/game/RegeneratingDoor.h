//
//  RegeneratingDoor.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__RegeneratingDoor__
#define __battlebombs__RegeneratingDoor__

#include "GridGameObject.h"

class RegeneratingDoor : public GridGameObject
{
public:
    RegeneratingDoor(int gridX, int gridY);
    
    void update(float deltaTime);
    
    void onDestroy();
    
    float getStateTime();
    
private:
    float m_fStateTime;
};

#endif /* defined(__battlebombs__RegeneratingDoor__) */