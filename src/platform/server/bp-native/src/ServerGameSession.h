//
//  ServerGameSession.h
//  bomber-party
//
//  Created by Stephen Gowen on 3/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef SERVER_GAME_SESSION_H
#define SERVER_GAME_SESSION_H

#include "GameSession.h"

class GameListener;
class BotPlayerDynamicGameObject;

class ServerGameSession : public GameSession
{
public:
    ServerGameSession();
    
    void initWithNumHumanPlayers(int numHumanPlayers);

    virtual void init();

    virtual void handleServerUpdate(const char *message);

    void update(float deltaTime);

    int getNumBreakableBlocks();

    int getBreakableBlockGridX(short breakableBlockIndex);

    int getBreakableBlockGridY(short breakableBlockIndex);

    int getBreakableBlockPowerUpFlag(short breakableBlockIndex);
    
    short popOldestEventId();

private:
    std::unique_ptr<GameListener> m_gameListener;
    std::vector<std::unique_ptr<BotPlayerDynamicGameObject >> m_bots;
    
    virtual void updateRunning(float deltaTime);

    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);
    
    bool isLocationOccupiedByInsideBlock(int gridX, int gridY);
    
    bool isLocationOccupiedByPlayer(int gridX, int gridY);
    
    // A utility function to swap to integers
    void swap(int *a, int *b);
    
    // A function to generate a random permutation of arr[]
    void randomize(int arr[][2], int n);
};

#endif /* SERVER_GAME_SESSION_H */