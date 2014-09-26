//
//  OpenGLESSpriteBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__OpenGLESSpriteBatcher__
#define __battle_bombs__OpenGLESSpriteBatcher__

#include <memory>
#include "SpriteBatcher.h"

extern "C"
{
#include "platform_gl.h"
}

class Vertices2D;

class OpenGLESSpriteBatcher : public SpriteBatcher
{
public:
    OpenGLESSpriteBatcher(int maxSprites);
    
    virtual void beginBatch();
    
    virtual void endBatchWithTexture(TextureWrapper &textureWrapper);
    
    virtual void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
    virtual void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
protected:
    virtual void drawSprite(float x, float y, float width, float height, TextureRegion tr);
    
    virtual void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);

private:
    std::unique_ptr<Vertices2D> m_vertices;
    std::unique_ptr<GLshort> m_indices;
    
    void addColorCoordinates(Color &color);
    
    void generateIndices(int maxSprites);
};

#endif /* defined(__battle_bombs__OpenGLESSpriteBatcher__) */