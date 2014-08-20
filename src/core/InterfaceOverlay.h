//
//  InterfaceOverlay.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__InterfaceOverlay__
#define __battlebombs__InterfaceOverlay__

#include "GameConstants.h"
#include "Color.h"
#include "GameState.h"
#include "SpectatorControlState.h"
#include <vector>
#include <memory>
#include <string>

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
class SpectatorControls;

class InterfaceOverlay
{
public:
	InterfaceOverlay(GameListener *gameListener);
    
    void initializeMiniMap(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType);
    
    void update(float deltaTime, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType, int playerIndex, Game_State gameState);
    
    void handleTouchDownInputRunning(Vector2D &touchPoint, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    void handleTouchDraggedInputRunning(Vector2D &touchPoint, PlayerDynamicGameObject &player);
    
    void handleTouchUpInputRunning(Vector2D &touchPoint, PlayerDynamicGameObject &player);
    
    bool handleTouchDownInputSpectating(Vector2D &touchPoint);
    
    bool handleTouchDraggedInputSpectating(Vector2D &touchPoint);
    
    Spectator_Control_State handleTouchUpInputSpectating(Vector2D &touchPoint);
    
    std::vector<std::unique_ptr<PlayerAvatar>> & getPlayerAvatars();
    
    DPadControl & getDPadControl();
    
    std::vector<std::unique_ptr<PowerUpBarItem>> & getPowerUpBarItems();
    
    ActiveButton & getActiveButton();
    
    BombButton & getBombButton();
    
    SpectatorControls & getSpectatorControls();
    
    float getPowerUpBarItemsStateTime();
    
    float getButtonsStateTime();
    
    void setNumSecondsLeft(int numSecondsLeft);
    
    int getNumMinutesLeft();
    
    int getNumSecondsLeftFirstColumn();
    
    int getNumSecondsLeftSecondColumn();
    
    int getMiniMapGridType(int x, int y);
    
    Color & getColorForMiniMapGridType(int miniMapGridType);
    
    std::string & getSpectatingUsername();
    
private:
    std::vector<std::unique_ptr<PlayerAvatar>> m_playerAvatars;
    std::unique_ptr<DPadControl> m_dPad;
    std::vector<std::unique_ptr<PowerUpBarItem>> m_powerUpBarItems;
    std::unique_ptr<ActiveButton> m_activeButton;
    std::unique_ptr<BombButton> m_bombButton;
    std::unique_ptr<SpectatorControls> m_spectatorControls;
    GameListener *m_gameListener;
    std::string m_spectatingWho;
    int m_miniMap[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS];
    float m_fPowerUpBarItemsStateTime;
    float m_fButtonsStateTime;
    float m_fCountdownStateTime;
    int m_iNumSecondsLeft;
    
    void updateMiniMap(int x, int y, int miniMapGridType);
};

#endif /* defined(__battlebombs__InterfaceOverlay__) */