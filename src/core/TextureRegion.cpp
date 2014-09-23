//
//  TextureRegion.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "TextureRegion.h"

TextureRegion::TextureRegion(float x, float y, float regionWidth, float regionHeight, float textureWidth, float textureHeight)
{
	u1 = x / textureWidth;
    v1 = y / textureHeight;
    u2 = u1 + regionWidth / textureWidth;
    v2 = v1 + regionHeight / textureHeight;
}
