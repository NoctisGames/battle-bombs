//
//  OpenGLESSpriteBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__OpenGLESSpriteBatcher__
#define __battle_bombs__OpenGLESSpriteBatcher__

extern "C"
{
#include "platform_gl.h"
}

struct TEXTURE_VERTEX
{
    GLfloat X, Y, Z; // vertex position
    GLfloat R, G, B, A; // vertex color
    GLfloat U, V;    // texture coordinates
};

#include "SpriteBatcher.h"
#include <memory>
#include <vector>

class OpenGLESSpriteBatcher : public SpriteBatcher
{
public:
    OpenGLESSpriteBatcher();
    
    virtual void beginBatch();
    
    virtual void endBatchWithTexture(TextureWrapper &textureWrapper);
    
    virtual void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
    virtual void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
protected:
    virtual void drawSprite(float x, float y, float width, float height, TextureRegion tr);
    
    virtual void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);

private:
    std::vector<TEXTURE_VERTEX> m_textureVertices;
    std::vector<GLshort> m_indices;
    
    void addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v);
    
    void generateIndices();
};

#endif /* defined(__battle_bombs__OpenGLESSpriteBatcher__) */