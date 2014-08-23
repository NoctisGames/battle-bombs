//
//  ActiveButton.cpp
//  battlebombs
//
//  Created by Mikel Adkins on 5/18/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "ActiveButton.h"
#include "Vector2D.h"
#include "Rectangle.h"

ActiveButton::ActiveButton(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_powerUpType = POWER_UP_TYPE_NONE;
    m_buttonState = DISABLED;
    m_isPressed = false;
}

void ActiveButton::setPowerUpType(int powerUpType)
{
    m_powerUpType = powerUpType;
}

int ActiveButton::getPowerUpType()
{
    return m_powerUpType;
}

void ActiveButton::setButtonState(Button_State buttonState)
{
    m_buttonState = buttonState;
}

Button_State ActiveButton::getButtonState()
{
    return m_buttonState;
}

void ActiveButton::setIsPressed(bool isPressed)
{
    m_isPressed = isPressed;
}

bool ActiveButton::isPressed()
{
    return m_isPressed;
}
