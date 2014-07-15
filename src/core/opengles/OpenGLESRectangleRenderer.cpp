//
//  OpenGLESRectangleRenderer.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define numPointsOnRectangle 6

#include "OpenGLESRectangleRenderer.h"
#include "Vertices2D.h"
#include "Rectangle.h"
#include "Vector2D.h"

extern "C"
{
#include "platform_gl.h"
}

OpenGLESRectangleRenderer::OpenGLESRectangleRenderer(bool useColor, bool isFill)
{
    int numVerticesForRectangle = numPointsOnRectangle * (useColor ? 6 : 2); // 2 vertices for coordinates, 4 vertices for colors
    m_vertices = std::unique_ptr<Vertices2D>(new Vertices2D(numVerticesForRectangle, false, useColor));
    m_useColor = useColor;
    m_isFill = isFill;
}

void OpenGLESRectangleRenderer::renderRectangle(Rectangle &rectangle, Color &color)
{
    float x1 = rectangle.getLowerLeft().getX();
    float y1 = rectangle.getLowerLeft().getY();
    float x2 = x1 + rectangle.getWidth();
    float y2 = y1 + rectangle.getHeight();
    
    renderRectangle(x1, y1, x2, y2, color);
}

void OpenGLESRectangleRenderer::renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color)
{
    m_vertices->resetIndex();
    
    if (m_useColor)
    {
        m_vertices->addVertexCoordinate(leftX);
        m_vertices->addVertexCoordinate(bottomY);
        addColorCoordinates(color);
        
        m_vertices->addVertexCoordinate(rightX);
        m_vertices->addVertexCoordinate(bottomY);
        addColorCoordinates(color);
        
        m_vertices->addVertexCoordinate(rightX);
        m_vertices->addVertexCoordinate(topY);
        addColorCoordinates(color);
        
        m_vertices->addVertexCoordinate(rightX);
        m_vertices->addVertexCoordinate(topY);
        addColorCoordinates(color);
        
        m_vertices->addVertexCoordinate(leftX);
        m_vertices->addVertexCoordinate(topY);
        addColorCoordinates(color);
        
        m_vertices->addVertexCoordinate(leftX);
        m_vertices->addVertexCoordinate(bottomY);
        addColorCoordinates(color);
    }
    else
    {
        m_vertices->addVertexCoordinate(leftX);
        m_vertices->addVertexCoordinate(bottomY);
        
        m_vertices->addVertexCoordinate(rightX);
        m_vertices->addVertexCoordinate(bottomY);
        
        m_vertices->addVertexCoordinate(rightX);
        m_vertices->addVertexCoordinate(topY);
        
        m_vertices->addVertexCoordinate(rightX);
        m_vertices->addVertexCoordinate(topY);
        
        m_vertices->addVertexCoordinate(leftX);
        m_vertices->addVertexCoordinate(topY);
        
        m_vertices->addVertexCoordinate(leftX);
        m_vertices->addVertexCoordinate(bottomY);
    }
    
    m_vertices->bind();
    m_vertices->drawPrimitiveType(m_isFill ? GL_TRIANGLES : GL_LINE_STRIP, 0, nullptr, numPointsOnRectangle);
    m_vertices->unbind();
}

void OpenGLESRectangleRenderer::addColorCoordinates(Color &color)
{
    m_vertices->addColorCoordinate(color.red);
    m_vertices->addColorCoordinate(color.green);
    m_vertices->addColorCoordinate(color.blue);
    m_vertices->addColorCoordinate(color.alpha);
}
