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
#include "ChosenBotFlags.h"

#include <vector>

int furthestRightActiveIndex = 0;

EnableBotButton::EnableBotButton(Button_State buttonState, int index, bool isFurthestRightActive, float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_buttonState = buttonState;
    m_iIndex = index;
    m_iDifficulty = BOT_DIFFICULTY_NORMAL;
    
    if(isFurthestRightActive)
    {
        furthestRightActiveIndex = index;
    }
}

void EnableBotButton::toggle()
{
    if(m_iIndex == furthestRightActiveIndex + 1)
    {
        m_buttonState = ENABLED;
        furthestRightActiveIndex = m_iIndex;
        
        return;
    }
    else if(m_buttonState == ENABLED)
    {
        m_iDifficulty++;
        if(m_iDifficulty > BOT_DIFFICULTY_HARD)
        {
            m_iDifficulty = BOT_DIFFICULTY_EASY;
            
            if(m_iIndex == furthestRightActiveIndex)
            {
                m_buttonState = DISABLED;
                furthestRightActiveIndex = m_iIndex - 1;
            }
        }
    }
}

int EnableBotButton::getChosenBotFlag()
{
    static std::vector<int> chosenBotFlags;
    if (chosenBotFlags.size() == 0)
    {
        chosenBotFlags.push_back(BOT_1_EASY);
        chosenBotFlags.push_back(BOT_1_NORMAL);
        chosenBotFlags.push_back(BOT_1_HARD);
        
        chosenBotFlags.push_back(BOT_2_EASY);
        chosenBotFlags.push_back(BOT_2_NORMAL);
        chosenBotFlags.push_back(BOT_2_HARD);
        
        chosenBotFlags.push_back(BOT_3_EASY);
        chosenBotFlags.push_back(BOT_3_NORMAL);
        chosenBotFlags.push_back(BOT_3_HARD);
        
        chosenBotFlags.push_back(BOT_4_EASY);
        chosenBotFlags.push_back(BOT_4_NORMAL);
        chosenBotFlags.push_back(BOT_4_HARD);
        
        chosenBotFlags.push_back(BOT_5_EASY);
        chosenBotFlags.push_back(BOT_5_NORMAL);
        chosenBotFlags.push_back(BOT_5_HARD);
        
        chosenBotFlags.push_back(BOT_6_EASY);
        chosenBotFlags.push_back(BOT_6_NORMAL);
        chosenBotFlags.push_back(BOT_6_HARD);
        
        chosenBotFlags.push_back(BOT_7_EASY);
        chosenBotFlags.push_back(BOT_7_NORMAL);
        chosenBotFlags.push_back(BOT_7_HARD);
        
        chosenBotFlags.push_back(BOT_8_EASY);
        chosenBotFlags.push_back(BOT_8_NORMAL);
        chosenBotFlags.push_back(BOT_8_HARD);
    }
    
    int basePosition = m_iIndex * 3;
    int position = basePosition + m_iDifficulty;
    
    return chosenBotFlags.at(position);
}

Button_State EnableBotButton::getButtonState()
{
    return m_buttonState;
}

int EnableBotButton::getIndex()
{
    return m_iIndex;
}

int EnableBotButton::getDifficulty()
{
    return m_iDifficulty;
}