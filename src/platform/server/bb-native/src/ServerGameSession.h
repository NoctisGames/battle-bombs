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

class ServerGameSession : public GameSession
{
public:
    ServerGameSession();
    
    void initWithNumHumanPlayersAndMapType(int numHumanPlayers, int mapType);

    virtual void init();

    virtual void handleServerUpdate(const char *message);

    void update(float deltaTime);

    int getNumBreakableBlocks();

    int getBreakableBlockGridX(int breakableBlockIndex);

    int getBreakableBlockGridY(int breakableBlockIndex);

    int getBreakableBlockPowerUpFlag(int breakableBlockIndex);
    
    int getNumDeletedBreakableBlocks();
    
    int getDeletedBreakableBlockGridX(int breakableBlockIndex);
    
    int getDeletedBreakableBlockGridY(int breakableBlockIndex);
    
    int getNumDeletedPowerUps();
    
    int getDeletedPowerUpGridX(int powerUpIndex);
    
    int getDeletedPowerUpGridY(int powerUpIndex);
    
    int getNumBreakableBlocksAtSpawnTime();
    
    int popOldestEventId();
    
protected:
    virtual void onBreakableBlockDestroyed(BreakableBlock &breakableBlock);
    virtual void onPowerUpPickedUp(PowerUp &powerUp);

private:
    std::vector<int> m_deletedBreakableBlockXValues;
    std::vector<int> m_deletedBreakableBlockYValues;
    std::vector<int> m_deletedPowerUpsXValues;
    std::vector<int> m_deletedPowerUpsYValues;
    float m_fCountDownTimeLeft;
    
    virtual void updateRunning(float deltaTime);

    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);
    
    bool isLocationOccupiedByPlayer(int gridX, int gridY);
    
    // A utility function to swap to integers
    void swap(int *a, int *b);
    
    // A function to generate a random permutation of arr[]
    void randomize(int arr[][2], int n);
};

#endif /* SERVER_GAME_SESSION_H */