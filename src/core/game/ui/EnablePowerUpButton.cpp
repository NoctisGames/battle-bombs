//
//  EnablePowerUpButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "EnablePowerUpButton.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "ChosenPowerUpFlags.h"

#include <vector>

EnablePowerUpButton::EnablePowerUpButton(int chosenPowerUpFlag, float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = ENABLED;
    m_iChosenPowerUpFlag = chosenPowerUpFlag;
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

int EnablePowerUpButton::getChosenPowerUpFlag()
{
    return m_iChosenPowerUpFlag;
}

Button_State EnablePowerUpButton::getButtonState()
{
    return m_buttonState;
}