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
#include "asset_utils.h"
#include "buffer.h"
#include "image.h"
#include "linmath.h"
#include "platform_gl.h"
#include "platform_asset_utils.h"
#include "program.h"
#include "shader.h"

static TextureProgram textureProgram;

static mat4x4 viewProjectionMatrix;

static const size_t MAX_BATCH_SIZE = 2048;
static const size_t VERTICES_PER_SPRITE = 4;
static const size_t INDICES_PER_SPRITE = 6;

OpenGLESSpriteBatcher::OpenGLESSpriteBatcher()
{
    m_iNumSprites = 0;
    
    textureProgram = get_texture_program(build_program_from_assets("shaders/texture_shader.vsh", "shaders/texture_shader.fsh"));
    
    generateIndices();
    
    mat4x4 viewMatrix;
    mat4x4_look_at(viewMatrix, (vec3){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1}, (vec3){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0}, (vec3){0, 1, 0});
    
    mat4x4 projectionMatrix;
    mat4x4_ortho(projectionMatrix, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -1.0, 1.0);
    
    mat4x4_mul(viewProjectionMatrix, viewMatrix, projectionMatrix);
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
        GLuint buffer = create_vbo(sizeof(TEXTURE_VERTEX) * m_iNumSprites, &m_textureVertices[0], GL_STATIC_DRAW);
        
        glUseProgram(textureProgram.program);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureWrapper.texture);
        glUniformMatrix4fv(textureProgram.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)viewProjectionMatrix);
        glUniform1i(textureProgram.u_texture_unit_location, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(textureProgram.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(TEXTURE_VERTEX), BUFFER_OFFSET(0));
        glVertexAttribPointer(textureProgram.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(TEXTURE_VERTEX), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
        glVertexAttribPointer(textureProgram.a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(TEXTURE_VERTEX), BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
        
        glEnableVertexAttribArray(textureProgram.a_position_location);
        glEnableVertexAttribArray(textureProgram.a_color_location);
        glEnableVertexAttribArray(textureProgram.a_texture_coordinates_location);
        
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
    TEXTURE_VERTEX tv = { x, y, z, r, g, b, a, u, v };
    m_textureVertices.push_back(tv);
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