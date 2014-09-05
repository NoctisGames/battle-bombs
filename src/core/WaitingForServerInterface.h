//
//  WaitingForServerInterface.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__WaitingForServerInterface__
#define __battlebombs__WaitingForServerInterface__

#include "GameObject.h"

class WaitingForServerInterface : public GameObject
{
public:
	WaitingForServerInterface(float x, float y, float width, float height);
    
    void update(float deltaTime);
};

#endif /* defined(__battlebombs__WaitingForServerInterface__) */
