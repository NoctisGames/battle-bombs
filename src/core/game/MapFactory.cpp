//
//  MapFactory.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "MapFactory.h"
#include "GameEvent.h"
#include "Map.h"
#include "MapSpace.h"
#include "MapGrasslands.h"
#include "MapMountains.h"
#include "MapBase.h"

Map * MapFactory::createMap(int mapType)
{
    switch (mapType)
    {
        case MAP_SPACE:
            return new MapSpace();
        case MAP_GRASSLANDS:
            return new MapGrasslands();
        case MAP_MOUNTAINS:
            return new MapMountains();
        case MAP_BASE:
            return new MapBase();
        default:
            return nullptr;
    }
}