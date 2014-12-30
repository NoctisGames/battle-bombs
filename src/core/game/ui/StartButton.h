//
//  StartButton.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__StartButton__
#define __battlebombs__StartButton__

#include "GameObject.h"
#include "ButtonState.h"

class StartButton : public GameObject
{
public:
    StartButton(float x, float y, float width, float height);
    
    void setButtonState(Button_State buttonState);
    
    Button_State getButtonState();
    
private:
    Button_State m_buttonState;
};

#endif /* defined(__battlebombs__StartButton__) */