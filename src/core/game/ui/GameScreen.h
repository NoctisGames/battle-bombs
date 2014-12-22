//
//  GameScreen.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "GameSession.h"
#include "DPadControl.h"
#include "ActiveButton.h"

class GameListener;
class GameState;
class TouchEvent;
class Vector2D;
class Rectangle;
class Renderer;
class WaitingForServerInterface;
class WaitingForLocalSettingsInterface;
class InterfaceOverlay;
class CountDownNumberGameObject;
class DisplayXMovingGameObject;
class DisplayGameOverGameObject;

class GameScreen : public GameSession
{
public:
	GameScreen(const char *username, bool isOffline = false);
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual bool handleOnBackPressed() = 0;
    
    virtual void init();
    
    virtual void handleServerUpdate(const char *message);

	virtual void onResume();
	
	virtual void onPause();
    
    void beginGameOffline(int mapType, int numHumanPlayers, int numSecondsLeftForRound);
    
    void beginGame(rapidjson::Document &d);
    
    void beginSpectate(rapidjson::Document &d);
    
	void update(float deltaTime, std::vector<TouchEvent> &touchEvents);
    
    void present();
    
    void updateRunning(float deltaTime);
    
    void updateSpectating(float deltaTime);
    
    virtual void suddenDeath();
    
    void gameOver(rapidjson::Document &d);
    
    void spectateNextLivePlayer();
    
    void spectatePreviousLivePlayer();
	
    // Getters and Setters
    
	int getScreenState();
    
    void setScreenState(int screenState);
    
    void clearState();
    
    short getPlayerIndex();
    
    void setPlayerIndex(short playerIndex);
    
    float getPlayerX();
    
    float getPlayerY();
    
    int getPlayerDirection();
    
    bool isTimeToSendKeepAlive();

	void resetTimeSinceLastClientEvent();
    
    int getNumSecondsLeft();
    
    void setGameState(GameState *gameState);
    
    Renderer * getRenderer();
    
    Vector2D * getTouchPoint();
    
    PlayerDynamicGameObject * getPlayer();
    
    std::vector<std::unique_ptr<CountDownNumberGameObject>> & getCountDownNumberGameObjects();
    
    DisplayXMovingGameObject * getDisplayXMovingGameObject();
    
    std::vector<std::unique_ptr<DisplayGameOverGameObject>> & getDisplayGameOverGameObjects();
    
    WaitingForServerInterface * getWaitingForServerInterface();
    
    WaitingForLocalSettingsInterface * getWaitingForLocalSettingsInterface();
    
    InterfaceOverlay * getInterfaceOverlay();
    
    float getTimeSinceLastClientEvent();
    
    void setTimeSinceLastClientEvent(float timeSinceLastClientEvent);
    
    float getTimeSinceLastServerUpdate();
    
    void setTimeSinceLastServerUpdate(float timeSinceLastServerUpdate);
    
    float getTimeSinceGameOver();
    
    void setTimeSinceGameOver(float timeSinceGameOver);
    
    float getBlackCoverTransitionAlpha();
    
    void setBlackCoverTransitionAlpha(float blackCoverTransitionAlpha);
    
    bool hasDisplayed2();
    
    void setHasDisplayed2(bool hasDisplayed2);
    
    bool hasDisplayed1();
    
    void setHasDisplayed1(bool hasDisplayed1);

protected:
    std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Vector2D> m_touchPoint;
    int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;

private:
    std::unique_ptr<char> m_username;
    PlayerDynamicGameObject *m_player; // Set once we figure out which player index we are.
    std::unique_ptr<GameState> m_gameState;
    std::vector<std::unique_ptr<CountDownNumberGameObject>> m_countDownNumbers;
    std::unique_ptr<DisplayXMovingGameObject> m_displayXMovingGameObject;
    std::vector<std::unique_ptr<DisplayGameOverGameObject>> m_displayGameOvers;
    std::unique_ptr<WaitingForServerInterface> m_waitingForServerInterface;
    std::unique_ptr<WaitingForLocalSettingsInterface> m_waitingForLocalSettingsInterface;
    std::unique_ptr<InterfaceOverlay> m_interfaceOverlay;
    float m_fTimeSinceLastClientEvent;
    float m_fTimeSinceLastServerUpdate;
    float m_fTimeSinceGameOver;
    float m_fBlackCoverTransitionAlpha;
    int m_iScreenState;
    short m_sPlayerIndex;
    bool m_isOffline;
    bool m_hasDisplayed2;
    bool m_hasDisplayed1;
    
    void beginGame();
    
    void beginSpectating();
    
    void updateForOffline(float deltaTime);
    
    // Server Stuff
    
    bool beginCommon(rapidjson::Document &d, bool isBeginGame);
    
    void handleBreakableBlocksArrayInDocument(rapidjson::Document &d);
    
    virtual void clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame);
};

#endif /* GAME_SCREEN_H */