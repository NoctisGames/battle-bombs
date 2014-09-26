//
//  TextureWrapper.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef battlebombs_TextureWrapper_h
#define battlebombs_TextureWrapper_h

#ifdef TECHNE_GAMES_OPENGL_ES
extern "C"
{
#include "platform_gl.h"
}
#elif defined TECHNE_GAMES_DIRECT_3D
// TODO
#endif

struct TextureWrapper
{
#ifdef TECHNE_GAMES_OPENGL_ES
    GLuint texture;
    
    TextureWrapper(GLuint textureIn) : texture(textureIn)
    {
        // Empty
    }
#elif defined TECHNE_GAMES_DIRECT_3D
    ID3D11ShaderResourceView *texture;
    
    TextureWrapper(ID3D11ShaderResourceView textureIn) : texture(textureIn)
    {
        // Empty
    }
#endif
};

#endif /* defined(battlebombs_TextureWrapper_h) */
