//
//  PowerUp.h
//  bomberparty
//
//  Created by Mikel Adkins on 5/10/2014.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__PowerUp__
#define __bomberparty__PowerUp__

#include "GameObject.h"
#include "PowerUpType.h"

class PowerUp : public GameObject
{
public:
	PowerUp(float x, float y, int powerUpFlag);

	void onPickedUp();

	bool isPickedUp();

	int getPowerUpFlag();

	Power_Up_Type getType();

private:
	int m_powerUpFlag;
    bool m_isPickedUp;
	Power_Up_Type m_powerUpType;
};

#endif /* defined (__bomberparty__PowerUp__) */