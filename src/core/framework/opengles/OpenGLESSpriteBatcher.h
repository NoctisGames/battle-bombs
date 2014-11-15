//
//  OpenGLESSpriteBatcher.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESSpriteBatcher__
#define __gowengamedev__OpenGLESSpriteBatcher__

extern "C"
{
#include "platform_gl.h"
#include "linmath.h"
}

typedef struct
{
    GLuint program;
    
    GLint u_mvp_matrix_location;
    GLint a_position_location;
    GLint a_color_location;
    GLint a_texture_coordinates_location;
    GLint u_texture_unit_location;
} TextureProgram;

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
    std::vector<GLfloat> m_textureVertices;
    std::vector<GLshort> m_indices;
    TextureProgram m_textureProgram;
    mat4x4 m_viewMatrix;
    mat4x4 m_projectionMatrix;
    mat4x4 m_viewProjectionMatrix;
    
    void addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v);
    
    TextureProgram get_texture_program(GLuint program);
    
    void generateIndices();
};

#endif /* defined(__gowengamedev__OpenGLESSpriteBatcher__) */