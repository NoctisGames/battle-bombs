//
//  InterfaceOverlay.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 7/1/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
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
#include "SpectatorControls.h"

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
    
    m_spectatorControls = std::unique_ptr<SpectatorControls>(new SpectatorControls(21.44776119402984f, 0.8932835821875f, 5.10447761194032f, 1.786567164375f));
    
    m_gameListener = gameListener;
    m_fPowerUpBarItemsStateTime = 0;
    m_fButtonsStateTime = 0;
    m_fCountdownStateTime = 0;
    m_iNumSecondsLeft = 180;
}

void InterfaceOverlay::initializeMiniMap(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType)
{
	for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
		{
			updateMiniMap(j, i, MINI_MAP_FREE_SPACE);
		}
	}

	for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
	{
		int gridX = (*itr)->getGridX();
		int gridY = (*itr)->getGridY();
		updateMiniMap(gridX, gridY, MINI_MAP_INSIDE_BLOCK);
	}

	for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
	{
		int gridX = (*itr)->getGridX();
		int gridY = (*itr)->getGridY();
		updateMiniMap(gridX, gridY, MINI_MAP_BREAKABLE_BLOCK);
	}

	// For Map Borders

	updateMiniMap(0, 0, MINI_MAP_MAP_BORDER);
	updateMiniMap(1, 0, MINI_MAP_MAP_BORDER);
	updateMiniMap(2, 0, MINI_MAP_MAP_BORDER);
	updateMiniMap(0, 1, MINI_MAP_MAP_BORDER);
	updateMiniMap(1, 1, MINI_MAP_MAP_BORDER);
	updateMiniMap(2, 1, MINI_MAP_MAP_BORDER);
	updateMiniMap(0, 2, MINI_MAP_MAP_BORDER);
	updateMiniMap(1, 2, MINI_MAP_MAP_BORDER);
	updateMiniMap(2, 2, MINI_MAP_MAP_BORDER);

	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 3, 0, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 2, 0, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 1, 0, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 3, 1, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 2, 1, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 1, 1, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 3, 2, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 2, 2, MINI_MAP_MAP_BORDER);
	updateMiniMap(NUM_GRID_CELLS_PER_ROW - 1, 2, MINI_MAP_MAP_BORDER);

	if (mapType == MAP_MOUNTAINS)
	{
		updateMiniMap(6, GRID_CELL_NUM_ROWS - 1, MINI_MAP_MAP_BORDER);
		updateMiniMap(7, GRID_CELL_NUM_ROWS - 1, MINI_MAP_MAP_BORDER);
		updateMiniMap(8, GRID_CELL_NUM_ROWS - 1, MINI_MAP_MAP_BORDER);
	}
}

