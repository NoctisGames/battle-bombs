//
//  TextureRegion.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomber_party__TextureRegion__
#define __bomber_party__TextureRegion__

#ifdef TECHNE_GAMES_DIRECT_3D
#include <windows.h>
#endif

class TextureRegion
{
public:
    TextureRegion(float x, float y, float regionWidth, float regionHeight, float textureWidth, float textureHeight);
    
#ifdef TECHNE_GAMES_OPENGL_ES
    float u1, v1, u2, v2;
#elif TECHNE_GAMES_DIRECT_3D
    long left, top, right, bottom;
    RECT getSourceRECT();
#endif
};

#endif /* defined(__bomber_party__TextureRegion__) */
