//
//  SpriteBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__SpriteBatcher__
#define __battle_bombs__SpriteBatcher__

#include <memory>
#include "Color.h"

extern "C"
{
#include "platform_gl.h"
}

class Vertices2D;
class TextureRegion;

class SpriteBatcher
{
public:
    SpriteBatcher(int maxSprites, bool useColors);
    
    void beginBatch();
    
    void endBatchWithTexture(GLuint &texture);
    
    void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
    void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
private:
    std::unique_ptr<Vertices2D> m_vertices;
    std::unique_ptr<GLshort> m_indices;
    int m_iNumSprites;
    
    void drawSprite(float x, float y, float width, float height, TextureRegion tr);
    
    void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);
    
    void addColorCoordinates(Color &color);
    
    void generateIndices(int maxSprites);
};

#endif /* defined(__battle_bombs__SpriteBatcher__) */