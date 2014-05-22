//
//  DPadControl.h
//  bomberparty
//
//  Created by Stephen Gowen on 3/9/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__DPadControl__
#define __bomberparty__DPadControl__

#include "GameObject.h"
#include "Triangle.h"

class DPadControl : public GameObject
{
public:
    DPadControl(float x, float y, float width, float height);
    
    int getDirectionForTouchPoint(Vector2D &touchPoint);

private:
    std::unique_ptr<Triangle> m_rightBounds;
    std::unique_ptr<Triangle> m_upBounds;
    std::unique_ptr<Triangle> m_leftBounds;
    std::unique_ptr<Triangle> m_downBounds;
};

#endif /* defined(__bomberparty__DPadControl__) */