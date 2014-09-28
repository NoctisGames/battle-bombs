//
//  OpenGLESRectangleBatcher.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "OpenGLESRectangleBatcher.h"
#include "Vertices2D.h"
#include "Rectangle.h"
#include "Vector2D.h"

OpenGLESRectangleBatcher::OpenGLESRectangleBatcher(int maxRectangles, bool isFill) : RectangleBatcher(isFill)
{
    m_vertices = std::unique_ptr<Vertices2D>(new Vertices2D(maxRectangles, false));
    
    generateIndices(maxRectangles);
}

void OpenGLESRectangleBatcher::beginBatch()
{
    m_vertices->resetIndex();
    m_iNumRectangles = 0;
}

void OpenGLESRectangleBatcher::endBatch()
{
    if (m_iNumRectangles > 0)
    {
        m_vertices->bind();
        m_vertices->drawPrimitiveType(m_isFill ? GL_TRIANGLES : GL_LINE_STRIP, 0, m_indices.get(), m_iNumRectangles * 6);
        m_vertices->unbind();
    }
}

void OpenGLESRectangleBatcher::renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color)
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
    
    m_vertices->addVertexCoordinate(leftX);
    m_vertices->addVertexCoordinate(topY);
    addColorCoordinates(color);
    
    m_iNumRectangles++;
}

void OpenGLESRectangleBatcher::addColorCoordinates(Color &color)
{
    m_vertices->addColorCoordinate(color.red);
    m_vertices->addColorCoordinate(color.green);
    m_vertices->addColorCoordinate(color.blue);
    m_vertices->addColorCoordinate(color.alpha);
}

void OpenGLESRectangleBatcher::generateIndices(int maxRectangles)
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
