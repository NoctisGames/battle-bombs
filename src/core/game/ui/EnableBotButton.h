//
//  EnableBotButton.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__EnableBotButton__
#define __battlebombs__EnableBotButton__

#include "GameObject.h"
#include "ButtonState.h"

class EnableBotButton : public GameObject
{
public:
    EnableBotButton(Button_State buttonState, int index, bool isFurthestRightActive, float x, float y, float width, float height);
    
    void toggle();
    
    int getChosenBotFlag();
    
    Button_State getButtonState();
    
    int getIndex();
    
    int getDifficulty();
    
private:
    Button_State m_buttonState;
    int m_iIndex;
    int m_iDifficulty;
};

#endif /* defined(__battlebombs__EnableBotButton__) */