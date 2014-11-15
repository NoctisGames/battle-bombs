//
//  Font.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Font__
#define __gowengamedev__Font__

#include <string>
#include "Color.h"
#include "TextureRegion.h"

#include <vector>
#include <memory>

class SpriteBatcher;

class Font
{
public:
	Font(int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight);
	   
	void renderText(SpriteBatcher &spriteBatcher, std::string &text, float x, float y, float width, float height, Color color, bool isCentered = false, bool isRightJustified = false);

private:
    std::vector<std::unique_ptr<TextureRegion>> m_glyphs;
};

#endif /* defined(__gowengamedev__Font__) */