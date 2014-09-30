//
//  program.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/29/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef Program_H
#define Program_H

#include "platform_gl.h"

typedef struct
{
	GLuint program;
    
    GLint u_mvp_matrix_location;
    GLint a_position_location;
    GLint a_color_location;
    GLint a_texture_coordinates_location;
    GLint u_texture_unit_location;
} TextureProgram;

typedef struct
{
	GLuint program;
    
    GLint u_mvp_matrix_location;
	GLint a_position_location;
    GLint a_color_location;
} ColorProgram;

TextureProgram get_texture_program(GLuint program);

ColorProgram get_color_program(GLuint program);

#endif /* Program_H */
