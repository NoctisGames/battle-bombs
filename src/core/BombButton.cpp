//
//  BombButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 7/11/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "BombButton.h"
#include "Vector2D.h"
#include "Rectangle.h"

BombButton::BombButton(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = DISABLED;
    m_isPressed = false;
}

void BombButton::setButtonState(Button_State buttonState)
{
    m_buttonState = buttonState;
}

Button_State BombButton::getButtonState()
{
    return m_buttonState;
}

void BombButton::setIsPressed(bool isPressed)
{
    m_isPressed = isPressed;
}

bool BombButton::isPressed()
{
    return m_isPressed;
}
