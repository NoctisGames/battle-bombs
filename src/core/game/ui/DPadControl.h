//
//  DPadControl.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/9/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__DPadControl__
#define __battlebombs__DPadControl__

#include "GameObject.h"
#include "DPadControlState.h"

class Triangle;

class DPadControl : public GameObject
{
public:
    DPadControl(float x, float y, float width, float height);
    
    int getDirectionForTouchPoint(Vector2D &touchPoint);
    
    void stop();
    
    DPad_Control_State getState();

private:
    std::unique_ptr<Triangle> m_rightBounds;
    std::unique_ptr<Triangle> m_upBounds;
    std::unique_ptr<Triangle> m_leftBounds;
    std::unique_ptr<Triangle> m_downBounds;
    DPad_Control_State m_state;
};

#endif /* defined(__battlebombs__DPadControl__) */