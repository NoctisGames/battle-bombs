//
//  EnablePowerUpButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "EnablePowerUpButton.h"

EnablePowerUpButton::EnablePowerUpButton(int powerUpType, float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = ENABLED;
    m_powerUpType = powerUpType;
}

void EnablePowerUpButton::toggle()
{
    if(m_buttonState == ENABLED)
    {
        m_buttonState = DISABLED;
    }
    else
    {
        m_buttonState = ENABLED;
    }
}

void EnablePowerUpButton::setButtonState(Button_State buttonState)
{
    m_buttonState = buttonState;
}

Button_State EnablePowerUpButton::getButtonState()
{
    return m_buttonState;
}

void EnablePowerUpButton::setPowerUpType(int powerUpType)
{
    m_powerUpType = powerUpType;
}

int EnablePowerUpButton::getPowerUpType()
{
    return m_powerUpType;
}