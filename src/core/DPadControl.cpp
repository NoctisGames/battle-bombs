//
//  DPadControl.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 3/9/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "GameConstants.h"
#include "DPadControl.h"
#include "OverlapTester.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Triangle.h"

DPadControl::DPadControl(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_rightBounds = std::unique_ptr<Triangle>(new Triangle(x, y, x + width / 2, y + height / 2, x + width / 2, y - height / 2));
    m_upBounds = std::unique_ptr<Triangle>(new Triangle(x, y, x + width / 2, y + height / 2, x - width / 2, y + height / 2));
    m_leftBounds = std::unique_ptr<Triangle>(new Triangle(x, y, x - width / 2, y + height / 2, x - width / 2, y - height / 2));
    m_downBounds = std::unique_ptr<Triangle>(new Triangle(x, y, x - width / 2, y - height / 2, x + width / 2, y - height / 2));
}

int DPadControl::getDirectionForTouchPoint(Vector2D &touchPoint)
{
    if(OverlapTester::isPointInTriangle(touchPoint, *m_rightBounds))
    {
        return DIRECTION_RIGHT;
    }
    else if(OverlapTester::isPointInTriangle(touchPoint, *m_upBounds))
    {
        return DIRECTION_UP;
    }
    else if(OverlapTester::isPointInTriangle(touchPoint, *m_leftBounds))
    {
        return DIRECTION_LEFT;
    }
    else if(OverlapTester::isPointInTriangle(touchPoint, *m_downBounds))
    {
        return DIRECTION_DOWN;
    }
    
    return -1;
}