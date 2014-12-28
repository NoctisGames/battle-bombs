//
//  EnableBotButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "EnableBotButton.h"
#include "BotDifficulty.h"
#include "Rectangle.h"
#include "Vector2D.h"

EnableBotButton::EnableBotButton(Button_State buttonState, bool isFurthestRightActive, bool isFurthestLeftInactive, float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = buttonState;
    m_iDifficulty = BOT_DIFFICULTY_NORMAL;
    m_isFurthestRightActive = isFurthestRightActive;
    m_isFurthestLeftInactive = isFurthestLeftInactive;
}

void EnableBotButton::toggle()
{
    if(m_isFurthestLeftInactive)
    {
        m_buttonState = ENABLED;
        m_isFurthestRightActive = true;
        m_isFurthestLeftInactive = false;
        
        return;
    }
    
    m_iDifficulty++;
    if(m_iDifficulty > BOT_DIFFICULTY_HARD)
    {
        m_iDifficulty = BOT_DIFFICULTY_EASY;
        
        if(m_isFurthestRightActive)
        {
            m_buttonState = DISABLED;
        }
    }
}

void EnableBotButton::setButtonState(Button_State buttonState)
{
    m_buttonState = buttonState;
}

Button_State EnableBotButton::getButtonState()
{
    return m_buttonState;
}

int EnableBotButton::getDifficulty()
{
    return m_iDifficulty;
}