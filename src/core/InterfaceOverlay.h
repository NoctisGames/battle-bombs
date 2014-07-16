//
//  InterfaceOverlay.h
//  bomberparty
//
//  Created by Stephen Gowen on 7/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__InterfaceOverlay__
#define __bomberparty__InterfaceOverlay__

#include "GameConstants.h"
#include "Color.h"
#include <vector>
#include <memory>

class Vector2D;
class DPadControl;
class ActiveButton;
class BombButton;
class PlayerDynamicGameObject;
class BombGameObject;
class Explosion;
class GameListener;
class PowerUpBarItem;
class PlayerAvatar;
class InsideBlock;
class BreakableBlock;
class Rectangle;

class InterfaceOverlay
{
public:
	InterfaceOverlay(GameListener *gameListener);
    
    void initializeMiniMap(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType);
    
    void update(float deltaTime, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType);
    
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
    
    int getMiniMapGridType(int x, int y);
    
    Color & getColorForMiniMapGridType(int miniMapGridType);
    
private:
    std::vector<std::unique_ptr<PlayerAvatar>> m_playerAvatars;
    std::unique_ptr<DPadControl> m_dPad;
    std::vector<std::unique_ptr<PowerUpBarItem>> m_powerUpBarItems;
    std::unique_ptr<ActiveButton> m_activeButton;
    std::unique_ptr<BombButton> m_bombButton;
    GameListener *m_gameListener;
    int m_miniMap[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS];
    float m_fPowerUpBarItemsStateTime;
    float m_fButtonsStateTime;
    float m_fCountdownStateTime;
    int m_iNumSecondsLeft;
    
    void updateMiniMap(int x, int y, int miniMapGridType);
};

#endif /* defined(__bomberparty__InterfaceOverlay__) */