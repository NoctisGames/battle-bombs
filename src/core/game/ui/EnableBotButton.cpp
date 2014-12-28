//
//  EnableBotButton.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "EnableBotButton.h"
#include "BotDifficulty.h"

EnableBotButton::EnableBotButton(Button_State buttonState, bool isFurthestRight, float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = buttonState;
    m_iDifficulty = BOT_DIFFICULTY_NORMAL;
    m_isFurthestRight = isFurthestRight;
}

void EnableBotButton::toggle()
{
    m_iDifficulty++;
    if(m_iDifficulty > BOT_DIFFICULTY_HARD)
    {
        m_iDifficulty = BOT_DIFFICULTY_EASY;
        
        if(m_isFurthestRight)
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

void EnableBotButton::setIsFurthestRight(bool isFurthestRight)
{
    m_isFurthestRight = isFurthestRight;
}

bool EnableBotButton::isFurthestRight()
{
    return m_isFurthestRight;
}