//
//  WaitingForLocalSettingsInterface.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "WaitingForLocalSettingsInterface.h"
#include "GameScreen.h"
#include "TouchEvent.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameEvent.h"
#include "FlagUtil.h"
#include "ChosenPowerUpFlags.h"
#include "ChosenBotFlags.h"
#include "OverlapTester.h"
#include "StartButton.h"
#include "EnableBotButton.h"
#include "EnablePowerUpButton.h"
#include "GameConstants.h"
#include "BotDifficulty.h"

WaitingForLocalSettingsInterface::WaitingForLocalSettingsInterface(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    float interfaceLeftX = x - width / 2;
    float interfaceBottomY = y - height / 2;
    float botsXSpacing = (width - (GRID_CELL_SIZE * 7)) / 8;
    float botsLeftX = interfaceLeftX + GRID_CELL_SIZE / 2 + botsXSpacing;
    float botsY = interfaceBottomY + height * 0.9f;
    float powerUpsY = interfaceBottomY + height * 0.1f;
    float powerUpSize = width / 9;
    float powerUpsLeftX = interfaceLeftX + powerUpSize / 2;
    
    m_leftArrow = std::unique_ptr<Rectangle>(new Rectangle(interfaceLeftX, powerUpsY + GRID_CELL_SIZE, width / 2, height * 0.5f));
    m_RightArrow = std::unique_ptr<Rectangle>(new Rectangle(x, powerUpsY + GRID_CELL_SIZE, width / 2, height * 0.5f));
    m_startButton = std::unique_ptr<StartButton>(new StartButton(x, interfaceBottomY - height / 8, width * 0.82386363636364f, height * 0.2125f));
    
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(ENABLED, 1, false, botsLeftX + GRID_CELL_SIZE * 0 + botsXSpacing * 0, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(ENABLED, 2, false, botsLeftX + GRID_CELL_SIZE * 1 + botsXSpacing * 1, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(ENABLED, 3, true, botsLeftX + GRID_CELL_SIZE * 2 + botsXSpacing * 2, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, 4, false, botsLeftX + GRID_CELL_SIZE * 3 + botsXSpacing * 3, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, 5, false, botsLeftX + GRID_CELL_SIZE * 4 + botsXSpacing * 4, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, 6, false, botsLeftX + GRID_CELL_SIZE * 5 + botsXSpacing * 5, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, 7, false, botsLeftX + GRID_CELL_SIZE * 6 + botsXSpacing * 6, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_SPEED_CHOSEN, powerUpsLeftX + powerUpSize * 0, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_FORCE_FIELD_CHOSEN, powerUpsLeftX + powerUpSize * 1, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_FIRE_CHOSEN, powerUpsLeftX + powerUpSize * 2, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_BOMB_CHOSEN, powerUpsLeftX + powerUpSize * 3, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_LAND_MINE_CHOSEN, powerUpsLeftX + powerUpSize * 4, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_REMOTE_BOMB_CHOSEN, powerUpsLeftX + powerUpSize * 5, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_CURSE_CHOSEN, powerUpsLeftX + powerUpSize * 6, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_SHIELD_CHOSEN, powerUpsLeftX + powerUpSize * 7, powerUpsY, powerUpSize, powerUpSize)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(PU_PUSH_CHOSEN, powerUpsLeftX + powerUpSize * 8, powerUpsY, powerUpSize, powerUpSize)));
    
    m_iChosenMapType = MAP_SPACE;
    m_iChosenBotFlags = 0;
    m_iChosenPowerUpFlags = 0;
    
    m_isStartingGame = false;
}

