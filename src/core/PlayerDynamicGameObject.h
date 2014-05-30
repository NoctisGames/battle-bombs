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
#include "DynamicGameObject.h"
#include "PlayerState.h"
#include "PowerUpType.h"

#include <memory>
#include <vector>

class InsideBlock;
class BreakableBlock;
class PowerUp;
class Explosion;
class GameListener;

class PlayerDynamicGameObject : public DynamicGameObject
{
public:
    PlayerDynamicGameObject(short playerIndex, float x, float y, GameListener *gameListener, int direction = DIRECTION_RIGHT, float width = PLAYER_WIDTH, float height = PLAYER_HEIGHT);

    virtual void update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions);

    float getStateTime();

    int getDirection();

    short getFirePower();

    void setDirection(int direction);

    void moveInDirection(int direction);

    bool isMoving();

    void onBombDropped();

    void onBombExploded();

    bool isHitByExplosion(std::vector<std::unique_ptr<Explosion >> &explosions);

    void onDeath();

    bool isAbleToDropAdditionalBomb();

    bool hasActivePowerUp();

    Player_State getPlayerState();

    Power_Up_Type getActivePowerUp();

    short getPlayerIndex();
    
    virtual bool isBot();

protected:
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
};

#endif /* defined(__bomberparty__PlayerDynamicGameObject__) */
