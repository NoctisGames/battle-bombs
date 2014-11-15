//
//  SpectatorControls.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 7/16/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "SpectatorControls.h"
#include "OverlapTester.h"
#include "Vector2D.h"
#include "Rectangle.h"

SpectatorControls::SpectatorControls(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_state = NOTHING_HIGHLIGHTED;
}

bool SpectatorControls::didUpdateForTouchPoint(Vector2D &touchPoint)
{
    if(OverlapTester::isPointInRectangle(touchPoint, *m_bounds))
    {
        if(touchPoint.getX() < m_position->getX())
        {
            m_state = LEFT_ARROW_HIGHLIGHTED;
        }
        else
        {
            m_state = RIGHT_ARROW_HIGHLIGHTED;
        }
        
        return true;
    }
    
    m_state = NOTHING_HIGHLIGHTED;
    
    return false;
}

Spectator_Control_State SpectatorControls::didMakeSelectionForTouchPoint(Vector2D &touchPoint)
{
    Spectator_Control_State state = m_state;
    
    m_state = NOTHING_HIGHLIGHTED;
    
    return state;
}

Spectator_Control_State SpectatorControls::getState()
{
    return m_state;
}