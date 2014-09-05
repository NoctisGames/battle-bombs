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
#define WORLD_BACKGROUND_Y SCREEN_HEIGHT
#define WORLD_BACKGROUND_WIDTH SCREEN_WIDTH
#define WORLD_BACKGROUND_HEIGHT SCREEN_HEIGHT * 2

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
class InterfaceOverlay;
class Font;

class Renderer
{
public:
    Renderer();

	void calcScrollYForPlayer(PlayerDynamicGameObject &player);
    
    virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players) = 0;
    
    virtual void clearScreenWithColor(float r, float g, float b, float a) = 0;
    
    virtual void beginFrame() = 0;
    
    virtual void renderWorldBackground() = 0;
    
    virtual void renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps) = 0;
    
	virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs) = 0;

	virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions) = 0;

    virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players) = 0;
    
    virtual void renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear) = 0;
    
    virtual void renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface) = 0;
    
    virtual void renderInterface(InterfaceOverlay &interfaceOverlay) = 0;
    
    virtual void renderSpectatorInterface(InterfaceOverlay &interfaceOverlay) = 0;
    
    virtual void renderGameOverBlackCover(float alpha) = 0;
    
    virtual void renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]) = 0;
    
    virtual void endFrame() = 0;

	virtual void cleanUp() = 0;
    
protected:
    std::unique_ptr<Font> m_font;
    bool player_sprites_loaded[8];
    float m_fScrollY;
    
    virtual void renderGameObject(GameObject &go, TextureRegion tr) = 0;
    
    virtual void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr) = 0;
};

#endif /* defined(__battle_bombs__Renderer__) */