void InterfaceOverlay::update(float deltaTime, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int mapType, int playerIndex, Game_State gameState)
{
    // Update Timer
    
    m_fCountdownStateTime += deltaTime;
    while(m_fCountdownStateTime > 1 && m_iNumSecondsLeft > 0)
    {
        m_fCountdownStateTime -= 1;
        m_iNumSecondsLeft--;
    }
    
    // Update PowerUp Bar and Buttons if the player is still alive
    
	if (gameState == RUNNING || gameState == COUNTING_DOWN)
    {
        m_fPowerUpBarItemsStateTime += deltaTime;
        m_fButtonsStateTime += deltaTime;
        
        for (int i = 0; i < m_playerAvatars.size(); i++)
        {
            m_playerAvatars.at(i)->setPlayerIndex(players.at(i)->getPlayerIndex());
            m_playerAvatars.at(i)->setPlayerState(players.at(i)->getPlayerState());
            m_playerAvatars.at(i)->setIsBot(players.at(i)->isBot());
        }
        
        if(player.getMaxBombCount() > 1)
        {
            m_powerUpBarItems.at(0).get()->setPowerUpType(POWER_UP_TYPE_BOMB);
            m_powerUpBarItems.at(0).get()->setLevel(player.getMaxBombCount() - 1);
        }
        
        if(player.getFirePower() > 1)
        {
            m_powerUpBarItems.at(1).get()->setPowerUpType(POWER_UP_TYPE_FIRE);
            m_powerUpBarItems.at(1).get()->setLevel(player.getFirePower() - 1);
        }
        
        if(player.getSpeed() > 3)
        {
            m_powerUpBarItems.at(2).get()->setPowerUpType(POWER_UP_TYPE_SPEED);
            m_powerUpBarItems.at(2).get()->setLevel(player.getSpeed() - 3);
        }
        
        if(player.getPlayerActionState() == PUSHING_BOMB || player.getPlayerActionState() == RAISING_SHIELD || player.getPlayerActionState() == SHIELD_RAISED)
        {
            m_activeButton->setIsPressed(true);
        }
        else
        {
            m_activeButton->setIsPressed(false);
            m_activeButton->setPowerUpType(player.getActivePowerUp());
            m_activeButton->setButtonState(DISABLED);
            
            if(m_activeButton->getPowerUpType() == POWER_UP_TYPE_PUSH)
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
            else if(m_activeButton->getPowerUpType() == POWER_UP_TYPE_SHIELD)
            {
                m_activeButton->setButtonState(ENABLED);
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
    }
    else if(gameState == SPECTATING)
    {
        m_spectatingWho = std::string(players.at(playerIndex)->getUsername());
    }
    
    // Update Mini Map
    
    initializeMiniMap(insideBlocks, breakableBlocks, mapType);
    
    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
    {
        for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
        {
            int x = (*itr2)->getGridX();
            int y = (*itr2)->getGridY();
            updateMiniMap(x, y, MINI_MAP_FIRE);
        }
    }
    
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        int x = (*itr)->getGridX();
        int y = (*itr)->getGridY();
        updateMiniMap(x, y, MINI_MAP_BOMB);
    }
    
    for (int i = 0; i < players.size(); i++)
    {
        if(players.at(i)->getPlayerState() == ALIVE)
        {
            int x = players.at(i)->getGridX();
            int y = players.at(i)->getGridY();
            updateMiniMap(x, y, MINI_MAP_PLAYER_ONE + i);
        }
    }
}

void InterfaceOverlay::handleTouchDownInputRunning(Vector2D &touchPoint, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
	if (OverlapTester::isPointInRectangle(touchPoint, m_bombButton->getBounds()))
	{
		if (player.isAbleToDropAdditionalBomb(players, bombs))
		{
			m_gameListener->addLocalEventForPlayer(PLAYER_PLANT_BOMB, player);
		}
	}
	else if (OverlapTester::isPointInRectangle(touchPoint, m_activeButton->getBounds()))
	{
		switch (player.getActivePowerUp())
		{
            case POWER_UP_TYPE_PUSH:
                for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
                {
                    if (player.isBombInFrontOfPlayer(**itr))
                    {
                        m_gameListener->addLocalEventForPlayer(PLAYER_PUSH_BOMB, player);
                    }
                }
                break;
            case POWER_UP_TYPE_SHIELD:
                m_gameListener->addLocalEventForPlayer(PLAYER_RAISE_SHIELD, player);
                break;
            default:
                break;
		}
	}
	else
	{
		int directionInput = m_dPad->getDirectionForTouchPoint(touchPoint);

		if (player.getDirection() != directionInput || !player.isMoving())
		{
			m_gameListener->addLocalEventForPlayer(directionInput + 1, player);
		}
	}
}

void InterfaceOverlay::handleTouchDraggedInputRunning(Vector2D &touchPoint, PlayerDynamicGameObject &player)
{
	if (!OverlapTester::isPointInRectangle(touchPoint, m_bombButton->getBounds()) && !OverlapTester::isPointInRectangle(touchPoint, m_activeButton->getBounds()))
	{
		int directionInput = m_dPad->getDirectionForTouchPoint(touchPoint);

		if (player.getDirection() != directionInput || !player.isMoving())
		{
			m_gameListener->addLocalEventForPlayer(directionInput + 1, player);
		}
	}
}

void InterfaceOverlay::handleTouchUpInputRunning(Vector2D &touchPoint, PlayerDynamicGameObject &player)
{
    if (OverlapTester::isPointInRectangle(touchPoint, m_bombButton->getBounds()))
    {
        // Do Nothing
    }
    else if (OverlapTester::isPointInRectangle(touchPoint, m_activeButton->getBounds()))
    {
        switch (player.getActivePowerUp())
        {
            case POWER_UP_TYPE_SHIELD:
                m_gameListener->addLocalEventForPlayer(PLAYER_LOWER_SHIELD, player);
                break;
            default:
                break;
        }
    }
	else
    {
		m_dPad->stop();
		m_gameListener->addLocalEventForPlayer(PLAYER_MOVE_STOP, player);
	}
}

bool InterfaceOverlay::handleTouchDownInputSpectating(Vector2D &touchPoint)
{
    return m_spectatorControls->didUpdateForTouchPoint(touchPoint);
}

bool InterfaceOverlay::handleTouchDraggedInputSpectating(Vector2D &touchPoint)
{
    return m_spectatorControls->didUpdateForTouchPoint(touchPoint);
}

Spectator_Control_State InterfaceOverlay::handleTouchUpInputSpectating(Vector2D &touchPoint)
{
    return m_spectatorControls->didMakeSelectionForTouchPoint(touchPoint);
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

SpectatorControls & InterfaceOverlay::getSpectatorControls()
{
    return *m_spectatorControls;
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

int InterfaceOverlay::getNumSecondsLeft()
{
    return m_iNumSecondsLeft;
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
    static Color fireColor = Color(0.85490196078431f, 0.62352941176471f, 0.08627450980392f, 1);
    static Color bombColor = Color(0.61960784313725f, 0.08627450980392f, 0.10588235294118f, 1);
    
    static Color player1Color = Color(0, 0, 0, 1);
    static Color player2Color = Color(0.4f, 0.4f, 0.8f, 1);
    static Color player3Color = Color(0, 0.60392156862745f, 0.05098039215686f, 1);
    static Color player4Color = Color(1, 0.35686274509804f, 0.08235294117647f, 1);
    static Color player5Color = Color(0.98823529411765f, 0.21176470588235f, 0.52941176470588f, 1);
    static Color player6Color = Color(1, 0, 0, 1);
    static Color player7Color = Color(1, 1, 1, 1);
    static Color player8Color = Color(0.97647058823529f, 0.74509803921569f, 0.10588235294118f, 1);
    
    static Color insideBlockColor = Color(0.4f, 0.4f, 0.4f, 1);
    static Color breakableBlockColor = Color(0.6f, 0.6f, 0.6f, 1);
    static Color mapBorderColor = Color(0.4f, 0.4f, 0.4f, 1);
    
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

std::string & InterfaceOverlay::getSpectatingUsername()
{
    return m_spectatingWho;
}

#pragma mark <Private>

void InterfaceOverlay::updateMiniMap(int x, int y, int miniMapGridType)
{
	if (x >= 0 && x < NUM_GRID_CELLS_PER_ROW && y >= 0 && y < GRID_CELL_NUM_ROWS)
	{
		m_miniMap[x][y] = miniMapGridType;
	}
}