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
    m_powerUpType = NONE;
    m_buttonState = DISABLED;
    m_isPressed = false;
}

void ActiveButton::setPowerUpType(Power_Up_Type powerUpType)
{
    m_powerUpType = powerUpType;
}

Power_Up_Type ActiveButton::getPowerUpType()
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
