//
//  BreakableBlock.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/25/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__BreakableBlock__
#define __battlebombs__BreakableBlock__

#include "GridGameObject.h"
#include "PowerUp.h"
#include "BreakableBlockState.h"

class BreakableBlock : public GridGameObject
{
public:
	BreakableBlock(int gridX, int gridY, int powerUpFlag);
    
    void update(float deltaTime);
    
    void onHitByIceBall();

    void onDestroy();
    
    void onHitByFireBall();

    bool hasPowerUp();

    Breakable_Block_State getBreakableBlockState();
    
    float getStateTime();
    
	int getPowerUpFlag();
    
    bool wasHitByFireBall();

private:
    Breakable_Block_State m_breakableBlockState;
    float m_fStateTime;
	int m_powerUpFlag;
    bool m_hitByFireBall;
};

#endif /* defined (__battlebombs__BreakableBlock__) */
