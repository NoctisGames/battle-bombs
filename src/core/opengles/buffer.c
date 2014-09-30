//
//  buffer.c
//  battlebombs
//
//  Created by Stephen Gowen on 9/29/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "buffer.h"
#include <assert.h>
#include <stdlib.h>

GLuint create_vbo(const GLsizeiptr size, const GLvoid* data, const GLenum usage)
{
    assert(data != NULL);
	GLuint vbo_object;
	glGenBuffers(1, &vbo_object);
	assert(vbo_object != 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo_object;
}
