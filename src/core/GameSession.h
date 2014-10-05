//
//  GameSession.h
//  battlebombs
//
//  Created by Stephen Gowen on 3/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#ifdef TECHNE_GAMES_DIRECT_3D
#define strdup _strdup
#endif

#include <vector>
#include <memory>

#include "rapidjson/document.h"

#include "GameState.h"

class MapBorder;
class InsideBlock;
class BreakableBlock;
class BombGameObject;
class Explosion;
class PlayerDynamicGameObject;
class PowerUp;

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
    std::vector<std::unique_ptr<MapBorder>> m_mapBorders;
    std::vector<std::unique_ptr<InsideBlock >> m_insideBlocks;
    std::vector<std::unique_ptr<BreakableBlock >> m_breakableBlocks;
    std::vector<std::unique_ptr<PlayerDynamicGameObject >> m_players;
    std::vector<std::unique_ptr<BombGameObject >> m_bombs;
    std::vector<std::unique_ptr<Explosion >> m_explosions;
    std::vector<std::unique_ptr<PowerUp >> m_powerUps;
    std::vector<int> m_sEventIds;
    Game_State m_gameState;
    int m_iMapType;

    virtual void updateRunning(float deltaTime) = 0;

    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame) = 0;
    
    void initializeInsideBlocksAndMapBordersForMapType(int mapType);
    
    void updateCommon(float deltaTime);

    void clientUpdate(rapidjson::Document &d, bool isBeginGame);
    
    virtual void suddenDeath(rapidjson::Document &d);

    void handlePlayerDataUpdate(rapidjson::Document &d, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);

    void handleClientEventsArrayInDocument(rapidjson::Document &d);

    void handleIntArrayInDocument(rapidjson::Document &d, const char *intArrayKey, std::vector<int> &intArray, int sentinelValue);

    void handlePlayerEvent(int event);

private:
    void layBombForPlayer(PlayerDynamicGameObject *player, int firePower);
    
    void pushBombForPlayer(PlayerDynamicGameObject *player);
};

#endif /* GAME_SESSION_H */
