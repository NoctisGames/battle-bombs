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

class ServerGameSession : public GameSession {
public:
    ServerGameSession();

    virtual void init();

    virtual void handleServerUpdate(const char *message);

    void update(float deltaTime);
    
    int getNumBreakableBlocks();
    
    int getBreakableBlockGridX(short breakableBlockIndex);
    
    int getBreakableBlockGridY(short breakableBlockIndex);
    
    int getBreakableBlockPowerUpFlag(short breakableBlockIndex);

private:
    virtual void updateRunning(float deltaTime);
    
    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex, bool isBeginGame);
};

#endif /* SERVER_GAME_SESSION_H */