//
//  PowerUpBarItem.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/12/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__PowerUpBarItem__
#define __battlebombs__PowerUpBarItem__

#include "GameObject.h"
#include "PowerUpType.h"

class PowerUpBarItem : public GameObject
{
public:
    PowerUpBarItem(float x, float y, float width, float height);
    
    void setPowerUpType(int powerUpType);
    
    int getPowerUpType();
    
    void setLevel(int level);
    
    int getLevel();
    
private:
    int m_powerUpType;
    int m_iLevel;
};

#endif /* defined(__battlebombs__PowerUpBarItem__) */
