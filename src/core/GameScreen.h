//
//  GameScreen.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "GameSession.h"
#include "DPadControl.h"
#include "ActiveButton.h"

class GameListener;
class TouchEvent;
class Vector2D;
class Rectangle;
class Renderer;
class WaitingForServerInterface;
class WaitingForLocalSettingsInterface;
class InterfaceOverlay;
class CountDownNumberGameObject;
class DisplayBattleGameObject;
class DisplayGameOverGameObject;

class GameScreen : public GameSession
{
public:
	GameScreen(const char *username, bool isOffline = false);
    
    ~GameScreen();
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual void platformResume() = 0;
    
    virtual void platformPause() = 0;
    
    virtual bool handleOnBackPressed() = 0;

	virtual void handleServerUpdate(const char *message);
    
    void init();

	void onResume();
	
	void onPause();
    
	void update(float deltaTime, std::vector<TouchEvent> &touchEvents);
    
    void present();
	
	int getState();
    
    void clearState();
    
    short getPlayerIndex();
    
    float getPlayerX();
    
    float getPlayerY();
    
    int getPlayerDirection();
    
    int popOldestEventId();
    
    bool isTimeToSendKeepAlive();

	void resetTimeSinceLastClientEvent();
    
    int getNumSecondsLeft();

protected:
    std::unique_ptr<Renderer> m_renderer;
    char *m_username;
    PlayerDynamicGameObject *m_player; // Set once we figure out which player index we are.
    short m_sPlayerIndex;
	std::unique_ptr<Vector2D> m_touchPoint;
	float m_fTimeSinceLastClientEvent;
    int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;
	int m_iScreenState;

private:
    std::vector<std::unique_ptr<CountDownNumberGameObject>> m_countDownNumbers;
    std::unique_ptr<DisplayBattleGameObject> m_displayBattle;
    std::vector<std::unique_ptr<DisplayGameOverGameObject>> m_displayGameOvers;
    std::unique_ptr<GameListener> m_gameListener;
    std::unique_ptr<WaitingForServerInterface> m_waitingForServerInterface;
    std::unique_ptr<WaitingForLocalSettingsInterface> m_waitingForLocalSettingsInterface;
    std::unique_ptr<InterfaceOverlay> m_interfaceOverlay;
    float m_fCountDownTimeLeft;
    bool m_isGameOver;
    float m_fTimeSinceGameOver;
    float m_fBlackCoverTransitionAlpha;
    bool m_isOffline;
    bool m_hasDisplayed2;
    bool m_hasDisplayed1;
    
    void updateInputWaitingForLocalSettings(std::vector<TouchEvent> &touchEvents);
    
    void updateInputConnectionErrorWaitingForInput(std::vector<TouchEvent> &touchEvents);
    
    virtual void updateRunning(float deltaTime);
    
    void updateInputRunning(std::vector<TouchEvent> &touchEvents);
    
    virtual void updateSpectating(float deltaTime);
    
    void updateInputSpectating(std::vector<TouchEvent> &touchEvents);
    
    void updateGameEnding(float deltaTime);
    
    void spectateNextLivePlayer();
    
    void spectatePreviousLivePlayer();
    
    // Server Stuff
    
    void processServerMessages();
    
    void beginGame(rapidjson::Document &d);
    
    void beginSpectate(rapidjson::Document &d);
    
    bool beginCommon(rapidjson::Document &d, bool isBeginGame);
    
    void gameOver(rapidjson::Document &d);
    
    void handleBreakableBlocksArrayInDocument(rapidjson::Document &d);
    
    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);
};

#endif /* GAME_SCREEN_H */