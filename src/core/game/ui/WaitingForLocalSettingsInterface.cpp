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

WaitingForLocalSettingsInterface::WaitingForLocalSettingsInterface(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    float interfaceLeftX = x - width / 2;
    float interfaceBottomY = y - height / 2;
    float botsLeftX = interfaceLeftX + GRID_CELL_SIZE * 3 / 2;
    float botsY = interfaceBottomY + height * 0.9f;
    float powerUpsY = interfaceBottomY + height * 0.1f;
    
    m_leftArrow = std::unique_ptr<Rectangle>(new Rectangle(interfaceLeftX, interfaceBottomY, width / 2, height));
    m_RightArrow = std::unique_ptr<Rectangle>(new Rectangle(x, interfaceBottomY, width / 2, height));
    m_startButton = std::unique_ptr<StartButton>(new StartButton(x, interfaceBottomY - height / 8, width * 0.82386363636364f, height * 0.2125f));
    
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(ENABLED, false, botsLeftX + GRID_CELL_SIZE * 0, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(ENABLED, false, botsLeftX + GRID_CELL_SIZE * 1, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(ENABLED, true, botsLeftX + GRID_CELL_SIZE * 2, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, false, botsLeftX + GRID_CELL_SIZE * 3, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, false, botsLeftX + GRID_CELL_SIZE * 4, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, false, botsLeftX + GRID_CELL_SIZE * 5, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enableBotButtons.push_back(std::unique_ptr<EnableBotButton>(new EnableBotButton(DISABLED, false, botsLeftX + GRID_CELL_SIZE * 6, botsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_SPEED, interfaceLeftX + GRID_CELL_SIZE * 0, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_FORCE_FIELD, interfaceLeftX + GRID_CELL_SIZE * 1, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_FIRE, interfaceLeftX + GRID_CELL_SIZE * 2, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_BOMB, interfaceLeftX + GRID_CELL_SIZE * 3, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_LAND_MINE, interfaceLeftX + GRID_CELL_SIZE * 4, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_REMOTE_BOMB, interfaceLeftX + GRID_CELL_SIZE * 5, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_CURSE, interfaceLeftX + GRID_CELL_SIZE * 6, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_SHIELD, interfaceLeftX + GRID_CELL_SIZE * 7, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    m_enablePowerUpButtons.push_back(std::unique_ptr<EnablePowerUpButton>(new EnablePowerUpButton(POWER_UP_TYPE_PUSH, interfaceLeftX + GRID_CELL_SIZE * 8, powerUpsY, GRID_CELL_SIZE, GRID_CELL_SIZE)));
    
    m_iChosenMapType = MAP_SPACE;
    
    m_iChosenBotFlags = 0;
    m_iChosenBotFlags = FlagUtil::setFlag(m_iChosenBotFlags, BOT_2_NORMAL);
    m_iChosenBotFlags = FlagUtil::setFlag(m_iChosenBotFlags, BOT_3_NORMAL);
    m_iChosenBotFlags = FlagUtil::setFlag(m_iChosenBotFlags, BOT_4_NORMAL);
    
    m_iChosenPowerUpFlags = 0;
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_SPEED_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_FORCE_FIELD_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_FIRE_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_BOMB_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_LAND_MINE_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_REMOTE_BOMB_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_CURSE_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_SHIELD_CHOSEN);
    m_iChosenPowerUpFlags = FlagUtil::setFlag(m_iChosenPowerUpFlags, PU_PUSH_CHOSEN);
    
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
                m_startButton->setButtonState(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()) ? HIGHLIGHTED : ENABLED);
                
                continue;
            case DRAGGED:
                m_startButton->setButtonState(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()) ? HIGHLIGHTED : ENABLED);
                continue;
            case UP:
                m_startButton->setButtonState(OverlapTester::isPointInRectangle(*gameScreen->getTouchPoint(), m_startButton->getBounds()) ? PRESSED : ENABLED);
                
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
                    m_isStartingGame = true;
                }
                return;
        }
    }
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