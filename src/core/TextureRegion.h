//
//  TextureRegion.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__TextureRegion__
#define __battle_bombs__TextureRegion__

class TextureRegion
{
public:
    TextureRegion(float x, float y, float regionWidth, float regionHeight, float textureWidth, float textureHeight);
    
	float u1, v1, u2, v2;
};

#endif /* defined(__battle_bombs__TextureRegion__) */
