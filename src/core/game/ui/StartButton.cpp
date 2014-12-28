//
//  StartButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "StartButton.h"
#include "Rectangle.h"
#include "Vector2D.h"

StartButton::StartButton(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = ENABLED;
}

void StartButton::setButtonState(Button_State buttonState)
{
    m_buttonState = buttonState;
}

Button_State StartButton::getButtonState()
{
    return m_buttonState;
}