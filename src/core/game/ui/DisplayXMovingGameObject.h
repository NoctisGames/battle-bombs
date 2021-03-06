//
//  DisplayXMovingGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__DisplayXMovingGameObject__
#define __battlebombs__DisplayXMovingGameObject__

#include "GameObject.h"
#include "DisplayXMovingType.h"

class DisplayXMovingGameObject : public GameObject
{
public:
	DisplayXMovingGameObject(float x, float y, float width, float height, Display_X_Moving_Type type);
    
    void begin();
    
    void update(float deltaTime);
    
    Display_X_Moving_Type getType();
    
private:
    float m_fStateTime;
    bool m_isAnimating;
    Display_X_Moving_Type m_type;
};

#endif /* defined(__battlebombs__DisplayXMovingGameObject__) */
