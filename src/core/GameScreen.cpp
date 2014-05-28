//
//  GameScreen.cpp
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "GameScreen.h"
#include "Vector2D.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Assets.h"
#include "OverlapTester.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "SoundListener.h"

GameScreen::GameScreen(const char *username) : GameSession()
{
    int usernameLength = strlen(username);
    
    m_username = new char[usernameLength];
    
    std::strncpy(m_username, username, usernameLength);
    m_username[usernameLength] = '\0';
    
    m_bombButtonBounds = std::unique_ptr<Rectangle>(new Rectangle(20.536f, 0, 3.456f, 2.916f));

	m_activeButton = std::unique_ptr<ActiveButton>(new ActiveButton(18.5f, 1.2f , 2.5f, 2.5f));
    
    m_dPad = std::unique_ptr<DPadControl>(new DPadControl(2.15f, 2.15f, 4.3f, 4.3f));
    
    m_soundListener = std::unique_ptr<SoundListener>(new SoundListener());
    
    init();
}

GameScreen::~GameScreen()
{
    delete m_username;
    m_username = NULL;
}

void GameScreen::handleServerUpdate(const char *message)
{
	char *copy = strdup(message);
	m_serverMessagesBuffer.push_back(copy);
}

void GameScreen::init()
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    
    m_player = nullptr;
    m_sPlayerIndex = -1;
    
    m_breakableBlocks.clear();
    m_players.clear();
    m_bombs.clear();
    m_explosions.clear();
    m_powerUps.clear();
    
    m_gameState = WAITING_FOR_SERVER;
    m_iScreenState = 0;
    m_fTimeSinceLastClientEvent = 0;
}

void GameScreen::onResume()
{
	if(m_gameState == RUNNING)
	{
        platformResume();
	}
}

void GameScreen::onPause()
{
    Assets::getInstance()->setMusicId(MUSIC_STOP);
    
    platformPause();
}

void GameScreen::update(float deltaTime, std::vector<TouchEvent> &touchEvents)
{
    m_fTimeSinceLastClientEvent += deltaTime;
    
    processServerMessages();
    
    switch (m_gameState)
    {
        case RUNNING:
            updateInputRunning(touchEvents);
            updateRunning(deltaTime);
            break;
        case SPECTATING:
            updateInputSpectating(touchEvents);
            updateSpectating(deltaTime);
            break;
        default:
            break;
    }
}

int GameScreen::getState()
{
	return m_iScreenState;
}

void GameScreen::clearState()
{
    m_iScreenState = 0;
}

short GameScreen::getPlayerIndex()
{
    return m_sPlayerIndex;
}

float GameScreen::getPlayerX()
{
    return getPlayerXAtIndex(m_sPlayerIndex);
}

float GameScreen::getPlayerY()
{
    return getPlayerYAtIndex(m_sPlayerIndex);
}

int GameScreen::getPlayerDirection()
{
    return getPlayerDirectionAtIndex(m_sPlayerIndex);
}

short GameScreen::getFirstEventId()
{
    if(m_sLocalConsumedEventIds.size() > 0)
    {
		return m_sLocalConsumedEventIds.front();
    }
    
    return 0;
}

void GameScreen::eraseFirstEventId()
{
    if(m_sLocalConsumedEventIds.size() > 0)
    {
        m_sLocalConsumedEventIds.erase(m_sLocalConsumedEventIds.begin());
    }
}

bool GameScreen::isTimeToSendKeepAlive()
{
    return m_fTimeSinceLastClientEvent > 3;
}

void GameScreen::resetTimeSinceLastClientEvent()
{
	m_fTimeSinceLastClientEvent = 0;
}

// Private Methods

void GameScreen::updateRunning(float deltaTime)
{
    if(m_player->isHitByExplosion(m_explosions))
    {
        addEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_DEATH);
    }
    
    for (std::vector<short>::iterator itr = m_sLocalEventIds.begin(); itr != m_sLocalEventIds.end(); itr++)
	{
        handlePlayerEvent((*itr));
        
        m_sLocalConsumedEventIds.push_back(*itr);
	}
	
    m_sLocalEventIds.clear();
    
    for (std::vector<short>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        short eventId = (*itr);
        short derivedPlayerIndexFromEvent = 0;
        
        while(eventId > PLAYER_EVENT_BASE)
        {
            eventId -= PLAYER_EVENT_BASE;
            derivedPlayerIndexFromEvent++;
        }
        
        if(derivedPlayerIndexFromEvent != m_sPlayerIndex)
        {
            // Only process Server Events that THIS client did not generate itself
            handlePlayerEvent((*itr));
        }
    }
    
    m_sEventIds.clear();
    
    updateCommon(deltaTime);
}

