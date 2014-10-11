//
//  PlayerDynamicGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/6/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__PlayerDynamicGameObject__
#define __battlebombs__PlayerDynamicGameObject__

#include "GameConstants.h"
#include "DynamicGridGameObject.h"
#include "PlayerState.h"
#include "PlayerActionState.h"
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
class IcePatch;

class PlayerDynamicGameObject : public DynamicGridGameObject
{
public:
    PlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction = DIRECTION_RIGHT, float width = PLAYER_FRAME_WIDTH, float height = PLAYER_FRAME_HEIGHT);

    virtual void update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);

    float getStateTime();

    int getDirection();
    
    float getPlayerForceFieldStateTime();
    
    int getPlayerForceFieldState();

    short getFirePower();
    
    int getMaxBombCount();
    
    int getSpeed();
    
    void collectPowerUp(int powerUpFlag);

    void setDirection(int direction);

    void moveInDirection(int direction);

    bool isMoving();

    void onBombDropped(BombGameObject *bomb);
    
    void onBombPushed(BombGameObject *bomb);
    
    void raiseShield();
    
    void lowerShield();

    void onBombExploded();

    bool isHitByExplosion(std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    bool isHitByIce(std::vector<std::unique_ptr<IcePatch >> &icePatches);
    
    void handlePowerUps(std::vector<std::unique_ptr<PowerUp >> &powerUps);
    
    void onForceFieldHit();

    void onTrappedOnFallingSpaceTile();
    
    void onFall();
    
    void onFreeze();
    
    void onDeath();
    
    void onWin();

    bool isAbleToDropAdditionalBomb(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);

	bool isBombInFrontOfPlayer(BombGameObject &bomb);
    
    void setPlayerState(Player_State playerState);

    Player_State getPlayerState();
    
    Player_Action_State getPlayerActionState();

    int getActivePowerUp();

    short getPlayerIndex();
    
    virtual void resetBounds(float width, float height);
    
    virtual void updateBounds();
    
    virtual Rectangle & getBoundsForGridLogic();
    
    virtual bool isBot();
    
    char * getUsername();
    
    void setUsername(const char *username);
    
    void setClientPlayer(bool isClientPlayer);
    
    void setIsBot(bool isBot);
    
    void setIsDisplayingName(bool isDisplayingName);
    
    bool isDisplayingName();
    
    void setIsDisplayingPointer(bool isDisplayingPointer);
    
    bool isDisplayingPointer();
    
    void reset();

protected:
    BombGameObject *m_lastBombDropped;
    Player_State m_playerState;
    Player_Action_State m_playerActionState;
    float m_fStateTime;
    int m_iSpeed;
    int m_iDirection;
    int m_iMaxBombCount;
    int m_iCurrentBombCount;
    short m_firePower;
    int m_activePowerUpIndex;
    int m_activePowerUp;
    short m_sPlayerIndex;
    int m_iPlayerForceFieldState;
    float m_fPlayerForceFieldStateTime;
    GameListener *m_gameListener;
    
private:
    std::unique_ptr<char> m_username;
    bool m_isClientPlayer;
    bool m_isBot;
    bool m_isDisplayingName;
    float m_fDisplayingPointerStateTime;
    bool m_isDisplayingPointer;
    
    bool isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    void setPlayerForceFieldState(int playerForceFieldState);
};

#endif /* defined(__battlebombs__PlayerDynamicGameObject__) */
