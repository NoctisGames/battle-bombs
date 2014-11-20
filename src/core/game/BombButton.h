//
//  BombButton.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/11/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__BombButton__
#define __battlebombs__BombButton__

#include "GameObject.h"
#include "ButtonState.h"

class BombButton : public GameObject
{
public:
    BombButton(float x, float y, float width, float height);
    
    void setButtonState(Button_State buttonState);
    
    Button_State getButtonState();
    
    void setIsPressed(bool isPressed);
    
    bool isPressed();
    
private:
    Button_State m_buttonState;
    bool m_isPressed;
};

#endif /* defined(__battlebombs__BombButton__) */
