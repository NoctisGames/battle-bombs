//
//  OpenGLESRectangleRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__OpenGLESRectangleRenderer__
#define __battlebombs__OpenGLESRectangleRenderer__

#include <memory>
#include "Color.h"

class Vertices2D;
class Rectangle;

class OpenGLESRectangleRenderer
{
public:
    OpenGLESRectangleRenderer(bool useColor, bool isFill);
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
    void renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color);
    
private:
    std::unique_ptr<Vertices2D> m_vertices;
    bool m_useColor;
    bool m_isFill;
    
    void addColorCoordinates(Color &color);
};

#endif /* defined(__battlebombs__OpenGLESRectangleRenderer__) */
