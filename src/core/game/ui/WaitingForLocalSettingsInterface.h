//
//  WaitingForLocalSettingsInterface.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__WaitingForLocalSettingsInterface__
#define __battlebombs__WaitingForLocalSettingsInterface__

#include "GameObject.h"

#include <memory>
#include <vector>

class GameScreen;
class TouchEvent;
class Rectangle;
class StartButton;
class EnablePowerUpButton;
class EnableBotButton;

class WaitingForLocalSettingsInterface : public GameObject
{
public:
	WaitingForLocalSettingsInterface(float x, float y, float width, float height);
    
    void updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents);
    
    void reset();
    
    StartButton & getStartButton();
    
    std::vector<std::unique_ptr<EnableBotButton>> & getEnableBotButtons();
    
    std::vector<std::unique_ptr<EnablePowerUpButton>> & getEnablePowerUpButtons();
    
    int getChosenMapType();
    
    int getChosenBotFlags();
    
    int getChosenPowerUpFlags();
    
    bool isStartingGame();
    
private:
    std::unique_ptr<Rectangle> m_leftArrow;
    std::unique_ptr<Rectangle> m_RightArrow;
    std::unique_ptr<StartButton> m_startButton;
    std::vector<std::unique_ptr<EnableBotButton>> m_enableBotButtons;
    std::vector<std::unique_ptr<EnablePowerUpButton>> m_enablePowerUpButtons;
    int m_iChosenMapType;
    int m_iChosenBotFlags;
    int m_iChosenPowerUpFlags;
    bool m_isStartingGame;
    
    void startGame();
};

#endif /* defined(__battlebombs__WaitingForLocalSettingsInterface__) */