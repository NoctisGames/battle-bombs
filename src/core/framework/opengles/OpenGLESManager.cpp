//
//  OpenGLESManager.cpp
//  insectoiddefense
//
//  Created by Stephen Gowen on 11/19/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESManager.h"
#include "GameConstants.h"
#include "macros.h"
#include <string.h>

extern "C"
{
#include "asset_utils.h"
}

OpenGLESManager * OpenGLESManager::getInstance()
{
    static OpenGLESManager *openGLESManager = new OpenGLESManager();
    return openGLESManager;
}

void OpenGLESManager::init(int width, int height)
{
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    
    buildShaderPrograms();
    generateIndices();
    createVertexBufferForSpriteBatcher();
    createVertexBufferForGeometryBatcher();
    createMatrix();
}

void OpenGLESManager::addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v)
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

void OpenGLESManager::addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    m_colorVertices.push_back(x);
    m_colorVertices.push_back(y);
    m_colorVertices.push_back(z);
    m_colorVertices.push_back(r);
    m_colorVertices.push_back(g);
    m_colorVertices.push_back(b);
    m_colorVertices.push_back(a);
}

void OpenGLESManager::prepareForSpriteRendering()
{
    glUseProgram(m_textureProgram.program);
    
    glUniformMatrix4fv(m_textureProgram.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m_viewProjectionMatrix);
    glUniform1i(m_textureProgram.u_texture_unit_location, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, sb_vbo_object);
    
    GLvoid* vbo_buffer = glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    memcpy(vbo_buffer, &m_textureVertices[0], sizeof(GLfloat) * m_textureVertices.size());
    glUnmapBufferOES(GL_ARRAY_BUFFER);
    
    glVertexAttribPointer(m_textureProgram.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    glVertexAttribPointer(m_textureProgram.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    glVertexAttribPointer(m_textureProgram.a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
    
    glEnableVertexAttribArray(m_textureProgram.a_position_location);
    glEnableVertexAttribArray(m_textureProgram.a_color_location);
    glEnableVertexAttribArray(m_textureProgram.a_texture_coordinates_location);
}

void OpenGLESManager::prepareForGeometryRendering()
{
    glUseProgram(m_colorProgram.program);
    
    glUniformMatrix4fv(m_colorProgram.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m_viewProjectionMatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, gb_vbo_object);
    
    GLvoid* vbo_buffer = glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    memcpy(vbo_buffer, &m_colorVertices[0], sizeof(GLfloat) * m_colorVertices.size());
    glUnmapBufferOES(GL_ARRAY_BUFFER);
    
    glVertexAttribPointer(m_colorProgram.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, BUFFER_OFFSET(0));
    glVertexAttribPointer(m_colorProgram.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    
    glEnableVertexAttribArray(m_colorProgram.a_position_location);
    glEnableVertexAttribArray(m_colorProgram.a_color_location);
}

void OpenGLESManager::finishRendering()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLESManager::cleanUp()
{
    glDeleteBuffers(1, &sb_vbo_object);
    glDeleteBuffers(1, &gb_vbo_object);
}

#pragma mark <Private>

void OpenGLESManager::buildShaderPrograms()
{
    m_textureProgram = OpenGLESProgram::get_texture_program(build_program_from_assets("texture_shader.vsh", "texture_shader.fsh"));
    m_colorProgram = OpenGLESProgram::get_color_program(build_program_from_assets("color_shader.vsh", "color_shader.fsh"));
}

void OpenGLESManager::generateIndices()
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

void OpenGLESManager::createVertexBufferForSpriteBatcher()
{
    for(int i = 0; i < MAX_BATCH_SIZE; i++)
    {
        addVertexCoordinate(0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    
    glGenBuffers(1, &sb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, sb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_textureVertices.size(), &m_textureVertices[0], GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLESManager::createVertexBufferForGeometryBatcher()
{
    for(int i = 0; i < MAX_BATCH_SIZE; i++)
    {
        addVertexCoordinate(0, 0, 0, 0, 0, 0, 0);
    }
    
    glGenBuffers(1, &gb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, gb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_colorVertices.size(), &m_colorVertices[0], GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLESManager::createMatrix()
{
    vec3 eye = { 0, 0, 1 };
    vec3 center = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };
    
    mat4x4_ortho(m_projectionMatrix, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
    mat4x4_look_at(m_viewMatrix, eye, center, up);
    
    mat4x4_mul(m_viewProjectionMatrix, m_projectionMatrix, m_viewMatrix);
}

OpenGLESManager::OpenGLESManager()
{
    // Hide Constructor for Singleton
}