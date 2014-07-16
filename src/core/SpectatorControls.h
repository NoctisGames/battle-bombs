//
//  SpectatorControls.h
//  bomberparty
//
//  Created by Stephen Gowen on 7/16/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__SpectatorControls__
#define __bomberparty__SpectatorControls__

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

#endif /* defined(__bomberparty__SpectatorControls__) */
