//
//  GameListener.h
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__GameListener__
#define __bomberparty__GameListener__

#ifdef TECHNE_GAMES_DIRECT_3D
#define strdup _strdup
#endif

#include <vector>

class GameListener
{
public:
    GameListener();
    
    void addEvent(short eventId);
    
    short getFirstEventId();
    
    void eraseFirstEventId();
    
    void addServerMessage(const char *serverMessage);
    
    std::vector<short> & freeLocalEventIds();
    
    std::vector<const char *> & freeServerMessages();
    
    void playSound(short soundId);

private:
    std::vector<const char *> m_serverMessages;
    std::vector<const char *> m_serverMessagesBuffer;
	std::vector<short> m_sLocalEventIds;
    std::vector<short> m_sLocalConsumedEventIds;
};

#endif /* defined(__bomberparty__GameListener__) */