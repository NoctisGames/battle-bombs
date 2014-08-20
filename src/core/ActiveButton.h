//
//  ActiveButton.h
//  battlebombs
//
//  Created by Mikel Adkins on 5/18/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__ActiveButton__
#define __battle_bombs__ActiveButton__

#include "GameObject.h"
#include "PowerUpType.h"
#include "ButtonState.h"

class ActiveButton : public GameObject
{
public:
	ActiveButton(float x, float y, float width, float height);
    
    void setPowerUpType(Power_Up_Type powerUpType);
    
    Power_Up_Type getPowerUpType();
    
    void setButtonState(Button_State buttonState);
    
    Button_State getButtonState();
    
    void setIsPressed(bool isPressed);
    
    bool isPressed();
    
private:
    Power_Up_Type m_powerUpType;
    Button_State m_buttonState;
    bool m_isPressed;
};

#endif /* defined (__ActiveButton__) */
