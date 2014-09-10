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

extern "C"
{
#include "platform_gl.h"
}

class SpriteBatcher;
class OpenGLESRectangleRenderer;

class OpenGLESRenderer : public Renderer
{
public:
    OpenGLESRenderer(int width, int height);
    
    ~OpenGLESRenderer();
    
    virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    virtual void clearScreenWithColor(float r, float g, float b, float a);
    
    virtual void beginFrame();
    
    virtual void renderWorldBackground();
    
    virtual void renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);
    
    virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs);
    
    virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions);
    
    virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    virtual void renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear);
    
    virtual void renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface);
    
    virtual void renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface);
    
    virtual void renderUIEffects(std::vector<std::unique_ptr<CountDownNumberGameObject>> &countDownNumbers, DisplayBattleGameObject &displayBattleGameObject, std::vector<std::unique_ptr<DisplayGameOverGameObject>> &displayGameOverGameObject);
    
    virtual void renderInterface(InterfaceOverlay &interfaceOverlay);
    
    virtual void renderSpectatorInterface(InterfaceOverlay &interfaceOverlay);
    
    virtual void renderGameOverBlackCover(float alpha);
    
    virtual void renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]);
    
    virtual void endFrame();
    
    virtual void cleanUp();
    
private:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<SpriteBatcher> m_spriteBatcherWithColor;
    std::unique_ptr<OpenGLESRectangleRenderer> m_rectangleRenderer;
    GLuint m_gameTexture;
    GLuint m_interfaceTexture;
    GLuint m_interfaceTexture2;
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

#endif /* defined(__battle_bombs__OpenGLESRenderer__) */