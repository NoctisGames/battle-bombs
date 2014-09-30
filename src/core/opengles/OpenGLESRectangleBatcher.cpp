//
//  OpenGLESRectangleBatcher.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "OpenGLESRectangleBatcher.h"
#include "GameConstants.h"
#include "Rectangle.h"
#include "Vector2D.h"

extern "C"
{
#include "asset_utils.h"
#include "buffer.h"
}

static const int MAX_BATCH_SIZE = 256;
static const int VERTICES_PER_RECTANGLE = 4;
static const int INDICES_PER_RECTANGLE = 6;

OpenGLESRectangleBatcher::OpenGLESRectangleBatcher(bool isFill) : RectangleBatcher(isFill)
{
    m_iNumRectangles = 0;
    
    m_colorProgram = get_color_program(build_program_from_assets("color_shader.vsh", "color_shader.fsh"));
    
    generateIndices();
    
    vec3 eye = { 0, 0, 1 };
    vec3 center = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };
    
    mat4x4_ortho(m_projectionMatrix, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
    mat4x4_look_at(m_viewMatrix, eye, center, up);
    
    mat4x4_mul(m_viewProjectionMatrix, m_projectionMatrix, m_viewMatrix);
}

void OpenGLESRectangleBatcher::beginBatch()
{
    m_colorVertices.clear();
    m_iNumRectangles = 0;
}

void OpenGLESRectangleBatcher::endBatch()
{
    if (m_iNumRectangles > 0)
    {
        m_buffer = create_vbo(sizeof(GLfloat) * m_colorVertices.size(), &m_colorVertices[0], GL_STATIC_DRAW);
        
        glUseProgram(m_colorProgram.program);
        
        glUniformMatrix4fv(m_colorProgram.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m_viewProjectionMatrix);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glVertexAttribPointer(m_colorProgram.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, BUFFER_OFFSET(0));
        glVertexAttribPointer(m_colorProgram.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
        
        glEnableVertexAttribArray(m_colorProgram.a_position_location);
        glEnableVertexAttribArray(m_colorProgram.a_color_location);
        
        glDrawElements(m_isFill ? GL_TRIANGLES : GL_LINE_STRIP, m_iNumRectangles * INDICES_PER_RECTANGLE, GL_UNSIGNED_SHORT, &m_indices[0]);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDeleteBuffers(1, &m_buffer);
    }
}

void OpenGLESRectangleBatcher::renderRectangle(float x1, float y1, float x2, float y2, Color &color)
{
    addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha);
    addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha);
    addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha);
    addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha);
    
    m_iNumRectangles++;
}

void OpenGLESRectangleBatcher::addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    m_colorVertices.push_back(x);
    m_colorVertices.push_back(y);
    m_colorVertices.push_back(z);
    m_colorVertices.push_back(r);
    m_colorVertices.push_back(g);
    m_colorVertices.push_back(b);
    m_colorVertices.push_back(a);
}

ColorProgram OpenGLESRectangleBatcher::get_color_program(GLuint program)
{
    return (ColorProgram)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color")
    };
}

void OpenGLESRectangleBatcher::generateIndices()
{
    m_indices.reserve(MAX_BATCH_SIZE * INDICES_PER_RECTANGLE);
    
    GLshort j = 0;
    for (int i = 0; i < MAX_BATCH_SIZE * INDICES_PER_RECTANGLE; i += INDICES_PER_RECTANGLE, j += VERTICES_PER_RECTANGLE)
    {
        m_indices.push_back(j);
        m_indices.push_back(j + 1);
        m_indices.push_back(j + 2);
        m_indices.push_back(j + 2);
        m_indices.push_back(j + 3);
        m_indices.push_back(j + 0);
    }
}
