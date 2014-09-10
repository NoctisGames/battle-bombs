//
//  PlayerRowPlatformAvatar.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__PlayerRowPlatformAvatar__
#define __battlebombs__PlayerRowPlatformAvatar__

#include "GameObject.h"

class PlayerRowPlatformAvatar : public GameObject
{
public:
	PlayerRowPlatformAvatar(float x, float y, float width, float height);
    
    void setPlayerPlatform(int playerPlatform);
    
    int getPlayerPlatform();
    
private:
    int m_iPlayerPlatform;
};

#endif /* defined(__battlebombs__PlayerRowPlatformAvatar__) */
