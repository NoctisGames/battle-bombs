//
//  MapFactory.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__MapFactory__
#define __battlebombs__MapFactory__

#include <memory>

class Map;

class MapFactory
{
public:
    static MapFactory& getInstance()
    {
        static MapFactory instance; // Guaranteed to be destroyed.
        return instance;
    }
    
    Map * createMap(int mapType);
    
private:
    MapFactory() {}; // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    MapFactory(MapFactory const&);     // Don't Implement
    void operator=(MapFactory const&); // Don't implement
};

#endif /* defined(__battlebombs__MapFactory__) */