//
//  InterfaceOverlay.h
//  bomberparty
//
//  Created by Stephen Gowen on 7/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__InterfaceOverlay__
#define __bomberparty__InterfaceOverlay__

#include <vector>
#include <memory>

class Vector2D;
class DPadControl;
class ActiveButton;
class BombButton;
class PlayerDynamicGameObject;
class BombGameObject;
class GameListener;
class PowerUpBarItem;
class PlayerAvatar;

class InterfaceOverlay
{
public:
	InterfaceOverlay(GameListener *gameListener);
    
    void update(float deltaTime, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    void handleTouchDownInput(Vector2D &touchPoint, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    void handleTouchDraggedInput(Vector2D &touchPoint, PlayerDynamicGameObject &player);
    
    void handleTouchUpInput(Vector2D &touchPoint, PlayerDynamicGameObject &player);
    
    std::vector<std::unique_ptr<PlayerAvatar>> & getPlayerAvatars();
    
    DPadControl & getDPadControl();
    
    std::vector<std::unique_ptr<PowerUpBarItem>> & getPowerUpBarItems();
    
    ActiveButton & getActiveButton();
    
    BombButton & getBombButton();
    
    float getPowerUpBarItemsStateTime();
    
    float getButtonsStateTime();
    
    void setNumSecondsLeft(int numSecondsLeft);
    
    int getNumMinutesLeft();
    
    int getNumSecondsLeftFirstColumn();
    
    int getNumSecondsLeftSecondColumn();
    
private:
    std::vector<std::unique_ptr<PlayerAvatar>> m_playerAvatars;
    std::unique_ptr<DPadControl> m_dPad;
    std::vector<std::unique_ptr<PowerUpBarItem>> m_powerUpBarItems;
    std::unique_ptr<ActiveButton> m_activeButton;
    std::unique_ptr<BombButton> m_bombButton;
    GameListener *m_gameListener;
    float m_fPowerUpBarItemsStateTime;
    float m_fButtonsStateTime;
    float m_fCountdownStateTime;
    int m_iNumSecondsLeft;
};

#endif /* defined(__bomberparty__InterfaceOverlay__) */