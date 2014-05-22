//
//  GameScreen.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "GameSession.h"
#include "DPadControl.h"
#include "ActiveButton.h"

class TouchEvent;
class Vector2D;
class Rectangle;

class GameScreen : public GameSession
{
public:
	GameScreen(const char *username);
    
    ~GameScreen();
    
    virtual void platformInit() = 0;
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual void platformResume() = 0;
    
    virtual void platformPause() = 0;
    
    virtual bool handleOnBackPressed() = 0;

	virtual void handleServerUpdate(const char *message);
    
    void init();

	void onResume();
	
	void onPause();
    
	void update(float deltaTime, std::vector<TouchEvent> &touchEvents);
	
	int getState();
    
    void clearState();
    
    short getPlayerIndex();
    
    float getPlayerX();
    
    float getPlayerY();
    
    int getPlayerDirection();
    
    short getFirstEventId();
    
    void eraseFirstEventId();
    
    bool isTimeToSendKeepAlive();

	void resetTimeSinceLastClientEvent();

protected:
    char *m_username;
    PlayerDynamicGameObject *m_player; // Set once we figure out which player index we are.
    short m_sPlayerIndex;
	std::unique_ptr<Vector2D> m_touchPoint;
    std::unique_ptr<Rectangle> m_bombButtonBounds;
	std::unique_ptr<ActiveButton> m_activeButton;
    std::unique_ptr<DPadControl> m_dPad;
	float m_fTimeSinceLastClientEvent;
    int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;
	int m_iScreenState;

private:
	std::vector<const char *> m_serverMessages;
    std::vector<const char *> m_serverMessagesBuffer;
	std::vector<short> m_sLocalEventIds;
    std::vector<short> m_sLocalConsumedEventIds;
    
    virtual void updateRunning(float deltaTime);
    
    void updateInputRunning(std::vector<TouchEvent> &touchEvents);
    
    virtual void updateSpectating(float deltaTime);
    
    void updateInputSpectating(std::vector<TouchEvent> &touchEvents);
    
    void updateCommon(float deltaTime);
    
    void spectateNextLivePlayer();
    
    void updatePlayerDirection();
    
    void addEvent(short eventId);
    
    // Server Stuff
    
    void processServerMessages();
    
    void beginGame(rapidjson::Document &d);
    
    void beginSpectate(rapidjson::Document &d);
    
    void handleBreakableBlocksArrayInDocument(rapidjson::Document &d);
    
    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex, bool isBeginGame);
};

#endif /* GAME_SCREEN_H */