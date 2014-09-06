//
//  WaitingForServerInterface.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "WaitingForServerInterface.h"
#include "Vector2D.h"
#include "Rectangle.h"

WaitingForServerInterface::WaitingForServerInterface(float x, float y, float width, float height, float timeToNextRound) : GameObject(x, y, width, height, 0)
{
    m_fTimeToNextRound = timeToNextRound;
}

void WaitingForServerInterface::update(float deltaTime)
{
    if(m_fTimeToNextRound > 0)
    {
        m_fTimeToNextRound -= deltaTime;
    }
}

void WaitingForServerInterface::setTimeToNextRound(float timeToNextRound)
{
    m_fTimeToNextRound = timeToNextRound;
}

int WaitingForServerInterface::getTimeToNextRound()
{
    return m_fTimeToNextRound;
}
