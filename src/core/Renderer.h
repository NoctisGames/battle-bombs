//
//  Renderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Renderer__
#define __battle_bombs__Renderer__

#define WORLD_BACKGROUND_X SCREEN_WIDTH / 2
#define WORLD_BACKGROUND_Y SCREEN_HEIGHT_X2 / 2
#define WORLD_BACKGROUND_WIDTH SCREEN_WIDTH
#define WORLD_BACKGROUND_HEIGHT SCREEN_HEIGHT_X2

#define INTERFACE_OVERLAY_BACKGROUND_X SCREEN_WIDTH / 2
#define INTERFACE_OVERLAY_BACKGROUND_Y SCREEN_HEIGHT / 2
#define INTERFACE_OVERLAY_BACKGROUND_WIDTH SCREEN_WIDTH
#define INTERFACE_OVERLAY_BACKGROUND_HEIGHT SCREEN_HEIGHT

#define INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_X SCREEN_WIDTH / 2
#define INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_Y 0.8932835821875
#define INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_WIDTH SCREEN_WIDTH
#define INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_HEIGHT 1.786567164375

#include <memory>
#include <vector>
#include "PowerUpType.h"
#include "GameConstants.h"
#include "TextureWrapper.h"

class GameObject;
class TextureRegion;
class ActiveButton;
class PlayerDynamicGameObject;
class MapBorder;
class InsideBlock;
class BreakableBlock;
class BombGameObject;
class Explosion;
class PowerUp;
class WaitingForServerInterface;
class WaitingForLocalSettingsInterface;
class InterfaceOverlay;
class Font;
class SpriteBatcher;
class RectangleBatcher;
class CountDownNumberGameObject;
class DisplayBattleGameObject;
class DisplayGameOverGameObject;

class Renderer
{
public:
    Renderer();

	void calcScrollYForPlayer(PlayerDynamicGameObject &player);
    
    virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players) = 0;
    
    virtual void clearScreenWithColor(float r, float g, float b, float a) = 0;
    
    virtual void beginFrame() = 0;
    
    virtual void renderWorldBackground();
    
    virtual void renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);
    
	virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs);

	virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions);

    virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    virtual void renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear);
    
    virtual void renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface);
    
    virtual void renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface);
    
    virtual void renderUIEffects(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<CountDownNumberGameObject>> &countDownNumbers, DisplayBattleGameObject &displayBattleGameObject, std::vector<std::unique_ptr<DisplayGameOverGameObject>> &displayGameOverGameObject);
    
    virtual void renderInterface(InterfaceOverlay &interfaceOverlay);
    
    virtual void renderSpectatorInterface(InterfaceOverlay &interfaceOverlay);
    
    virtual void renderGameOverBlackCover(float alpha);
    
    virtual void renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]);
    
    virtual void endFrame() = 0;

	virtual void cleanUp() = 0;
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<RectangleBatcher> m_rectangleBatcher;
	std::unique_ptr<Font> m_font;
    std::unique_ptr<TextureWrapper> m_gameTexture;
    std::unique_ptr<TextureWrapper> m_interfaceTexture;
    std::unique_ptr<TextureWrapper> m_interfaceTexture2;
    std::unique_ptr<TextureWrapper> m_charBlackTexture;
    std::unique_ptr<TextureWrapper> m_charBlueTexture;
    std::unique_ptr<TextureWrapper> m_charGreenTexture;
    std::unique_ptr<TextureWrapper> m_charOrangeTexture;
    std::unique_ptr<TextureWrapper> m_charPinkTexture;
    std::unique_ptr<TextureWrapper> m_charRedTexture;
    std::unique_ptr<TextureWrapper> m_charWhiteTexture;
    std::unique_ptr<TextureWrapper> m_charYellowTexture;
    bool player_sprites_loaded[8];
    float m_fScrollY;
    
    void renderGameObject(GameObject &go, TextureRegion tr);
    
    void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr);
    
    void updatePlayerSpritesLoadedArray(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
};

#endif /* defined(__battle_bombs__Renderer__) */