void GameScreen::updateInputRunning(std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
	{
		touchToWorld((*itr));
		
		switch (itr->getTouchType())
		{
            case DOWN:
                if(OverlapTester::isPointInRectangle(*m_touchPoint, *m_bombButtonBounds))
                {
                    if(m_player->isAbleToDropAdditionalBomb())
                    {
                        addEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
                    }
                }
				/*else if(m_activeButton->isPointInBounds(*m_touchPoint))
				{

				}*/
                else
                {
                    updatePlayerDirection();
                }
                continue;
            case DRAGGED:
                if(!OverlapTester::isPointInRectangle(*m_touchPoint, *m_bombButtonBounds))
                {
                    updatePlayerDirection();
                }
                continue;
            case UP:
                if(!OverlapTester::isPointInRectangle(*m_touchPoint, *m_bombButtonBounds))
                {
                    if(m_player->getPlayerState() == ALIVE)
                    {
                        addEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_MOVE_STOP);
                    }
                }
                return;
		}
	}
}

void GameScreen::updateSpectating(float deltaTime)
{
    for (std::vector<short>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }
    
    m_sEventIds.clear();
    
    updateCommon(deltaTime);
}

void GameScreen::updateInputSpectating(std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
	{
		touchToWorld((*itr));
		
		switch (itr->getTouchType())
		{
            case DOWN:
                continue;
            case DRAGGED:
                continue;
            case UP:
                spectateNextLivePlayer();
                return;
		}
	}
}

