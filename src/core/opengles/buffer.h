//
//  buffer.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/29/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef Buffer_H
#define Buffer_H

#include "platform_gl.h"

#define BUFFER_OFFSET(i) ((void*)(i))

GLuint create_vbo(const GLsizeiptr size, const GLvoid* data, const GLenum usage);

#endif /* Buffer_H */
