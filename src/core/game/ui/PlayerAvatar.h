//
//  PlayerAvatar.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/14/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__PlayerAvatar__
#define __battlebombs__PlayerAvatar__

#include "GameObject.h"
#include "PlayerState.h"

class PlayerAvatar : public GameObject
{
public:
    PlayerAvatar(float x, float y, float width, float height);
    
    void setPlayerState(Player_State playerState);
    
    Player_State getPlayerState();
    
    void setPlayerIndex(short playerIndex);
    
    short getPlayerIndex();
    
    void setIsBot(bool isBot);
    
    bool isBot();
    
private:
    Player_State m_playerState;
    short m_sPlayerIndex;
    bool m_isBot;
};

#endif /* defined(__battlebombs__PlayerAvatar__) */
