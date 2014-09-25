//
//  OpenGLESRectangleBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__OpenGLESRectangleBatcher__
#define __battlebombs__OpenGLESRectangleBatcher__

#include <memory>
#include "Color.h"

extern "C"
{
#include "platform_gl.h"
}

class Vertices2D;
class Rectangle;

class OpenGLESRectangleBatcher
{
public:
    OpenGLESRectangleBatcher(int maxRectangles, bool useColor, bool isFill);
    
    void beginBatch();
    
    void endBatch();
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
    void renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color);
    
private:
    std::unique_ptr<Vertices2D> m_vertices;
    std::unique_ptr<GLshort> m_indices;
    int m_iNumRectangles;
    bool m_useColor;
    bool m_isFill;
    
    void addColorCoordinates(Color &color);
    
    void generateIndices(int maxSprites);
};

#endif /* defined(__battlebombs__OpenGLESRectangleBatcher__) */
