//
//  program.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/29/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "program.h"

TextureProgram get_texture_program(GLuint program)
{
	return (TextureProgram)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color"),
        glGetAttribLocation(program, "a_TextureCoordinates"),
        glGetUniformLocation(program, "u_TextureUnit")
    };
}

ColorProgram get_color_program(GLuint program)
{
	return (ColorProgram)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color")
    };
}
