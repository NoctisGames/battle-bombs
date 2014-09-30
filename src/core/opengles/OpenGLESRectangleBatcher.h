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
}

struct COLOR_VERTEX
{
    GLfloat X, Y, Z; // vertex position
    GLfloat R, G, B, A; // vertex color
};

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
    std::vector<COLOR_VERTEX> m_colorVertices;
    std::vector<GLshort> m_indices;
    
    void addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v);
    
    void generateIndices();
};

#endif /* defined(__battlebombs__OpenGLESRectangleBatcher__) */
