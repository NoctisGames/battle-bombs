//
//  GameSession.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include <vector>
#include <memory>

#include "rapidjson/document.h"

#include "GameState.h"

class GameListener;
class MapBorder;
class SpaceTile;
class InsideBlock;
class BreakableBlock;
class BombGameObject;
class Explosion;
class PlayerDynamicGameObject;
class PowerUp;
class Crater;
class FireBall;
class IceBall;
class IcePatch;

class GameSession
{
public:
    GameSession();

    virtual void init() = 0;

    virtual void handleServerUpdate(const char *message) = 0;
    
    int getNumPlayers();
    
    bool isPlayerBotAtIndex(short playerIndex);

    float getPlayerXAtIndex(short playerIndex);

    float getPlayerYAtIndex(short playerIndex);

    int getPlayerDirectionAtIndex(short playerIndex);

    bool isPlayerAliveAtIndex(short playerIndex);

protected:
    std::unique_ptr<GameListener> m_gameListener;
    std::vector<std::unique_ptr<MapBorder>> m_mapBorders;
    std::vector<std::unique_ptr<SpaceTile>> m_spaceTiles;
    std::vector<std::unique_ptr<InsideBlock >> m_insideBlocks;
    std::vector<std::unique_ptr<BreakableBlock >> m_breakableBlocks;
    std::vector<std::unique_ptr<PlayerDynamicGameObject >> m_players;
    std::vector<std::unique_ptr<BombGameObject >> m_bombs;
    std::vector<std::unique_ptr<Explosion >> m_explosions;
    std::vector<std::unique_ptr<PowerUp >> m_powerUps;
    std::vector<std::unique_ptr<Crater >> m_craters;
    std::vector<std::unique_ptr<FireBall >> m_fireBalls;
    std::vector<std::unique_ptr<IceBall >> m_iceBalls;
    std::vector<std::unique_ptr<IcePatch >> m_icePatches;
    std::vector<int> m_sEventIds;
    Game_State m_gameState;
    int m_iMapType;
    int m_iNumBreakableBlocksAtSpawnTime;
    bool m_isSuddenDeath;

    virtual void updateRunning(float deltaTime) = 0;

    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame) = 0;
    
    void initializeInsideBlocksAndMapBordersForMapType(int mapType);
    
    void updateCommon(float deltaTime);
    
    void updateBots();

    void clientUpdate(rapidjson::Document &d, bool isBeginGame);
    
    void hardUpdate(rapidjson::Document &d);
    
    virtual void suddenDeath();

    void handlePlayerDataUpdate(rapidjson::Document &d, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);

    void handleClientEventsArrayInDocument(rapidjson::Document &d);

    void handleIntArrayInDocument(rapidjson::Document &d, const char *intArrayKey, std::vector<int> &intArray, int sentinelValue);
    
    void readCharArrayIntoIntArray(const char *charArray, std::vector<int> &intArray, int sentinelValue);

    void handlePlayerEvent(int event);
    
    // For ServerGameSession to override
    virtual void onBreakableBlockDestroyed(BreakableBlock &breakableBlock);
    virtual void onPowerUpPickedUp(PowerUp &powerUp);

private:
    void layBombForPlayer(PlayerDynamicGameObject *player, int firePower);
    
    void pushBombForPlayer(PlayerDynamicGameObject *player);
};

#endif /* GAME_SESSION_H */