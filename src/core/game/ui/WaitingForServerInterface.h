//
//  WaitingForServerInterface.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/4/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__WaitingForServerInterface__
#define __battlebombs__WaitingForServerInterface__

#include "GameObject.h"
#include "rapidjson/document.h"
#include <vector>

class PlayerRow;

class WaitingForServerInterface : public GameObject
{
public:
	WaitingForServerInterface(float x, float y, float width, float height, char *username);
    
    void update(float deltaTime);
    
    void handlePreGameServerUpdate(rapidjson::Document &d);
    
    char * getUsername();
    
    std::vector<std::unique_ptr<PlayerRow >> & getPlayerRows();
    
    int getTimeToNextRound();
    
    void setPreGamePhase(int preGamePhase);
    
    int getPreGamePhase();
    
    bool renderTimeToNextRound();
    
private:
    std::vector<std::unique_ptr<PlayerRow>> m_playerRows;
    char *m_username;
    float m_fTimeToNextRound;
    int m_iPreGamePhase;
    bool m_renderTimeToNextRound;
};

#endif /* defined(__battlebombs__WaitingForServerInterface__) */
