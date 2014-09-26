//
//  OpenGLESRectangleBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__OpenGLESRectangleBatcher__
#define __battlebombs__OpenGLESRectangleBatcher__

#include "RectangleBatcher.h"
#include <memory>

extern "C"
{
#include "platform_gl.h"
}

class Vertices2D;

class OpenGLESRectangleBatcher : public RectangleBatcher
{
public:
    OpenGLESRectangleBatcher(int maxRectangles, bool isFill);
    
    virtual void beginBatch();
    
    virtual void endBatch();
    
    virtual void renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color);
    
private:
    std::unique_ptr<Vertices2D> m_vertices;
    std::unique_ptr<GLshort> m_indices;
    
    void addColorCoordinates(Color &color);
    
    void generateIndices(int maxSprites);
};

#endif /* defined(__battlebombs__OpenGLESRectangleBatcher__) */
