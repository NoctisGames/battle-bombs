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
#include "PlayerDynamicGameObject.h"
#include "BotPlayerDynamicGameObject.h"
#include "MapSearchNode.h"
#include "GameListener.h"
#include "Renderer.h"
#include "Fire.h"
#include "Triangle.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "PathFinder.h"

GameScreen::GameScreen(const char *username) : GameSession()
{
    int usernameLength = (int) strlen(username);
    
    m_username = new char[usernameLength];
    
    std::strncpy(m_username, username, usernameLength);
    m_username[usernameLength] = '\0';
    
    m_bombButtonBounds = std::unique_ptr<Rectangle>(new Rectangle(21.1343282f, 2.46268653, 2.59701504f, 2.59701504f));

	m_activeButton = std::unique_ptr<ActiveButton>(new ActiveButton(19.2089551f, 0.35820895f, 2.3283582f, 2.3283582f));
    
    m_dPad = std::unique_ptr<DPadControl>(new DPadControl(2.95522392f, 2.95522392f, 5.91044784f, 5.91044784f));
    
    m_gameListener = std::unique_ptr<GameListener>(new GameListener());
    
    init();
}

GameScreen::~GameScreen()
{
    delete m_username;
    m_username = NULL;
}

void GameScreen::handleServerUpdate(const char *message)
{
    m_gameListener->addServerMessage(message);
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

void GameScreen::present()
{
    m_renderer->clearScreenWithColor(0, 0, 0, 1);
    
    switch (m_gameState)
    {
        case WAITING_FOR_SERVER:
            m_renderer->renderWorldBackground();
            
            m_renderer->renderWorldForeground(m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderMapBordersNear(m_mapBorders);
            m_renderer->renderInterface();
            
            m_renderer->endFrame();
            break;
        case RUNNING:
        case SPECTATING:
            m_renderer->calcScrollYForPlayer(*m_player);
            
            m_renderer->renderWorldBackground();
            
            m_renderer->renderWorldForeground(m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderBombs(m_bombs);
            m_renderer->renderExplosions(m_explosions);
            m_renderer->renderPlayers(m_players);
            m_renderer->renderMapBordersNear(m_mapBorders);
            m_renderer->renderInterface();
//            m_renderer->renderGameGrid(PathFinder::getInstance().game_grid);
            
            m_renderer->endFrame();
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

short GameScreen::popOldestEventId()
{
    return m_gameListener->popOldestEventId();
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
    if(m_player->isHitByExplosion(m_explosions, m_bombs))
    {
        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_DEATH);
        
        m_gameState = SPECTATING;
    }
    
    std::vector<short> localConsumedEventIds = m_gameListener->freeLocalEventIds();
    
    for (std::vector<short>::iterator itr = localConsumedEventIds.begin(); itr != localConsumedEventIds.end(); itr++)
	{
        handlePlayerEvent((*itr));
	}
    
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
                    if(m_player->isAbleToDropAdditionalBomb(m_players, m_bombs))
                    {
                        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
                    }
                }
				else if(m_activeButton->isPointInBounds(*m_touchPoint))
				{
					switch(m_player->getActivePowerUp())
					{
						case PUSH :
							for(std::vector<std::unique_ptr<BombGameObject>>::iterator itr = m_bombs.begin(); itr != m_bombs.end(); itr++)
							{
								if(m_player->isBombInFrontOfPlayer(**itr))
								{
                                    m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PUSH_BOMB);
								}
							}
                        default:
                            break;
                            // TODO!
					}
				}
                else
                {
                    updatePlayerDirection();
                }
                continue;
            case DRAGGED:
                if(!OverlapTester::isPointInRectangle(*m_touchPoint, *m_bombButtonBounds) && !m_activeButton->isPointInBounds(*m_touchPoint))
                {
                    updatePlayerDirection();
                }
                continue;
            case UP:
                if(!OverlapTester::isPointInRectangle(*m_touchPoint, *m_bombButtonBounds) && !m_activeButton->isPointInBounds(*m_touchPoint))
                {
                    if(m_player->getPlayerState() == ALIVE)
                    {
                        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_MOVE_STOP);
                    }
                }
                return;
		}
	}
}

void GameScreen::updateSpectating(float deltaTime)
{
    std::vector<short> localConsumedEventIds = m_gameListener->freeLocalEventIds();
    
    for (std::vector<short>::iterator itr = localConsumedEventIds.begin(); itr != localConsumedEventIds.end(); itr++)
	{
        handlePlayerEvent((*itr));
	}
    
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
    if(m_player->getPlayerState() == ALIVE && m_player->getPlayerActionState() != WINNING)
    {
        int directionInput = m_dPad->getDirectionForTouchPoint(*m_touchPoint);
        
        if(m_player->getDirection() != directionInput || !m_player->isMoving())
        {
            m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + directionInput + 1);
        }
    }
}

// Server Stuff

void GameScreen::processServerMessages()
{
    std::vector<const char *> serverMessages = m_gameListener->freeServerMessages();
    
	for (std::vector<const char *>::iterator itr = serverMessages.begin(); itr != serverMessages.end(); itr++)
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
            else if(eventType == BEGIN_SPECTATE && m_gameState == WAITING_FOR_SERVER)
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
    static const char *numClientBotsKey = "numClientBots";
    static const char *mapTypeKey = "mapType";
    
    if(d.HasMember(numPlayersKey))
    {
        init();
        
        int numPlayers = d[numPlayersKey].GetInt();
        for(int i = 0; i < numPlayers; i++)
        {
            m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(i, 0, 0, m_gameListener.get())));
        }
        
        if(d.HasMember(numClientBotsKey))
        {
            int numClientBots = d[numClientBotsKey].GetInt();
            for(int i = 0; i < numClientBots; i++)
            {
                m_players.push_back(std::unique_ptr<BotPlayerDynamicGameObject>(new BotPlayerDynamicGameObject(i + numPlayers, 0, 0, m_gameListener.get())));
            }
        }
        
        clientUpdate(d, isBeginGame);
        
        handleBreakableBlocksArrayInDocument(d);
        
        int mapType = d[mapTypeKey].GetInt();
        initializeInsideBlocksAndMapBordersForMapType(mapType);
        m_renderer->loadMapType(mapType);
        
        PathFinder::getInstance().resetGameGrid();
        PathFinder::getInstance().initializeGameGrid(m_insideBlocks, m_breakableBlocks, m_iMapType);
        
        Assets::getInstance()->setMusicId(MUSIC_PLAY);
        
        return true;
    }
    
    return false;
}

void GameScreen::handleBreakableBlocksArrayInDocument(rapidjson::Document &d)
{
    static const char *numBreakableBlocksKey = "numBreakableBlocks";
    
    if(d.HasMember(numBreakableBlocksKey))
    {
        m_breakableBlocks.clear();
        
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

void GameScreen::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame)
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
        handlePlayerDataUpdate(d, keyX, keyY, keyDirection, keyAlive, playerIndex);
    }
    
    handleClientEventsArrayInDocument(d);
}