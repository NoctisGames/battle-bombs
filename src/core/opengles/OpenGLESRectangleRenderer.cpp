//
//  OpenGLESRectangleRenderer.cpp
//  battlebombs
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

OpenGLESRectangleRenderer::OpenGLESRectangleRenderer(int maxRectangles, bool useColor, bool isFill)
{
    m_vertices = std::unique_ptr<Vertices2D>(new Vertices2D(maxRectangles, false, useColor));
    m_iNumRectangles = 0;
    m_useColor = useColor;
    m_isFill = isFill;
    
    generateIndices(maxRectangles);
}

void OpenGLESRectangleRenderer::beginBatch()
{
    m_vertices->resetIndex();
    m_iNumRectangles = 0;
}

void OpenGLESRectangleRenderer::endBatch()
{
    if (m_iNumRectangles > 0)
    {
        m_vertices->bind();
        m_vertices->drawPrimitiveType(m_isFill ? GL_TRIANGLES : GL_LINE_STRIP, 0, m_indices.get(), m_iNumRectangles * 6);
        m_vertices->unbind();
    }
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
    
    m_iNumRectangles++;
}

void OpenGLESRectangleRenderer::addColorCoordinates(Color &color)
{
    m_vertices->addColorCoordinate(color.red);
    m_vertices->addColorCoordinate(color.green);
    m_vertices->addColorCoordinate(color.blue);
    m_vertices->addColorCoordinate(color.alpha);
}

void OpenGLESRectangleRenderer::generateIndices(int maxRectangles)
{
    int numIndices = maxRectangles * 6;
    m_indices = std::unique_ptr<GLshort>(new GLshort[numIndices]);
    
    GLshort j = 0;
    
    for (int i = 0; i < numIndices; i += 6, j += 4)
    {
        m_indices.get()[i + 0] = j + 0;
        m_indices.get()[i + 1] = j + 1;
        m_indices.get()[i + 2] = j + 2;
        m_indices.get()[i + 3] = j + 2;
        m_indices.get()[i + 4] = j + 3;
        m_indices.get()[i + 5] = j + 0;
    }
}
