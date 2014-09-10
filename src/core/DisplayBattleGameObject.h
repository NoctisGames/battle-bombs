//
//  DisplayBattleGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__DisplayBattleGameObject__
#define __battlebombs__DisplayBattleGameObject__

#include "GameObject.h"

class DisplayBattleGameObject : public GameObject
{
public:
	DisplayBattleGameObject(float x, float y, float width, float height);
    
    void update(float deltaTime);
    
private:
    float m_fStateTime;
};

#endif /* defined(__battlebombs__DisplayBattleGameObject__) */
