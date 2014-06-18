//
//  BreakableBlock.h
//  bomberparty
//
//  Created by Mikel Adkins on 3/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__BreakableBlock__
#define __bomberparty__BreakableBlock__

#include "GridGameObject.h"
#include "PowerUp.h"
#include "BreakableBlockState.h"

class BreakableBlock : public GridGameObject
{
public:
	BreakableBlock(int gridX, int gridY, int powerUpFlag);
    
    void update(float deltaTime);

    void onDestroy();

    bool hasPowerUp();

    float getX();

    float getY();

    Breakable_Block_State getBreakableBlockState();
    
    float getStateTime();
    
	int getPowerUpFlag();

private:
    Breakable_Block_State m_breakableBlockState;
    float m_fStateTime;
	int m_powerUpFlag;
};

#endif /* defined (__bomberparty__BreakableBlock__) */
