//
//  MapBorder.h
//  battlebombs
//
//  Created by Stephen Gowen on 6/23/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__MapBorder__
#define __battlebombs__MapBorder__

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

#endif /* defined(__battlebombs__MapBorder__) */