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

    void update(float deltaTime);
    
    int getNumPlayers();
    
    const char * getPlayerNameAtIndex(short playerIndex);
    
    bool isPlayerBotAtIndex(short playerIndex);
    
    bool isPlayerAliveAtIndex(short playerIndex);

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
    
    virtual void onBreakableBlockDestroyed(BreakableBlock &breakableBlock);
    virtual void onPowerUpPickedUp(PowerUp &powerUp);

private:
    std::vector<int> m_deletedBreakableBlockXValues;
    std::vector<int> m_deletedBreakableBlockYValues;
    std::vector<int> m_deletedPowerUpsXValues;
    std::vector<int> m_deletedPowerUpsYValues;
    int m_gameState;
    
    void updateRunning(float deltaTime);

    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);
};

#endif /* SERVER_GAME_SESSION_H */