//
//  InterfaceOverlay.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 7/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "InterfaceOverlay.h"
#include "BombButton.h"
#include "ActiveButton.h"
#include "DPadControl.h"
#include "OverlapTester.h"
#include "PlayerDynamicGameObject.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "Fire.h"
#include "GameListener.h"
#include "GameEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "PowerUpBarItem.h"
#include "PlayerAvatar.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "MiniMapGridType.h"

InterfaceOverlay::InterfaceOverlay(GameListener *gameListener)
{
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  9.1880597025f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  8.5074626875f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  7.8268656725f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  7.1462686575f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 9.1880597025f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 8.5074626875f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 7.8268656725f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 7.1462686575f, 1.43283582089544f, 1.36119403f)));
    
    m_dPad = std::unique_ptr<DPadControl>(new DPadControl(2.95522388059704f, 3.0626865675f, 5.91044776119408f, 6.125373135f));
    
    m_powerUpBarItems.push_back(std::unique_ptr<PowerUpBarItem>(new PowerUpBarItem(8.95522388059692f, 0.85074626875f, 1.43283582089544f, 1.36119403f)));
    m_powerUpBarItems.push_back(std::unique_ptr<PowerUpBarItem>(new PowerUpBarItem(11.1044776119402f, 0.85074626875f, 1.43283582089544f, 1.36119403f)));
    m_powerUpBarItems.push_back(std::unique_ptr<PowerUpBarItem>(new PowerUpBarItem(13.25373134328348f, 0.85074626875f, 1.43283582089544f, 1.36119403f)));
    m_powerUpBarItems.push_back(std::unique_ptr<PowerUpBarItem>(new PowerUpBarItem(15.40298507462676f, 0.85074626875f, 1.43283582089544f, 1.36119403f)));
    
    m_activeButton = std::unique_ptr<ActiveButton>(new ActiveButton(20.3731342f, 1.40238805f, 2.3283582f, 2.3283582f));
    m_bombButton = std::unique_ptr<BombButton>(new BombButton(22.43283572f, 3.64119405f, 2.59701504f, 2.59701504f));
    
    m_gameListener = gameListener;
    m_fPowerUpBarItemsStateTime = 0;
    m_fButtonsStateTime = 0;
    m_fCountdownStateTime = 0;
    m_iNumSecondsLeft = 120;
}

void InterfaceOverlay::initializeMiniMap(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType)
{
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            m_miniMap[j][i] = MINI_MAP_FREE_SPACE;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        m_miniMap[gridX][gridY] = MINI_MAP_INSIDE_BLOCK;
    }
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        m_miniMap[gridX][gridY] = MINI_MAP_BREAKABLE_BLOCK;
    }
    
    // For Map Borders
    
    m_miniMap[0][0] = MINI_MAP_MAP_BORDER;
    m_miniMap[1][0] = MINI_MAP_MAP_BORDER;
    m_miniMap[2][0] = MINI_MAP_MAP_BORDER;
    m_miniMap[0][1] = MINI_MAP_MAP_BORDER;
    m_miniMap[1][1] = MINI_MAP_MAP_BORDER;
    m_miniMap[2][1] = MINI_MAP_MAP_BORDER;
    m_miniMap[0][2] = MINI_MAP_MAP_BORDER;
    m_miniMap[1][2] = MINI_MAP_MAP_BORDER;
    m_miniMap[2][2] = MINI_MAP_MAP_BORDER;
    
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 3][0] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 2][0] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 1][0] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 3][1] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 2][1] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 1][1] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 3][2] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 2][2] = MINI_MAP_MAP_BORDER;
    m_miniMap[NUM_GRID_CELLS_PER_ROW - 1][2] = MINI_MAP_MAP_BORDER;
    
    if(mapType == MAP_MOUNTAINS)
    {
        m_miniMap[6][GRID_CELL_NUM_ROWS - 1] = MINI_MAP_MAP_BORDER;
        m_miniMap[7][GRID_CELL_NUM_ROWS - 1] = MINI_MAP_MAP_BORDER;
        m_miniMap[8][GRID_CELL_NUM_ROWS - 1] = MINI_MAP_MAP_BORDER;
    }
}