void GameScreen::updateCommon(float deltaTime)
{
    for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = m_bombs.begin(); itr != m_bombs.end(); )
    {
		(**itr).update(deltaTime, m_explosions, m_insideBlocks, m_breakableBlocks);
        
        if((**itr).isExploding())
        {
            itr = m_bombs.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector<std::unique_ptr<BreakableBlock>>::iterator itr = m_breakableBlocks.begin(); itr != m_breakableBlocks.end(); )
    {
        if((**itr).isDestroyed())
        {
			if((**itr).hasPowerUp())
			{
				m_powerUps.push_back(std::unique_ptr<PowerUp>(new PowerUp((**itr).getX(), (**itr).getY(), (**itr).getPowerUpFlag())));
			}
            itr = m_breakableBlocks.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector<std::unique_ptr<Explosion>>::iterator itr = m_explosions.begin(); itr != m_explosions.end(); )
    {
		(**itr).update(deltaTime);
        
        if((**itr).isComplete())
        {
            itr = m_explosions.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
		(**itr).update(deltaTime, m_insideBlocks, m_breakableBlocks, m_powerUps);
    }
    
	for (std::vector<std::unique_ptr<PowerUp>>::iterator itr = m_powerUps.begin(); itr != m_powerUps.end(); )
	{
		if((**itr).isPickedUp())
		{
			itr = m_powerUps.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void GameScreen::spectateNextLivePlayer()
{
    short playerIndex = m_sPlayerIndex >= 0 ? m_sPlayerIndex : 0;
    playerIndex++;
    if (playerIndex < m_players.size())
    {
        while (playerIndex < m_players.size() && m_players.at(playerIndex)->getPlayerState() != ALIVE)
        {
            playerIndex++;
        }
    }
    
    m_sPlayerIndex = playerIndex >= m_players.size() || playerIndex < 0 ? 0 : playerIndex;
    
    m_player = m_players.at(m_sPlayerIndex).get();
}

void GameScreen::updatePlayerDirection()
{
    if(m_player->getPlayerState() == ALIVE)
    {
        int directionInput = m_dPad->getDirectionForTouchPoint(*m_touchPoint);
        
        if(m_player->getDirection() != directionInput || !m_player->isMoving())
        {
            addEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + directionInput + 1);
        }
    }
}

void GameScreen::addEvent(short eventId)
{
    m_sLocalEventIds.push_back(eventId);
}

// Server Stuff

void GameScreen::processServerMessages()
{
    m_serverMessages.clear();
	m_serverMessages.swap(m_serverMessagesBuffer);
	m_serverMessagesBuffer.clear();
    
	for (std::vector<const char *>::iterator itr = m_serverMessages.begin(); itr != m_serverMessages.end(); itr++)
	{
		static const char *eventTypeKey = "eventType";
        
		rapidjson::Document d;
		d.Parse<0>(*itr);
        if(d.HasMember(eventTypeKey))
        {
            int eventType = d[eventTypeKey].GetInt();
            
            if (eventType == BEGIN_GAME)
            {
                beginGame(d);
            }
            else if (eventType == CLIENT_UPDATE && (m_gameState == RUNNING || m_gameState == SPECTATING))
            {
                clientUpdate(d, false);
            }
            else if(m_gameState == WAITING_FOR_SERVER && eventType == BEGIN_SPECTATE)
            {
                beginSpectate(d);
            }
        }
	}
}

void GameScreen::beginGame(rapidjson::Document &d)
{
    if(beginCommon(d, true))
    {
        if(m_sPlayerIndex != -1)
        {
            m_player = m_players.at(m_sPlayerIndex).get();
            
            m_gameState = RUNNING;
        }
    }
}

void GameScreen::beginSpectate(rapidjson::Document &d)
{
    if(beginCommon(d, false))
    {
        m_sPlayerIndex = 0;
        m_player = m_players.at(m_sPlayerIndex).get();
        
        m_gameState = SPECTATING;
    }
}

bool GameScreen::beginCommon(rapidjson::Document &d, bool isBeginGame)
{
    static const char *numPlayersKey = "numPlayers";
    
    const bool hasNumPlayersKey = d.HasMember(numPlayersKey);
    
    if(hasNumPlayersKey)
    {
        init();
        
        int numPlayers = d[numPlayersKey].GetInt();
        for(int i = 0; i < numPlayers; i++)
        {
            m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(0, 0, m_soundListener.get())));
        }
        
        clientUpdate(d, isBeginGame);
        
        handleBreakableBlocksArrayInDocument(d);
        
        Assets::getInstance()->setMusicId(MUSIC_PLAY);
    }
    
    return hasNumPlayersKey;
}

void GameScreen::handleBreakableBlocksArrayInDocument(rapidjson::Document &d)
{
    static const char *numBreakableBlocksKey = "numBreakableBlocks";
    
    if(d.HasMember(numBreakableBlocksKey))
    {
        static const char *breakableBlockXValuesKey = "breakableBlockXValues";
        static const char *breakableBlockYValuesKey = "breakableBlockYValues";
        static const char *breakableBlockPowerUpFlagsKey = "breakableBlockPowerUpFlags";
        
        std::vector<short> breakableBlockXValues;
        std::vector<short> breakableBlockYValues;
        std::vector<short> breakableBlockPowerUpFlags;
        
        handleShortArrayInDocument(d, breakableBlockXValuesKey, breakableBlockXValues, -1);
        handleShortArrayInDocument(d, breakableBlockYValuesKey, breakableBlockYValues, -1);
        handleShortArrayInDocument(d, breakableBlockPowerUpFlagsKey, breakableBlockPowerUpFlags, -1);
        
        int numBreakableBlocks = d[numBreakableBlocksKey].GetInt();
        for(int i = 0; i < numBreakableBlocks; i++)
        {
            m_breakableBlocks.push_back(std::unique_ptr<BreakableBlock>(new BreakableBlock(breakableBlockXValues.at(i), breakableBlockYValues.at(i), breakableBlockPowerUpFlags.at(i))));
        }
    }
}

void GameScreen::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, short playerIndex, bool isBeginGame)
{
    if(isBeginGame && d.HasMember(keyIndex))
    {
        const char *username = d[keyIndex].GetString();
        if (std::strcmp(username, m_username) == 0)
        {
            // Now we know which player index the user is
            m_sPlayerIndex = playerIndex;
        }
    }
    
    if(isBeginGame || m_gameState == SPECTATING || playerIndex != m_sPlayerIndex)
    {
        handlePositionAndDirectionUpdate(d, keyX, keyY, keyDirection, playerIndex);
    }
    
    handleClientEventsArrayInDocument(d);
}