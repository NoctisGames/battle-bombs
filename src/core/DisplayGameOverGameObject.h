//
//  DisplayGameOverGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__DisplayGameOverGameObject__
#define __battlebombs__DisplayGameOverGameObject__

#include "GameObject.h"
#include "DisplayGameOverType.h"

class DisplayGameOverGameObject : public GameObject
{
public:
	DisplayGameOverGameObject(float x, float y, float width, float height, Display_Game_Over_Type type);
    
    void update(float deltaTime);
    
    Display_Game_Over_Type getType();
    
private:
    Display_Game_Over_Type m_type;
    float m_fStateTime;
    float m_fOrigWidth;
    float m_fOrigHeight;
    float m_fSizeScalar;
    bool m_isSizeIncreasing;
};

#endif /* defined(__battlebombs__DisplayGameOverGameObject__) */
