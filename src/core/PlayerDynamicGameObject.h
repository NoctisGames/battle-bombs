//
//  PlayerDynamicGameObject.h
//  bomberparty
//
//  Created by Stephen Gowen on 3/6/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__PlayerDynamicGameObject__
#define __bomberparty__PlayerDynamicGameObject__

#include "GameConstants.h"
#include "DynamicGridGameObject.h"
#include "PlayerState.h"
#include "PowerUpType.h"

#include <memory>
#include <vector>

class MapBorder;
class InsideBlock;
class BreakableBlock;
class PowerUp;
class Explosion;
class GameListener;
class BombGameObject;

class PlayerDynamicGameObject : public DynamicGridGameObject
{
public:
    PlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction = DIRECTION_RIGHT, float width = PLAYER_FRAME_WIDTH, float height = PLAYER_FRAME_HEIGHT);

    virtual void update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);

    float getStateTime();

    int getDirection();

    short getFirePower();

    void setDirection(int direction);

    void moveInDirection(int direction);

    bool isMoving();

    void onBombDropped(BombGameObject *bomb);

    void onBombExploded();

    bool isHitByExplosion(std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<BombGameObject >> &bombs);

    void onDeath();

    bool isAbleToDropAdditionalBomb(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);

    bool hasActivePowerUp();

	bool isBombInFrontOfPlayer(BombGameObject &bomb);

    Player_State getPlayerState();

    Power_Up_Type getActivePowerUp();

    short getPlayerIndex();
    
    virtual void resetBounds(float width, float height);
    
    virtual void updateBounds();
    
    virtual bool isBot();

protected:
    BombGameObject *lastBombDropped;
    Player_State m_playerState;
    float m_fStateTime;
    float m_fSpeed;
    int m_iDirection;
    int m_iMaxBombCount;
    int m_iCurrentBombCount;
    bool m_isMoving;
    short m_firePower;
    bool m_hasActivePowerUp;
    int m_activePowerUpIndex;
    Power_Up_Type m_activePowerUp;
    short m_sPlayerIndex;
    GameListener *m_gameListener;
    
private:
    bool isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
};

#endif /* defined(__bomberparty__PlayerDynamicGameObject__) */
