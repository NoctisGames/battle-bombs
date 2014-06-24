//
//  GameSession.h
//  bomber-party
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
    std::vector<short> m_sEventIds;
    Game_State m_gameState;

    virtual void updateRunning(float deltaTime) = 0;

    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex, bool isBeginGame) = 0;
    
    void updateCommon(float deltaTime);

    void clientUpdate(rapidjson::Document &d, bool isBeginGame);

    void handlePositionAndDirectionUpdate(rapidjson::Document &d, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex);

    void handleClientEventsArrayInDocument(rapidjson::Document &d);

    void handleShortArrayInDocument(rapidjson::Document &d, const char *shortArrayKey, std::vector<short> &shortArray, short sentinelValue);

    void handlePlayerEvent(short event);

private:
    void layBombForPlayer(PlayerDynamicGameObject *player);
    
    void pushBombForPlayer(PlayerDynamicGameObject *player);
};

#endif /* GAME_SESSION_H */
