//
//  Font.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Font__
#define __battle_bombs__Font__

#include <string>
#include "Color.h"
#include <vector>
#include <memory>
#include "TextureRegion.h"

class SpriteBatcher;

class Font
{
public:
	Font(int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight);
	   
	void renderText(SpriteBatcher &spriteBatcher, std::string &text, float x, float y, float width, float height, Color color, bool isCentered = false, bool isRightJustified = false);

private:
    std::vector<std::unique_ptr<TextureRegion>> m_glyphs;
};

#endif /* defined(__battle_bombs__Font__) */