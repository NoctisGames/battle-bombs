//
//  CountDownNumberGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__CountDownNumberGameObject__
#define __battlebombs__CountDownNumberGameObject__

#include "GameObject.h"
#include "CountDownNumberType.h"

class CountDownNumberGameObject : public GameObject
{
public:
	CountDownNumberGameObject(float x, float y, float width, float height, Count_Down_Number_Type type);
    
    void update(float deltaTime);
    
    Count_Down_Number_Type getType();
    
    bool remove();
    
private:
    Count_Down_Number_Type m_type;
    float m_fOriginalWidthToTargetWidthDifference;
    float m_fOriginalHeightToTargetHeightDifference;
    float m_fStateTime;
    bool m_remove;
};

#endif /* defined(__battlebombs__CountDownNumberGameObject__) */
