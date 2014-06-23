//
//  MapBorder.h
//  bomberparty
//
//  Created by Stephen Gowen on 6/23/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__MapBorder__
#define __bomberparty__MapBorder__

#include "GameObject.h"
#include "MapBorderType.h"

class MapBorder : public GameObject
{
public:
    MapBorder(Map_Border_Type type, float x, float y, float width, float height);
    
    Map_Border_Type getType();
    
    bool isNearFront();
    
private:
    Map_Border_Type m_type;
};

#endif /* defined(__bomberparty__MapBorder__) */