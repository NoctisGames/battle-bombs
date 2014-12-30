//
//  EnablePowerUpButton.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/27/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__EnablePowerUpButton__
#define __battlebombs__EnablePowerUpButton__

#include "GameObject.h"
#include "ButtonState.h"

class EnablePowerUpButton : public GameObject
{
public:
    EnablePowerUpButton(int chosenPowerUpFlag, float x, float y, float width, float height);
    
    void toggle();
    
    int getChosenPowerUpFlag();
    
    Button_State getButtonState();
    
private:
    Button_State m_buttonState;
    int m_iChosenPowerUpFlag;
};

#endif /* defined(__battlebombs__EnablePowerUpButton__) */