void InterfaceOverlay::update(float deltaTime, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType)
{
    m_fPowerUpBarItemsStateTime += deltaTime;
    m_fButtonsStateTime += deltaTime;
    m_fCountdownStateTime += deltaTime;
    while(m_fCountdownStateTime > 1 && m_iNumSecondsLeft > 0)
    {
        m_fCountdownStateTime -= 1;
        m_iNumSecondsLeft--;
    }
    
    m_playerAvatars.at(0)->setPlayerIndex(players.at(0)->getPlayerIndex());
    m_playerAvatars.at(0)->setPlayerState(players.at(0)->getPlayerState());
    m_playerAvatars.at(0)->setIsBot(players.at(0)->isBot());
    
    m_playerAvatars.at(1)->setPlayerIndex(players.at(1)->getPlayerIndex());
    m_playerAvatars.at(1)->setPlayerState(players.at(1)->getPlayerState());
    m_playerAvatars.at(1)->setIsBot(players.at(1)->isBot());
    
    m_playerAvatars.at(2)->setPlayerIndex(players.at(2)->getPlayerIndex());
    m_playerAvatars.at(2)->setPlayerState(players.at(2)->getPlayerState());
    m_playerAvatars.at(2)->setIsBot(players.at(2)->isBot());
    
    m_playerAvatars.at(3)->setPlayerIndex(players.at(3)->getPlayerIndex());
    m_playerAvatars.at(3)->setPlayerState(players.at(3)->getPlayerState());
    m_playerAvatars.at(3)->setIsBot(players.at(3)->isBot());
    
    m_playerAvatars.at(4)->setPlayerIndex(players.at(4)->getPlayerIndex());
    m_playerAvatars.at(4)->setPlayerState(players.at(4)->getPlayerState());
    m_playerAvatars.at(4)->setIsBot(players.at(4)->isBot());
    
    m_playerAvatars.at(5)->setPlayerIndex(players.at(5)->getPlayerIndex());
    m_playerAvatars.at(5)->setPlayerState(players.at(5)->getPlayerState());
    m_playerAvatars.at(5)->setIsBot(players.at(5)->isBot());
    
    m_playerAvatars.at(6)->setPlayerIndex(players.at(6)->getPlayerIndex());
    m_playerAvatars.at(6)->setPlayerState(players.at(6)->getPlayerState());
    m_playerAvatars.at(6)->setIsBot(players.at(6)->isBot());
    
    m_playerAvatars.at(7)->setPlayerIndex(players.at(7)->getPlayerIndex());
    m_playerAvatars.at(7)->setPlayerState(players.at(7)->getPlayerState());
    m_playerAvatars.at(7)->setIsBot(players.at(7)->isBot());
    
    if(player.getMaxBombCount() > 1)
    {
        m_powerUpBarItems.at(0).get()->setPowerUpType(BOMB);
        m_powerUpBarItems.at(0).get()->setLevel(player.getMaxBombCount() - 1);
    }
    
    if(player.getFirePower() > 1)
    {
        m_powerUpBarItems.at(1).get()->setPowerUpType(FIRE);
        m_powerUpBarItems.at(1).get()->setLevel(player.getFirePower() - 1);
    }
    
    if(player.getSpeed() > 3)
    {
        m_powerUpBarItems.at(2).get()->setPowerUpType(SPEED);
        m_powerUpBarItems.at(2).get()->setLevel(player.getSpeed() - 3);
    }
    
    if(player.getPlayerActionState() == PUSHING_BOMB)
    {
        m_activeButton->setIsPressed(true);
    }
    else
    {
        m_activeButton->setIsPressed(false);
        m_activeButton->setPowerUpType(player.getActivePowerUp());
        
        m_activeButton->setButtonState(DISABLED);
        
        if(m_activeButton->getPowerUpType() == PUSH)
        {
            for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
            {
                if(player.isBombInFrontOfPlayer((**itr)))
                {
                    m_activeButton->setButtonState(ENABLED);
                    break;
                }
            }
        }
    }
    
    if(player.getPlayerActionState() == PLACING_BOMB)
    {
        m_bombButton->setIsPressed(true);
    }
    else
    {
        m_bombButton->setIsPressed(false);
        m_bombButton->setButtonState(player.isAbleToDropAdditionalBomb(players, bombs) ? ENABLED : DISABLED);
    }
    
    initializeMiniMap(insideBlocks, breakableBlocks, mapType);
    
    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
    {
        for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
        {
            int x = (*itr2)->getGridX();
            int y = (*itr2)->getGridY();
            if(x >= 0 && x <= NUM_GRID_CELLS_PER_ROW && y >= 0 && y <= GRID_CELL_NUM_ROWS)
            {
                m_miniMap[x][y] = MINI_MAP_FIRE;
            }
        }
    }
    
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        m_miniMap[(*itr)->getGridX()][(*itr)->getGridY()] = MINI_MAP_BOMB;
    }
    
    m_miniMap[players.at(0)->getGridX()][players.at(0)->getGridY()] = players.at(0)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_ONE : m_miniMap[players.at(0)->getGridX()][players.at(0)->getGridY()];
    m_miniMap[players.at(1)->getGridX()][players.at(1)->getGridY()] = players.at(1)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_TWO : m_miniMap[players.at(1)->getGridX()][players.at(1)->getGridY()];
    m_miniMap[players.at(2)->getGridX()][players.at(2)->getGridY()] = players.at(2)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_THREE : m_miniMap[players.at(2)->getGridX()][players.at(2)->getGridY()];
    m_miniMap[players.at(3)->getGridX()][players.at(3)->getGridY()] = players.at(3)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_FOUR : m_miniMap[players.at(3)->getGridX()][players.at(3)->getGridY()];
    m_miniMap[players.at(4)->getGridX()][players.at(4)->getGridY()] = players.at(4)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_FIVE : m_miniMap[players.at(4)->getGridX()][players.at(4)->getGridY()];
    m_miniMap[players.at(5)->getGridX()][players.at(5)->getGridY()] = players.at(5)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_SIX : m_miniMap[players.at(5)->getGridX()][players.at(5)->getGridY()];
    m_miniMap[players.at(6)->getGridX()][players.at(6)->getGridY()] = players.at(6)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_SEVEN : m_miniMap[players.at(6)->getGridX()][players.at(6)->getGridY()];
    m_miniMap[players.at(7)->getGridX()][players.at(7)->getGridY()] = players.at(7)->getPlayerState() == ALIVE ? MINI_MAP_PLAYER_EIGHT : m_miniMap[players.at(7)->getGridX()][players.at(7)->getGridY()];
}