void WaitingForLocalSettingsInterface::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
    {
        gameScreen->touchToWorld((*itr));
        
        switch (itr->getTouchType())
        {
            case DOWN:
                m_iChosenBotFlags = 0;
                for (std::vector<std::unique_ptr<EnableBotButton>>::iterator itr = m_enableBotButtons.begin(); itr != m_enableBotButtons.end(); itr++)
                {
                    if(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), (*itr)->getBounds()))
                    {
                        (*itr)->toggle();
                    }
                    
                    if((*itr)->getButtonState() == ENABLED)
                    {
                        m_iChosenBotFlags = FlagUtil::setFlag(m_iChosenBotFlags, (*itr)->getChosenBotFlag());
                    }
                }
                
                for (std::vector<std::unique_ptr<EnablePowerUpButton>>::iterator itr = m_enablePowerUpButtons.begin(); itr != m_enablePowerUpButtons.end(); itr++)
                {
                    if(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), (*itr)->getBounds()))
                    {
                        (*itr)->toggle();
                    }
                }
                
                m_startButton->setButtonState(m_iChosenBotFlags > 0 ? OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()) ? HIGHLIGHTED : ENABLED : DISABLED);
                continue;
            case DRAGGED:
                m_startButton->setButtonState(m_iChosenBotFlags > 0 ? OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()) ? HIGHLIGHTED : ENABLED : DISABLED);
                continue;
            case UP:
                m_startButton->setButtonState(m_iChosenBotFlags > 0 ? OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()) ? PRESSED : ENABLED : DISABLED);
                
                if(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), *m_leftArrow))
                {
                    m_iChosenMapType--;
                    if(m_iChosenMapType < MAP_SPACE)
                    {
                        m_iChosenMapType = NUM_MAPS - 1;
                    }
                }
                else if(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), *m_RightArrow))
                {
                    m_iChosenMapType++;
                    if(m_iChosenMapType >= NUM_MAPS)
                    {
                        m_iChosenMapType = MAP_SPACE;
                    }
                }
                else if(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()))
                {
                    startGame();
                }
                return;
        }
    }
}

void WaitingForLocalSettingsInterface::reset()
{
    m_isStartingGame = false;
    
    m_startButton->setButtonState(ENABLED);
}

StartButton & WaitingForLocalSettingsInterface::getStartButton()
{
    return *m_startButton;
}

std::vector<std::unique_ptr<EnableBotButton>> & WaitingForLocalSettingsInterface::getEnableBotButtons()
{
    return m_enableBotButtons;
}

std::vector<std::unique_ptr<EnablePowerUpButton>> & WaitingForLocalSettingsInterface::getEnablePowerUpButtons()
{
    return m_enablePowerUpButtons;
}

int WaitingForLocalSettingsInterface::getChosenMapType()
{
    return m_iChosenMapType;
}

int WaitingForLocalSettingsInterface::getChosenBotFlags()
{
    return m_iChosenBotFlags;
}

int WaitingForLocalSettingsInterface::getChosenPowerUpFlags()
{
    return m_iChosenPowerUpFlags;
}

bool WaitingForLocalSettingsInterface::isStartingGame()
{
    return m_isStartingGame;
}

#pragma mark <Private>

void WaitingForLocalSettingsInterface::startGame()
{
    m_iChosenBotFlags = 0;
    for (std::vector<std::unique_ptr<EnableBotButton>>::iterator itr = m_enableBotButtons.begin(); itr != m_enableBotButtons.end(); itr++)
    {
        if((*itr)->getButtonState() == ENABLED)
        {
            m_iChosenBotFlags = FlagUtil::setFlag(m_iChosenBotFlags, (*itr)->getChosenBotFlag());
        }
    }
    
    m_iChosenPowerUpFlags = 0;
    for (std::vector<std::unique_ptr<EnablePowerUpButton>>::iterator itr = m_enablePowerUpButtons.begin(); itr != m_enablePowerUpButtons.end(); itr++)
    {
        if((*itr)->getButtonState() == ENABLED)
        {
            m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, (*itr)->getChosenPowerUpFlag());
        }
    }
    
    if(m_iChosenBotFlags > 0)
    {
        m_isStartingGame = true;
    }
}