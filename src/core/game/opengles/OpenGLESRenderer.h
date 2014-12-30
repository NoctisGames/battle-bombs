//
//  OpenGLESRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battle_bombs__OpenGLESRenderer__
#define __battle_bombs__OpenGLESRenderer__

#include "Renderer.h"

class OpenGLESRenderer : public Renderer
{
public:
    OpenGLESRenderer(int width, int height);
    
    virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    virtual void clearScreenWithColor(float r, float g, float b, float a);
    
    virtual void beginFrame();
    
    virtual void endFrame();
    
    virtual void cleanUp();
};

#endif /* defined(__battle_bombs__OpenGLESRenderer__) */