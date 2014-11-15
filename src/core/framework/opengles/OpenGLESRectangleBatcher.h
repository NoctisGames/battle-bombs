//
//  OpenGLESRectangleBatcher.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESRectangleBatcher__
#define __gowengamedev__OpenGLESRectangleBatcher__

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
} ColorProgram;

#include "RectangleBatcher.h"
#include <memory>
#include <vector>

class OpenGLESRectangleBatcher : public RectangleBatcher
{
public:
    OpenGLESRectangleBatcher(bool isFill);
    
    virtual void beginBatch();
    
    virtual void endBatch();
    
    virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
    
private:
    std::vector<GLfloat> m_colorVertices;
    std::vector<GLshort> m_indices;
    ColorProgram m_colorProgram;
    mat4x4 m_viewMatrix;
    mat4x4 m_projectionMatrix;
    mat4x4 m_viewProjectionMatrix;
    
    void addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    
    ColorProgram get_color_program(GLuint program);
    
    void generateIndices();
};

#endif /* defined(__gowengamedev__OpenGLESRectangleBatcher__) */
