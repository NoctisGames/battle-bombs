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
    EnableBotButton(Button_State buttonState, bool isFurthestRight, float x, float y, float width, float height);
    
    void toggle();
    
    void setButtonState(Button_State buttonState);
    
    Button_State getButtonState();
    
    int getDifficulty();
    
    void setIsFurthestRight(bool isFurthestRight);
    
    bool isFurthestRight();
    
private:
    Button_State m_buttonState;
    int m_iDifficulty;
    bool m_isFurthestRight;
};

#endif /* defined(__battlebombs__EnableBotButton__) */