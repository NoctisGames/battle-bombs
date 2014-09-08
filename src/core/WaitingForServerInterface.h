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

class WaitingForServerInterface : public GameObject
{
public:
	WaitingForServerInterface(float x, float y, float width, float height, const char *username);
    
    void update(float deltaTime, Game_State gameState);
    
    char * getUsername();
    
    void setTimeToNextRound(float timeToNextRound);
    
    int getTimeToNextRound();
    
    void setPreGamePhase(int preGamePhase);
    
    int getPreGamePhase();
    
    bool renderTimeToNextRound();
    
    bool renderPlayersList();
    
    bool renderMessage();
    
private:
    char *m_username;
    float m_fTimeToNextRound;
    int m_iPreGamePhase;
    bool m_renderTimeToNextRound;
    bool m_renderPlayersList;
    bool m_renderMessage;
};

#endif /* defined(__battlebombs__WaitingForServerInterface__) */
