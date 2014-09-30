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
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "asset_utils.h"
#include "buffer.h"
#include "image.h"
#include "linmath.h"
#include "platform_gl.h"
#include "platform_asset_utils.h"
#include "program.h"
#include "shader.h"

static ColorProgram colorProgram;

static mat4x4 viewProjectionMatrix;

static const size_t MAX_BATCH_SIZE = 256;
static const size_t VERTICES_PER_RECTANGLE = 4;
static const size_t INDICES_PER_RECTANGLE = 6;

OpenGLESRectangleBatcher::OpenGLESRectangleBatcher(bool isFill) : RectangleBatcher(isFill)
{
    m_iNumRectangles = 0;
    
    colorProgram = get_color_program(build_program_from_assets("shaders/color_shader.vsh", "shaders/color_shader.fsh"));
    
    generateIndices();
    
    mat4x4 viewMatrix;
    mat4x4_look_at(viewMatrix, (vec3){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1}, (vec3){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0}, (vec3){0, 1, 0});
    
    mat4x4 projectionMatrix;
    mat4x4_ortho(projectionMatrix, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -1.0, 1.0);
    
    mat4x4_mul(viewProjectionMatrix, viewMatrix, projectionMatrix);
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
        GLuint buffer = create_vbo(sizeof(COLOR_VERTEX) * m_iNumRectangles, &m_colorVertices[0], GL_STATIC_DRAW);
        
        glUseProgram(colorProgram.program);
        
        glUniformMatrix4fv(colorProgram.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)viewProjectionMatrix);
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(colorProgram.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(COLOR_VERTEX), BUFFER_OFFSET(0));
        glVertexAttribPointer(colorProgram.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(COLOR_VERTEX), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
        
        glEnableVertexAttribArray(colorProgram.a_position_location);
        glEnableVertexAttribArray(colorProgram.a_color_location);
        
        glDrawElements(m_isFill ? GL_TRIANGLES : GL_LINE_STRIP, m_iNumRectangles * INDICES_PER_RECTANGLE, GL_UNSIGNED_SHORT, &m_indices[0]);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void OpenGLESRectangleBatcher::renderRectangle(float x1, float y1, float x2, float y2, Color &color)
{
    addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
    addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
    addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
    addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
    
    m_iNumRectangles++;
}

void OpenGLESRectangleBatcher::addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v)
{
    COLOR_VERTEX cv = { x, y, z, r, g, b, a };
    m_colorVertices.push_back(cv);
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
