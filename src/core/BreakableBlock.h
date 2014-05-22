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

class BreakableBlock : public GridGameObject
{
public:
	BreakableBlock(int gridX, int gridY, int powerUpFlag);

    void onDestroy();

    bool isDestroyed();

    bool hasPowerUp();

    float getX();

    float getY();

	int getPowerUpFlag();

private:
    bool m_isDestroyed;
    bool m_hasPowerUp;
	int m_powerUpFlag;
};

#endif /* defined (__bomberparty__BreakableBlock__) */
