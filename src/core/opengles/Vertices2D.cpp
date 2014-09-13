//
//  Vertices2D.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "Vertices2D.h"
#include <stdlib.h>

Vertices2D::Vertices2D(int maxNumVertices, bool isUsingTextureCoordinates, bool isUsingColors)
{
    m_vertices = std::unique_ptr<GLfloat>(new GLfloat[maxNumVertices * 2]);
    m_textureCoords = std::unique_ptr<GLfloat>(new GLfloat[maxNumVertices * 2]);
    
    if(isUsingColors)
    {
        m_colors = std::unique_ptr<GLfloat>(new GLfloat[maxNumVertices * 4]);
    }
    
    m_iVerticesIndex = 0;
    m_iTextureCoordsIndex = 0;
    m_iColorsIndex = 0;
    
    m_hasColor = isUsingColors;
    m_hasTextureCoordinates = isUsingTextureCoordinates;
}

void Vertices2D::resetIndex()
{
    m_iVerticesIndex = 0;
    m_iTextureCoordsIndex = 0;
    m_iColorsIndex = 0;
}

void Vertices2D::addVertexCoordinate(float vc)
{
    m_vertices.get()[m_iVerticesIndex++] = vc;
}

void Vertices2D::addTextureCoordinate(float tc)
{
    m_textureCoords.get()[m_iTextureCoordsIndex++] = tc;
}

void Vertices2D::addColorCoordinate(float cc)
{
    m_colors.get()[m_iColorsIndex++] = cc;
}

void Vertices2D::bind()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, m_vertices.get());
    
    if (m_hasColor)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, m_colors.get());
    }
    
    if (m_hasTextureCoordinates)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, m_textureCoords.get());
    }
}

void Vertices2D::unbind()
{
    if (m_hasTextureCoordinates)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    if (m_hasColor)
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

void Vertices2D::drawPrimitiveType(GLenum mode, int offset, GLshort *indices, int numVertices)
{
    if (indices == nullptr)
    {
        glDrawArrays(mode, offset, numVertices);
    }
    else
    {
        glDrawElements(mode, numVertices, GL_UNSIGNED_SHORT, indices);
    }
}