//
//  PlayerRowAvatar.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__PlayerRowAvatar__
#define __battlebombs__PlayerRowAvatar__

#include "GameObject.h"

class PlayerRowAvatar : public GameObject
{
public:
	PlayerRowAvatar(float x, float y, float width, float height, int playerIndex);
    
    void setPlayerIndex(int playerIndex);
    
    int getPlayerIndex();
    
    void setIsBot(bool isBot);
    
    bool isBot();
    
private:
    int m_iPlayerIndex;
    bool m_isBot;
};

#endif /* defined(__battlebombs__PlayerRowAvatar__) */