void InterfaceOverlay::handleTouchDownInput(Vector2D &touchPoint, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    if(OverlapTester::isPointInRectangle(touchPoint, m_bombButton->getBounds()))
    {
        if(player.isAbleToDropAdditionalBomb(players, bombs))
        {
            m_gameListener->addLocalEventForPlayer(PLAYER_PLANT_BOMB, player);
        }
    }
    else if(OverlapTester::isPointInRectangle(touchPoint, m_activeButton->getBounds()))
    {
        // TODO, handle more actions like Shield, Throw, etc.
        switch(player.getActivePowerUp())
        {
            case PUSH :
                for(std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
                {
                    if(player.isBombInFrontOfPlayer(**itr))
                    {
                        m_gameListener->addLocalEventForPlayer(PLAYER_PUSH_BOMB, player);
                    }
                }
            default:
                break;
        }
    }
    else
    {
        int directionInput = m_dPad->getDirectionForTouchPoint(touchPoint);
        
        if(player.getDirection() != directionInput || !player.isMoving())
        {
            m_gameListener->addLocalEventForPlayer(directionInput + 1, player);
        }
    }
}

void InterfaceOverlay::handleTouchDraggedInput(Vector2D &touchPoint, PlayerDynamicGameObject &player)
{
    if(!OverlapTester::isPointInRectangle(touchPoint, m_bombButton->getBounds()) && !OverlapTester::isPointInRectangle(touchPoint, m_activeButton->getBounds()))
    {
        int directionInput = m_dPad->getDirectionForTouchPoint(touchPoint);
        
        if(player.getDirection() != directionInput || !player.isMoving())
        {
            m_gameListener->addLocalEventForPlayer(directionInput + 1, player);
        }
    }
}

void InterfaceOverlay::handleTouchUpInput(Vector2D &touchPoint, PlayerDynamicGameObject &player)
{
    if(!OverlapTester::isPointInRectangle(touchPoint, m_bombButton->getBounds()) && !OverlapTester::isPointInRectangle(touchPoint, m_activeButton->getBounds()))
    {
        m_dPad->stop();
        m_gameListener->addLocalEventForPlayer(PLAYER_MOVE_STOP, player);
    }
}

std::vector<std::unique_ptr<PlayerAvatar>> & InterfaceOverlay::getPlayerAvatars()
{
    return m_playerAvatars;
}

DPadControl & InterfaceOverlay::getDPadControl()
{
    return *m_dPad;
}

std::vector<std::unique_ptr<PowerUpBarItem>> & InterfaceOverlay::getPowerUpBarItems()
{
    return m_powerUpBarItems;
}

ActiveButton & InterfaceOverlay::getActiveButton()
{
    return *m_activeButton;
}

BombButton & InterfaceOverlay::getBombButton()
{
    return *m_bombButton;
}

float InterfaceOverlay::getPowerUpBarItemsStateTime()
{
    return m_fPowerUpBarItemsStateTime;
}

float InterfaceOverlay::getButtonsStateTime()
{
    return m_fButtonsStateTime;
}

void InterfaceOverlay::setNumSecondsLeft(int numSecondsLeft)
{
    m_iNumSecondsLeft = numSecondsLeft;
	m_fCountdownStateTime = 0;
}

int InterfaceOverlay::getNumMinutesLeft()
{
    int numMinutes = 0;
    int numSecondsLeft = m_iNumSecondsLeft;
    while (numSecondsLeft >= 60)
    {
        numSecondsLeft -= 60;
        numMinutes++;
    }
    
    return numMinutes;
}

int InterfaceOverlay::getNumSecondsLeftFirstColumn()
{
    int numSecondsLeft = m_iNumSecondsLeft;
    while (numSecondsLeft >= 60)
    {
        numSecondsLeft -= 60;
    }
    
    int numSecondsLeftFirstColumn = 0;
    while (numSecondsLeft >= 10)
    {
        numSecondsLeft -= 10;
        numSecondsLeftFirstColumn++;
    }
    
    return numSecondsLeftFirstColumn;
}

int InterfaceOverlay::getNumSecondsLeftSecondColumn()
{
    int numSecondsLeft = m_iNumSecondsLeft;
    while (numSecondsLeft >= 60)
    {
        numSecondsLeft -= 60;
    }
    
    while (numSecondsLeft >= 10)
    {
        numSecondsLeft -= 10;
    }
    
    return numSecondsLeft;
}

int InterfaceOverlay::getMiniMapGridType(int x, int y)
{
    return m_miniMap[x][y];
}

Color & InterfaceOverlay::getColorForMiniMapGridType(int miniMapGridType)
{
    static Color fireColor = Color { 0.85490196078431f, 0.62352941176471f, 0.08627450980392f, 1 };
    static Color bombColor = Color { 0.61960784313725f, 0.08627450980392f, 0.10588235294118f, 1 };
    
    static Color player1Color = Color { 0.22745098039216f, 0.22745098039216f, 0.22745098039216f, 1 };
    static Color player2Color = Color { 0.4f, 0.4f, 0.8f, 1 };
    static Color player3Color = Color { 0, 0.60392156862745f, 0.05098039215686f, 1 };
    static Color player4Color = Color { 1, 0.35686274509804f, 0.08235294117647f, 1 };
    static Color player5Color = Color { 0.98823529411765f, 0.21176470588235f, 0.52941176470588f, 1 };
    static Color player6Color = Color { 1, 0, 0, 1 };
    static Color player7Color = Color { 0.68235294117647f, 0.68235294117647f, 0.68235294117647f, 1 };
    static Color player8Color = Color { 0.97647058823529f, 0.74509803921569f, 0.10588235294118f, 1 };
    
    static Color insideBlockColor = Color { 0.4f, 0.4f, 0.4f, 1 };
    static Color breakableBlockColor = Color { 0.6f, 0.6f, 0.6f, 1 };
    static Color mapBorderColor = Color { 0.4f, 0.4f, 0.4f, 1 };
    
    switch (miniMapGridType)
    {
        case MINI_MAP_FIRE:
            return fireColor;
        case MINI_MAP_BOMB:
            return bombColor;
        case MINI_MAP_PLAYER_ONE:
            return player1Color;
        case MINI_MAP_PLAYER_TWO:
            return player2Color;
        case MINI_MAP_PLAYER_THREE:
            return player3Color;
        case MINI_MAP_PLAYER_FOUR:
            return player4Color;
        case MINI_MAP_PLAYER_FIVE:
            return player5Color;
        case MINI_MAP_PLAYER_SIX:
            return player6Color;
        case MINI_MAP_PLAYER_SEVEN:
            return player7Color;
        case MINI_MAP_PLAYER_EIGHT:
            return player8Color;
        case MINI_MAP_INSIDE_BLOCK:
            return insideBlockColor;
        case MINI_MAP_BREAKABLE_BLOCK:
            return breakableBlockColor;
        case MINI_MAP_MAP_BORDER:
        default:
            return mapBorderColor;
    }
}