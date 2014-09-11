//
//  Vertices2D.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Vertices2D__
#define __battle_bombs__Vertices2D__

#include <memory>

extern "C"
{
#include "platform_gl.h"
}

class Vertices2D
{
public:
    Vertices2D(int maxNumVertices, bool isUsingTextureCoordinates, bool isUsingColors);
    
    void resetIndex();
    
    void addVertexCoordinate(float vc);
    
    void addTextureCoordinate(float tc);
    
    void addColorCoordinate(float cc);
    
    void bind();
    
    void unbind();
    
    void drawPrimitiveType(GLenum mode, int offset, GLshort *indices, int numVertices);

private:
    std::unique_ptr<GLfloat> m_vertices;
    std::unique_ptr<GLfloat> m_textureCoords;
    std::unique_ptr<GLfloat> m_colors;
    int m_iVerticesIndex;
    int m_iTextureCoordsIndex;
    int m_iColorsIndex;
    bool m_hasColor;
    bool m_hasTextureCoordinates;
};

#endif /* defined(__battle_bombs__Vertices2D__) */