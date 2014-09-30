//
//  OpenGLESSpriteBatcher.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "OpenGLESSpriteBatcher.h"
#include "GameConstants.h"
#include "TextureRegion.h"
#include "Rectangle.h"
#include "Vector2D.h"

extern "C"
{
#include "asset_utils.h"
#include "buffer.h"
}

static const int MAX_BATCH_SIZE = 2048;
static const int VERTICES_PER_SPRITE = 4;
static const int INDICES_PER_SPRITE = 6;

OpenGLESSpriteBatcher::OpenGLESSpriteBatcher()
{
    m_iNumSprites = 0;
    
    m_textureProgram = get_texture_program(build_program_from_assets("texture_shader.vsh", "texture_shader.fsh"));
    
    generateIndices();
    
    vec3 eye = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1 };
    vec3 center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
    vec3 up = { 0, 1, 0 };
    mat4x4_look_at(m_viewMatrix, eye, center, up);
    
    mat4x4_ortho(m_projectionMatrix, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -1.0, 1.0);
    
    mat4x4_mul(m_viewProjectionMatrix, m_viewMatrix, m_projectionMatrix);
}

void OpenGLESSpriteBatcher::beginBatch()
{
    m_textureVertices.clear();
    m_iNumSprites = 0;
}

void OpenGLESSpriteBatcher::endBatchWithTexture(TextureWrapper &textureWrapper)
{
    if(m_iNumSprites > 0)
    {
        m_buffer = create_vbo(sizeof(GLfloat) * m_textureVertices.size(), &m_textureVertices[0], GL_STATIC_DRAW);
        
        glUseProgram(m_textureProgram.program);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureWrapper.texture);
        glUniformMatrix4fv(m_textureProgram.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m_viewProjectionMatrix);
        glUniform1i(m_textureProgram.u_texture_unit_location, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glVertexAttribPointer(m_textureProgram.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
        glVertexAttribPointer(m_textureProgram.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
        glVertexAttribPointer(m_textureProgram.a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
        
        glEnableVertexAttribArray(m_textureProgram.a_position_location);
        glEnableVertexAttribArray(m_textureProgram.a_color_location);
        glEnableVertexAttribArray(m_textureProgram.a_texture_coordinates_location);
        
        glDrawElements(GL_TRIANGLES, m_iNumSprites * INDICES_PER_SPRITE, GL_UNSIGNED_SHORT, &m_indices[0]);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void OpenGLESSpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr)
{
    if(angle > 0)
    {
        float halfWidth = width / 2;
        float halfHeight = height / 2;
        
        float rad = DEGREES_TO_RADIANS(angle);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        float x1 = -halfWidth * cos - (-halfHeight) * sin;
        float y1 = -halfWidth * sin + (-halfHeight) * cos;
        
        float x2 = halfWidth * cos - (-halfHeight) * sin;
        float y2 = halfWidth * sin + (-halfHeight) * cos;
        
        float x3 = halfWidth * cos - halfHeight * sin;
        float y3 = halfWidth * sin + halfHeight * cos;
        
        float x4 = -halfWidth * cos - halfHeight * sin;
        float y4 = -halfWidth * sin + halfHeight * cos;
        
        x1 += x;
        y1 += y;
        
        x2 += x;
        y2 += y;
        
        x3 += x;
        y3 += y;
        
        x4 += x;
        y4 += y;
        
        addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
        addVertexCoordinate(x4, y4, 0, 1, 1, 1, 1, tr.u1, tr.v1);
        addVertexCoordinate(x3, y3, 0, 1, 1, 1, 1, tr.u2, tr.v1);
        addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v2);
    }
    else
    {
        drawSprite(x, y, width, height, tr);
    }
    
    m_iNumSprites++;
}

void OpenGLESSpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr)
{
    if(angle > 0)
    {
        float halfWidth = width / 2;
        float halfHeight = height / 2;
        
        float rad = DEGREES_TO_RADIANS(angle);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        float x1 = -halfWidth * cos - (-halfHeight) * sin;
        float y1 = -halfWidth * sin + (-halfHeight) * cos;
        
        float x2 = halfWidth * cos - (-halfHeight) * sin;
        float y2 = halfWidth * sin + (-halfHeight) * cos;
        
        float x3 = halfWidth * cos - halfHeight * sin;
        float y3 = halfWidth * sin + halfHeight * cos;
        
        float x4 = -halfWidth * cos - halfHeight * sin;
        float y4 = -halfWidth * sin + halfHeight * cos;
        
        x1 += x;
        y1 += y;
        
        x2 += x;
        y2 += y;
        
        x3 += x;
        y3 += y;
        
        x4 += x;
        y4 += y;
        
        addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v2);
        addVertexCoordinate(x4, y4, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v1);
        addVertexCoordinate(x3, y3, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v1);
        addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v2);
    }
    else
    {
        drawSprite(x, y, width, height, color, tr);
    }
    
    m_iNumSprites++;
}

#pragma private methods

void OpenGLESSpriteBatcher::drawSprite(float x, float y, float width, float height, TextureRegion tr)
{
    GLfloat halfWidth = width / 2;
    GLfloat halfHeight = height / 2;
    GLfloat x1 = x - halfWidth;
    GLfloat y1 = y - halfHeight;
    GLfloat x2 = x + halfWidth;
    GLfloat y2 = y + halfHeight;
    
    addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
    addVertexCoordinate(x1, y2, 0, 1, 1, 1, 1, tr.u1, tr.v1);
    addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v1);
    addVertexCoordinate(x2, y1, 0, 1, 1, 1, 1, tr.u2, tr.v2);
}

void OpenGLESSpriteBatcher::drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr)
{
    GLfloat halfWidth = width / 2;
    GLfloat halfHeight = height / 2;
    GLfloat x1 = x - halfWidth;
    GLfloat y1 = y - halfHeight;
    GLfloat x2 = x + halfWidth;
    GLfloat y2 = y + halfHeight;
    
    addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v2);
    addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v1);
    addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v1);
    addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v2);
}

void OpenGLESSpriteBatcher::addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v)
{
    m_textureVertices.push_back(x);
    m_textureVertices.push_back(y);
    m_textureVertices.push_back(z);
    m_textureVertices.push_back(r);
    m_textureVertices.push_back(g);
    m_textureVertices.push_back(b);
    m_textureVertices.push_back(a);
    m_textureVertices.push_back(u);
    m_textureVertices.push_back(v);
}

void OpenGLESSpriteBatcher::generateIndices()
{
    m_indices.reserve(MAX_BATCH_SIZE * INDICES_PER_SPRITE);
    
    GLshort j = 0;
    for (int i = 0; i < MAX_BATCH_SIZE * INDICES_PER_SPRITE; i += INDICES_PER_SPRITE, j += VERTICES_PER_SPRITE)
    {
        m_indices.push_back(j);
        m_indices.push_back(j + 1);
        m_indices.push_back(j + 2);
        m_indices.push_back(j + 2);
        m_indices.push_back(j + 3);
        m_indices.push_back(j + 0);
    }
}