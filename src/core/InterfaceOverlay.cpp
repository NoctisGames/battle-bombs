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
#include "GameListener.h"
#include "GameEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "PowerUpBarItem.h"
#include "PlayerAvatar.h"

InterfaceOverlay::InterfaceOverlay(GameListener *gameListener)
{
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  8.67761194125f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  7.99701492625f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  7.31641791125f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(0.5373134328357f,  6.63582089625f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 8.67761194125f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 7.99701492625f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 7.31641791125f, 1.43283582089544f, 1.36119403f)));
    m_playerAvatars.push_back(std::unique_ptr<PlayerAvatar>(new PlayerAvatar(1.25373134328342f, 6.63582089625f, 1.43283582089544f, 1.36119403f)));
    
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

void InterfaceOverlay::update(float deltaTime, PlayerDynamicGameObject &player, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
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
