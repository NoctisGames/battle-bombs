//
//  OpenGLESRenderer.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomber_party__OpenGLESRenderer__
#define __bomber_party__OpenGLESRenderer__

#include "Renderer.h"

extern "C"
{
#include "platform_gl.h"
}

class SpriteBatcher;

class OpenGLESRenderer : public Renderer
{
public:
    OpenGLESRenderer(int width, int height);
    
    virtual void clearScreenWithColor(float r, float g, float b, float a);
    
    virtual void renderWorldBackground();
    
    virtual void renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);
    
    virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs);
    
    virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions);
    
    virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    virtual void renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear);
    
    virtual void renderInterface();
    
    virtual void endFrame();
    
    virtual void cleanUp();
    
private:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    GLuint m_gameTexture;
    GLuint m_charBlackTexture;
    GLuint m_charBlueTexture;
    GLuint m_charGreenTexture;
    GLuint m_charOrangeTexture;
    GLuint m_charPinkTexture;
    GLuint m_charRedTexture;
    GLuint m_charWhiteTexture;
    GLuint m_charYellowTexture;
    
    virtual void renderGameObject(GameObject &go, TextureRegion tr);
    
    virtual void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr);
};

#endif /* defined(__bomber_party__OpenGLESRenderer__) */