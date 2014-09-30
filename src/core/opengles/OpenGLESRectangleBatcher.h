//
//  OpenGLESRectangleBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__OpenGLESRectangleBatcher__
#define __battlebombs__OpenGLESRectangleBatcher__

extern "C"
{
#include "platform_gl.h"
#include "program.h"
#include "linmath.h"
}

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
    
    void generateIndices();
};

#endif /* defined(__battlebombs__OpenGLESRectangleBatcher__) */
