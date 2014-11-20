//
//  GameListener.h
//  battlebombs
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__GameListener__
#define __battlebombs__GameListener__

#ifdef TECHNE_GAMES_DIRECT_3D
#define strdup _strdup
#endif

#include <vector>

class PlayerDynamicGameObject;

class GameListener
{
public:
    GameListener();

    void addLocalEventForPlayer(int eventId, PlayerDynamicGameObject &player);

    int popOldestEventId();

    void addServerMessage(const char *serverMessage);

    std::vector<int> & freeLocalEventIds();

    std::vector<const char *> & freeServerMessages();

    void playSound(short soundId);

private:
    std::vector<const char *> m_serverMessages;
    std::vector<const char *> m_serverMessagesBuffer;
    std::vector<int> m_sLocalEventIds;
    std::vector<int> m_sLocalConsumedEventIds;
};

#endif /* defined(__battlebombs__GameListener__) */