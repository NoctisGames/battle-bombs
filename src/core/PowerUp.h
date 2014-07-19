//
//  PowerUp.h
//  bomberparty
//
//  Created by Mikel Adkins on 5/10/2014.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__PowerUp__
#define __bomberparty__PowerUp__

#include "GridGameObject.h"
#include "PowerUpType.h"

class PowerUp : public GridGameObject
{
public:
	PowerUp(int gridX, int gridY, int powerUpFlag);
    
    void update(float deltaTime);

	void onPickedUp();

	bool isPickedUp();
    
    float getStateTime();

	int getPowerUpFlag();

	Power_Up_Type getType();

private:
    float m_fStateTime;
	int m_powerUpFlag;
    bool m_isPickedUp;
	Power_Up_Type m_powerUpType;
};

#endif /* defined (__bomberparty__PowerUp__) */