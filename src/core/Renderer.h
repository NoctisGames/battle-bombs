//
//  Renderer.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomber_party__Renderer__
#define __bomber_party__Renderer__

#define WORLD_BACKGROUND_X SCREEN_WIDTH / 2
#define WORLD_BACKGROUND_Y SCREEN_HEIGHT
#define WORLD_BACKGROUND_WIDTH SCREEN_WIDTH
#define WORLD_BACKGROUND_HEIGHT SCREEN_HEIGHT * 2

#define INTERFACE_BACKGROUND_X 20.95522388059704f
#define INTERFACE_BACKGROUND_Y 1.61194029868423f
#define INTERFACE_BACKGROUND_WIDTH 6.08955223880592f
#define INTERFACE_BACKGROUND_HEIGHT 3.22388059736845f

#include <memory>
#include <vector>
#include "PowerUpType.h"

class GameObject;
class TextureRegion;
class DPadControl;
class ActiveButton;
class PlayerDynamicGameObject;
class InsideBlock;
class BreakableBlock;
class BombGameObject;
class Explosion;
class PowerUp;

class Renderer
{
public:
    Renderer();

	void calcScrollYForPlayer(PlayerDynamicGameObject &player);
    
    virtual void clearScreenWithColor(float r, float g, float b, float a) = 0;
    
    virtual void renderWorldBackground() = 0;
    
    virtual void renderWorldForeground(std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps) = 0;
    
	virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs) = 0;

	virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions) = 0;

    virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players) = 0;
    
    virtual void renderInterface() = 0;
    
    virtual void renderControls(DPadControl &dPadControl) = 0;

	virtual void renderActivePowerUpIcon(Power_Up_Type activePowerUp) = 0;
    
    virtual void endFrame() = 0;

	virtual void cleanUp() = 0;
    
protected:
    float m_fScrollY;
    
    virtual void renderGameObject(GameObject &go, TextureRegion tr) = 0;
    
    virtual void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr) = 0;
};

#endif /* defined(__bomber_party__Renderer__) */