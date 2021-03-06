//
//  PlayerDynamicGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/6/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__PlayerDynamicGameObject__
#define __battlebombs__PlayerDynamicGameObject__

#include "GameConstants.h"
#include "DynamicGridGameObject.h"
#include "PlayerState.h"
#include "PlayerActionState.h"

#include <memory>
#include <vector>

class MapBorder;
class SpaceTile;
class InsideBlock;
class BreakableBlock;
class PowerUp;
class Explosion;
class GameListener;
class BombGameObject;
class Crater;
class IcePatch;
class BaseTile;
class RegeneratingDoor;
class Landmine;
class RemoteBomb;
class GameSession;

class PlayerDynamicGameObject : public DynamicGridGameObject
{
public:
    PlayerDynamicGameObject(GameSession &gameSession, short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction = DIRECTION_RIGHT, float width = PLAYER_FRAME_WIDTH, float height = PLAYER_FRAME_HEIGHT);

    virtual void update(float deltaTime, GameSession &gameSession);

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
    
    void onLandminePlaced(Landmine *landmine);
    
    bool isOwnerOfLandmine(Landmine *landmine);
    
    void raiseShield();
    
    void lowerShield();

    void onBombExploded();

    bool isHitByExplosion(std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    bool isTrappedOnFallingSpaceTile(std::vector<std::unique_ptr<SpaceTile>> &spaceTiles);
    
    bool isFallingDueToSpaceTile(std::vector<std::unique_ptr<SpaceTile>> &spaceTiles);
    
    bool isHitByFireBall(std::vector<std::unique_ptr<Crater >> &craters);
    
    bool isHitByIce(std::vector<std::unique_ptr<IcePatch >> &icePatches);
    
    bool isTriggeringLandmine(GameSession &gameSession);
    
    bool isTrappedOnExplodingBaseTile(std::vector<std::unique_ptr<BaseTile>> &baseTiles);
    
    void handlePowerUps(std::vector<std::unique_ptr<PowerUp >> &powerUps);
    
    void onForceFieldHit();

    void onTrappedOnFallingSpaceTile(GameSession &gameSession);
    
    void onHitByFireBall(GameSession &gameSession);
    
    void onFall(GameSession &gameSession);
    
    void onFreeze(GameSession &gameSession);
    
    void onDeath(GameSession &gameSession);
    
    void onWin();

    bool isAbleToDropAdditionalBomb(GameSession &gameSession);
    
    bool isAbleToPlaceLandmine(GameSession &gameSession);

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
    
    bool isUsingRemoteBombs();
    
    void reset(GameSession &gameSession);
    
    void handleBombErasure(BombGameObject *bomb);
    
    int getNumCurrentlyDeployedRemoteBombs();
    
    void detonateFirstRemoteBomb();
    
    void triggerLandmine();

protected:
    BombGameObject *m_lastBombDropped;
    Landmine *m_lastLandminePlaced;
    std::vector<RemoteBomb *> m_currentlyDeployedRemoteBombs;
    Player_State m_playerState;
    Player_Action_State m_playerActionState;
    float m_fStateTime;
    int m_iSpeed;
    int m_iDirection;
    int m_iMaxBombCount;
    int m_iCurrentBombCount;
    short m_firePower;
    int m_activePowerUp;
    short m_sPlayerIndex;
    int m_iPlayerForceFieldState;
    float m_fPlayerForceFieldStateTime;
    bool m_isUsingRemoteBombs;
    GameListener *m_gameListener;
    
private:
    std::unique_ptr<char> m_username;
    bool m_isClientPlayer;
    bool m_isBot;
    bool m_isDisplayingName;
    float m_fDisplayingPointerStateTime;
    bool m_isDisplayingPointer;
    
    bool isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<SpaceTile>> &spaceTiles, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<RegeneratingDoor>> &doors, std::vector<std::unique_ptr<Crater >> &craters, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    void setPlayerForceFieldState(int playerForceFieldState);
};

#endif /* defined(__battlebombs__PlayerDynamicGameObject__) */
