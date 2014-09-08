//
//  WaitingForServerInterface.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/4/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__WaitingForServerInterface__
#define __battlebombs__WaitingForServerInterface__

#include "GameObject.h"
#include "GameState.h"
#include "rapidjson/document.h"
#include <vector>

class WaitingForServerInterface : public GameObject
{
public:
	WaitingForServerInterface(float x, float y, float width, float height, const char *username);
    
    ~WaitingForServerInterface();
    
    void update(float deltaTime, Game_State gameState);
    
    void handlePreGameServerUpdate(rapidjson::Document &d);
    
    char * getUsername();
    
    std::vector<char *> & getPlayerNames();
    
    std::vector<int> & getPlayerPlatforms();
    
    int getTimeToNextRound();
    
    void setPreGamePhase(int preGamePhase);
    
    int getPreGamePhase();
    
    bool renderTimeToNextRound();
    
    bool renderPlayersList();
    
    bool renderMessage();
    
private:
    char *m_username;
    std::vector<char *> m_playerNames;
    std::vector<int> m_playerPlatforms;
    float m_fTimeToNextRound;
    int m_iPreGamePhase;
    bool m_renderTimeToNextRound;
    bool m_renderPlayersList;
    bool m_renderMessage;
    
    void handlePlayerNameAndPlatform(rapidjson::Document &d, const char *keyName, const char *keyPlatform);
};

#endif /* defined(__battlebombs__WaitingForServerInterface__) */
