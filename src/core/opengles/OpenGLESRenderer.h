//
//  OpenGLESRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
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
    
    virtual void renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);
    
    virtual void renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface);
    
    virtual void renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface);
    
    virtual void endFrame();
    
    virtual void cleanUp();
};

#endif /* defined(__battle_bombs__OpenGLESRenderer__) */