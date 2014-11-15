//
//  InsideBlock.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/24/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__InsideBlock__
#define __battlebombs__InsideBlock__

#include "GridGameObject.h"
#include "InsideBlockState.h"

class InsideBlock : public GridGameObject
{
public:
    InsideBlock(int gridX, int gridY);
    
    void update(float deltaTime);
    
    void onHitByIceBall();
    
    void onDestroy();
    
    Inside_Block_State getInsideBlockState();
    
    float getStateTime();
    
private:
    Inside_Block_State m_insideBlockState;
    float m_fStateTime;
};

#endif /* defined(__battlebombs__InsideBlock__) */
