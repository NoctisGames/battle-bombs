//
//  PowerUp.h
//  battlebombs
//
//  Created by Mikel Adkins on 5/10/2014.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__PowerUp__
#define __battlebombs__PowerUp__

#include "GridGameObject.h"
#include "PowerUpType.h"

class PowerUp : public GridGameObject
{
public:
	PowerUp(int gridX, int gridY, int m_powerUpType);
    
    void update(float deltaTime);

	void onPickedUp();

	bool isPickedUp();
    
    float getStateTime();

	int getType();

private:
    float m_fStateTime;
    bool m_isPickedUp;
	int m_powerUpType;
};

#endif /* defined (__battlebombs__PowerUp__) */