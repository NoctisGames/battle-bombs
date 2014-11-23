//
//  SpectatorControls.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/16/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__SpectatorControls__
#define __battlebombs__SpectatorControls__

#include "GameObject.h"
#include "SpectatorControlState.h"

class SpectatorControls : public GameObject
{
public:
    SpectatorControls(float x, float y, float width, float height);
    
    bool didUpdateForTouchPoint(Vector2D &touchPoint);
    
    Spectator_Control_State didMakeSelectionForTouchPoint(Vector2D &touchPoint);
    
    Spectator_Control_State getState();
    
private:
    Spectator_Control_State m_state;
};

#endif /* defined(__battlebombs__SpectatorControls__) */
