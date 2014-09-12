//
//  GameScreen.cpp
//  battlebombs
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
#include "WaitingForServerInterface.h"
#include "WaitingForLocalSettingsInterface.h"
#include "InterfaceOverlay.h"
#include "BombButton.h"
#include "PowerUpBarItem.h"
#include "PlayerAvatar.h"
#include "Font.h"
#include "SpectatorControls.h"
#include "CountDownNumberGameObject.h"
#include "DisplayBattleGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"

GameScreen::GameScreen(const char *username, bool isOffline) : GameSession()
{
    int usernameLength = (int) strlen(username);
    
    m_username = std::unique_ptr<char>(new char[usernameLength + 1]);
    
    std::strncpy(m_username.get(), username, usernameLength);
    m_username.get()[usernameLength] = '\0';
    
    m_isOffline = isOffline;
    
    init();
    
    m_gameState = m_isOffline ? WAITING : WAITING_FOR_CONNECTION;
}

void GameScreen::handleServerUpdate(const char *message)
{
    m_gameListener->addServerMessage(message);
}

void GameScreen::init()
{
	m_touchPoint.release();
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
	m_displayBattle.release();
	m_displayBattle = std::unique_ptr<DisplayBattleGameObject>(new DisplayBattleGameObject(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, GRID_CELL_WIDTH * 14, GRID_CELL_HEIGHT * 1.75f));
	m_gameListener.release();
	m_gameListener = std::unique_ptr<GameListener>(new GameListener());
	m_waitingForServerInterface.release();
	m_waitingForServerInterface = std::unique_ptr<WaitingForServerInterface>(new WaitingForServerInterface(SCREEN_WIDTH / 2, 5.81492537375f, 10.38805970149248f, 11.2298507475f, m_username.get()));
	m_waitingForLocalSettingsInterface.release();
	m_waitingForLocalSettingsInterface = std::unique_ptr<WaitingForLocalSettingsInterface>(new WaitingForLocalSettingsInterface());
	m_interfaceOverlay.release();
	m_interfaceOverlay = std::unique_ptr<InterfaceOverlay>(new InterfaceOverlay(m_gameListener.get()));
    
    m_player = nullptr;
    m_sPlayerIndex = -1;
    
    m_breakableBlocks.clear();
    m_players.clear();
    m_bombs.clear();
    m_explosions.clear();
    m_powerUps.clear();
    m_countDownNumbers.clear();
    m_displayGameOvers.clear();
    
    m_gameState = m_isOffline ? WAITING_FOR_LOCAL_SETTINGS : WAITING_FOR_SERVER;
    m_iScreenState = 0;
    m_fTimeSinceLastClientEvent = 0;
    m_fCountDownTimeLeft = 3;
    m_fTimeSinceGameOver = 0;
    m_fBlackCoverTransitionAlpha = 0;
    
    m_hasDisplayed2 = false;
    m_hasDisplayed1 = false;
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
    
    m_renderer->cleanUp();
    
    platformPause();
}

void GameScreen::update(float deltaTime, std::vector<TouchEvent> &touchEvents)
{
    if(m_gameState == WAITING_FOR_SERVER || m_gameState == COUNTING_DOWN || m_gameState == RUNNING || m_gameState == SPECTATING || m_gameState == GAME_ENDING)
    {
        m_fTimeSinceLastClientEvent += deltaTime;
    }
    
    processServerMessages();
    
    switch (m_gameState)
    {
        case WAITING_FOR_CONNECTION:
            // See processServerMessages()
            break;
        case CONNECTION_ERROR_WAITING_FOR_INPUT:
            updateInputConnectionErrorWaitingForInput(touchEvents);
            break;
        case WAITING_FOR_SERVER:
            m_waitingForServerInterface->update(deltaTime, m_gameState);
            break;
        case WAITING_FOR_LOCAL_SETTINGS:
            // TODO, allow the user to pick a map and set the number of bots
            updateInputWaitingForLocalSettings(touchEvents);
            m_waitingForLocalSettingsInterface->update(deltaTime);
            break;
        case COUNTING_DOWN:
            m_fCountDownTimeLeft -= deltaTime;
            if(m_fCountDownTimeLeft < 0)
            {
                m_countDownNumbers.clear();
                
                m_gameListener->playSound(SOUND_BATTLE);
                m_gameState = RUNNING;
                
                for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
                {
                    (*itr)->setIsDisplayingName(false);
                }
                
                Assets::getInstance()->setMusicId(m_iMapType + 2);
            }
            else if(m_fCountDownTimeLeft <= 1 && !m_hasDisplayed1)
            {
                m_gameListener->playSound(SOUND_COUNT_DOWN_1);
                m_countDownNumbers.push_back(std::unique_ptr<CountDownNumberGameObject>(new CountDownNumberGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2, SCREEN_WIDTH * 2 * 0.76923076923077f, DISPLAY_1)));
                m_hasDisplayed1 = true;
            }
            else if(m_fCountDownTimeLeft <= 2 && !m_hasDisplayed2)
            {
                m_gameListener->playSound(SOUND_COUNT_DOWN_2);
                m_countDownNumbers.push_back(std::unique_ptr<CountDownNumberGameObject>(new CountDownNumberGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2, SCREEN_WIDTH * 2 * 0.76923076923077f, DISPLAY_2)));
                m_hasDisplayed2 = true;
            }
            
            for (std::vector < std::unique_ptr < CountDownNumberGameObject >> ::iterator itr = m_countDownNumbers.begin(); itr != m_countDownNumbers.end(); )
            {
                (*itr)->update(deltaTime);
                
                if ((*itr)->remove())
                {
                    itr = m_countDownNumbers.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
            
            break;
        case RUNNING:
            if(m_player->getPlayerState() == ALIVE && m_player->getPlayerActionState() != WINNING)
            {
                updateInputRunning(touchEvents);
            }
            updateRunning(deltaTime);
            break;
        case SPECTATING:
            updateInputSpectating(touchEvents);
            updateSpectating(deltaTime);
            break;
        case GAME_ENDING:
            updateGameEnding(deltaTime);
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
        case WAITING_FOR_CONNECTION:
        case CONNECTION_ERROR_WAITING_FOR_INPUT:
        case WAITING_FOR_SERVER:
            m_renderer->beginFrame();
            m_renderer->renderWaitingForServerInterface(*m_waitingForServerInterface);
            m_renderer->endFrame();
            break;
        case WAITING_FOR_LOCAL_SETTINGS:
            // TODO, Render interface for picking a map and setting # of bots
            m_renderer->beginFrame();
            m_renderer->renderWaitingForLocalSettingsInterface(*m_waitingForLocalSettingsInterface);
            m_renderer->endFrame();
            break;
        case COUNTING_DOWN:
            m_renderer->calcScrollYForPlayer(*m_player);
            
            m_renderer->beginFrame();
            m_renderer->renderWorldBackground();
            
            m_renderer->renderWorldForeground(m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderBombs(m_bombs);
            m_renderer->renderExplosions(m_explosions);
            m_renderer->renderPlayers(m_players);
            m_renderer->renderMapBordersNear(m_mapBorders);
            
            m_renderer->renderUIEffects(m_countDownNumbers, *m_displayBattle, m_displayGameOvers);
            
            m_renderer->endFrame();
            break;
        case RUNNING:
            m_renderer->calcScrollYForPlayer(*m_player);
            
            m_renderer->beginFrame();
            m_renderer->renderWorldBackground();
            
            m_renderer->renderWorldForeground(m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderBombs(m_bombs);
            m_renderer->renderExplosions(m_explosions);
            m_renderer->renderPlayers(m_players);
            m_renderer->renderMapBordersNear(m_mapBorders);
            m_renderer->renderInterface(*m_interfaceOverlay);
            
            m_renderer->renderUIEffects(m_countDownNumbers, *m_displayBattle, m_displayGameOvers);
            
            m_renderer->endFrame();
            break;
        case SPECTATING:
            m_renderer->calcScrollYForPlayer(*m_player);
            
            m_renderer->beginFrame();
            m_renderer->renderWorldBackground();
            
            m_renderer->renderWorldForeground(m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderBombs(m_bombs);
            m_renderer->renderExplosions(m_explosions);
            m_renderer->renderPlayers(m_players);
            m_renderer->renderMapBordersNear(m_mapBorders);
            m_renderer->renderSpectatorInterface(*m_interfaceOverlay);
            
            m_renderer->renderUIEffects(m_countDownNumbers, *m_displayBattle, m_displayGameOvers);
            
            m_renderer->endFrame();
            break;
        case GAME_ENDING:
            m_renderer->calcScrollYForPlayer(*m_player);
            
            m_renderer->beginFrame();
            m_renderer->renderWorldBackground();
            
            m_renderer->renderWorldForeground(m_mapBorders, m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderBombs(m_bombs);
            m_renderer->renderExplosions(m_explosions);
            m_renderer->renderPlayers(m_players);
            m_renderer->renderMapBordersNear(m_mapBorders);
            
            m_renderer->renderUIEffects(m_countDownNumbers, *m_displayBattle, m_displayGameOvers);
            
            m_renderer->renderGameOverBlackCover(m_fBlackCoverTransitionAlpha);
            
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

int GameScreen::popOldestEventId()
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

int GameScreen::getNumSecondsLeft()
{
    return m_interfaceOverlay == nullptr ? 0 : m_interfaceOverlay->getNumSecondsLeft();
}

#pragma mark <Private>

void GameScreen::updateInputWaitingForLocalSettings(std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
	{
        m_iScreenState = 1;
	}
}

void GameScreen::updateInputConnectionErrorWaitingForInput(std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
	{
        m_iScreenState = 1;
	}
}

void GameScreen::updateRunning(float deltaTime)
{
    m_player->handlePowerUps(m_powerUps);
    
    if(m_player->isHitByExplosion(m_explosions, m_bombs))
    {
        m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, *m_player);
        
        m_gameState = SPECTATING;
    }
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        if ((*itr)->isBot())
        {
            (*itr)->handlePowerUps(m_powerUps);
            
            if ((*itr)->isHitByExplosion(m_explosions, m_bombs))
            {
                m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, (**itr));
            }
        }
    }
    
    std::vector<int> localConsumedEventIds = m_gameListener->freeLocalEventIds();
    
    for (std::vector<int>::iterator itr = localConsumedEventIds.begin(); itr != localConsumedEventIds.end(); itr++)
	{
        handlePlayerEvent((*itr));
	}
    
    for (std::vector<int>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        int eventId = (*itr);
        int derivedPlayerIndexFromEvent = 0;
        
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
    
    m_interfaceOverlay->update(deltaTime, *m_player, m_players, m_bombs, m_explosions, m_insideBlocks, m_breakableBlocks, m_iMapType, m_sPlayerIndex, m_gameState);
    
    m_displayBattle->update(deltaTime);
    
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
                m_interfaceOverlay->handleTouchDownInputRunning(*m_touchPoint, *m_player, m_players, m_bombs);
                continue;
            case DRAGGED:
                m_interfaceOverlay->handleTouchDraggedInputRunning(*m_touchPoint, *m_player);
                continue;
            case UP:
                m_interfaceOverlay->handleTouchUpInputRunning(*m_touchPoint, *m_player);
                return;
		}
	}
}

void GameScreen::updateSpectating(float deltaTime)
{
	for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
	{
		if ((*itr)->isBot())
		{
			(*itr)->handlePowerUps(m_powerUps);

			if ((*itr)->isHitByExplosion(m_explosions, m_bombs))
			{
				m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, (**itr));
			}
		}
	}

    std::vector<int> localConsumedEventIds = m_gameListener->freeLocalEventIds();
    
    for (std::vector<int>::iterator itr = localConsumedEventIds.begin(); itr != localConsumedEventIds.end(); itr++)
	{
        handlePlayerEvent((*itr));
	}
    
    for (std::vector<int>::iterator itr = m_sEventIds.begin(); itr != m_sEventIds.end(); itr++)
    {
        handlePlayerEvent((*itr));
    }
    
    m_sEventIds.clear();
    
    m_interfaceOverlay->update(deltaTime, *m_player, m_players, m_bombs, m_explosions, m_insideBlocks, m_breakableBlocks, m_iMapType, m_sPlayerIndex, m_gameState);
    
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
                if(m_interfaceOverlay->handleTouchDownInputSpectating(*m_touchPoint))
                {
                    continue;
                }
                
                // Basically, if the user isn't touching the arrows,
                // assume they want to pan the game world using their finger
                continue;
            case DRAGGED:
                if(m_interfaceOverlay->handleTouchDraggedInputSpectating(*m_touchPoint))
                {
                    continue;
                }
                
                // Basically, if the user isn't touching the arrows,
                // assume they want to pan the game world using their finger
                continue;
            case UP:
                Spectator_Control_State spectatorControlState = m_interfaceOverlay->handleTouchUpInputSpectating(*m_touchPoint);
                if(spectatorControlState == LEFT_ARROW_HIGHLIGHTED)
                {
                    spectatePreviousLivePlayer();
                }
                else if(spectatorControlState == RIGHT_ARROW_HIGHLIGHTED)
                {
                    spectateNextLivePlayer();
                }
                return;
		}
	}
}

void GameScreen::updateGameEnding(float deltaTime)
{
    m_fTimeSinceGameOver += deltaTime;
    
    for (std::vector < std::unique_ptr < DisplayGameOverGameObject >> ::iterator itr = m_displayGameOvers.begin(); itr != m_displayGameOvers.end(); itr++)
    {
        (*itr)->update(deltaTime);
    }
    
    if(m_fTimeSinceGameOver > 6)
    {
        m_fBlackCoverTransitionAlpha += deltaTime * 0.40f;
        if(m_fBlackCoverTransitionAlpha > 1)
        {
            Assets::getInstance()->setMusicId(MUSIC_STOP);
            
            init();
        }
    }
    else
    {
        updateSpectating(deltaTime / (m_fTimeSinceGameOver + 1));
    }
}

void GameScreen::spectateNextLivePlayer()
{
    short playerIndex = m_sPlayerIndex >= 0 ? m_sPlayerIndex : 0;
    for(int i = 0; i < m_players.size(); i++)
    {
        playerIndex++;
        if(playerIndex == m_players.size())
        {
            playerIndex = 0;
        }
        
        if(m_players.at(playerIndex)->getPlayerState() == ALIVE)
        {
            break;
        }
    }
    
    m_sPlayerIndex = playerIndex;
    
    m_player = m_players.at(m_sPlayerIndex).get();
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        (*itr)->setIsDisplayingPointer(false);
    }
    m_player->setIsDisplayingPointer(true);
}

void GameScreen::spectatePreviousLivePlayer()
{
    short playerIndex = m_sPlayerIndex >= 0 ? m_sPlayerIndex : 0;
    for(int i = 0; i < m_players.size(); i++)
    {
        playerIndex--;
        if(playerIndex <= 0)
        {
            playerIndex = m_players.size() - 1;
        }
        
        if(m_players.at(playerIndex)->getPlayerState() == ALIVE)
        {
            break;
        }
    }
    
    m_sPlayerIndex = playerIndex;
    
    m_player = m_players.at(m_sPlayerIndex).get();
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        (*itr)->setIsDisplayingPointer(false);
    }
    m_player->setIsDisplayingPointer(true);
}

// Server Stuff

void GameScreen::processServerMessages()
{
    std::vector<const char *> serverMessages = m_gameListener->freeServerMessages();
    
	for (std::vector<const char *>::iterator itr = serverMessages.begin(); itr != serverMessages.end(); itr++)
	{
        rapidjson::Document d;
		d.Parse<0>(*itr);
        if(d.IsObject())
        {
            static const char *eventTypeKey = "eventType";
            
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
                else if(eventType == GAME_OVER && (m_gameState == RUNNING || m_gameState == SPECTATING))
                {
                    gameOver(d);
                }
                else if(eventType == PRE_GAME_SERVER_UPDATE && m_gameState == WAITING_FOR_SERVER)
                {
                    m_waitingForServerInterface->handlePreGameServerUpdate(d);
                }
                else if(eventType == PRE_GAME && m_gameState == WAITING_FOR_CONNECTION)
                {
                    static const char *phaseKey = "phase";
                    
                    if(d.HasMember(phaseKey))
                    {
                        int phase = d[phaseKey].GetInt();
                        m_waitingForServerInterface->setPreGamePhase(phase);
                        
                        if(phase == ROOM_JOINED_WAITING_FOR_SERVER)
                        {
                            m_gameState = WAITING_FOR_SERVER;
                        }
						else if (phase == CONNECTION_ERROR || phase ==  BATTLE_BOMBS_BETA_CLOSED)
                        {
                            m_gameState = CONNECTION_ERROR_WAITING_FOR_INPUT;
                        }
                    }
                }
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
            
            m_gameState = COUNTING_DOWN;
            
            m_gameListener->playSound(SOUND_COUNT_DOWN_3);
            m_countDownNumbers.push_back(std::unique_ptr<CountDownNumberGameObject>(new CountDownNumberGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2, SCREEN_WIDTH * 2 * 0.76923076923077f, DISPLAY_3)));
            
            for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
            {
                (*itr)->setIsDisplayingName(true);
            }

			m_interfaceOverlay->update(0, *m_player, m_players, m_bombs, m_explosions, m_insideBlocks, m_breakableBlocks, m_iMapType, m_sPlayerIndex, m_gameState);
        }
    }
}

void GameScreen::beginSpectate(rapidjson::Document &d)
{
    if(beginCommon(d, true))
    {
        m_sPlayerIndex = 0;
        m_player = m_players.at(m_sPlayerIndex).get();
        
        spectateNextLivePlayer();
        
        m_gameState = SPECTATING;
        
        Assets::getInstance()->setMusicId(m_iMapType + 2);
    }
}

bool GameScreen::beginCommon(rapidjson::Document &d, bool isBeginGame)
{
    static const char *numPlayersKey = "numPlayers";
    static const char *numClientBotsKey = "numClientBots";
    static const char *mapTypeKey = "mapType";
    static const char *numSecondsLeftForRoundKey = "numSecondsLeftForRound";
    
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
        m_renderer->loadMapType(mapType, m_players);
        
        if(d.HasMember(numSecondsLeftForRoundKey))
        {
            int numSecondsLeftForRound = d[numSecondsLeftForRoundKey].GetInt();
            m_interfaceOverlay->setNumSecondsLeft(numSecondsLeftForRound);
        }
        
        PathFinder::getInstance().resetGameGrid();
        PathFinder::getInstance().initializeGameGrid(m_insideBlocks, m_breakableBlocks, m_iMapType);
        m_interfaceOverlay->initializeMiniMap(m_insideBlocks, m_breakableBlocks, m_iMapType);
        
        return true;
    }
    
    return false;
}

void GameScreen::gameOver(rapidjson::Document &d)
{
    static const char *hasWinnerKey = "hasWinner";
    
    if(d.HasMember(hasWinnerKey))
    {
        bool hasWinner = d[hasWinnerKey].GetBool();
        
        if(hasWinner)
        {
            static const char *winningPlayerIndexKey = "winningPlayerIndex";
            int winningPlayerIndex = d[winningPlayerIndexKey].GetInt();
            m_players.at(winningPlayerIndex).get()->onWin();
            
            // Adjusts the camera
            m_sPlayerIndex = winningPlayerIndex;
            m_player = m_players.at(m_sPlayerIndex).get();
            
            m_player->setIsDisplayingName(true);
            m_player->setIsDisplayingPointer(false);
            
            m_gameListener->playSound(SOUND_GAME_SET);
            
            m_displayGameOvers.push_back(std::unique_ptr<DisplayGameOverGameObject>(new DisplayGameOverGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.80f, GRID_CELL_WIDTH * 14.75f, GRID_CELL_HEIGHT * 1.5f, GAME_SET)));
        }
        else
        {
            m_gameListener->playSound(SOUND_DRAW);
            
            m_displayGameOvers.push_back(std::unique_ptr<DisplayGameOverGameObject>(new DisplayGameOverGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.80f, GRID_CELL_WIDTH * 11.25f, GRID_CELL_HEIGHT * 1.75f, DRAW)));
        }
    }
    
    m_gameState = GAME_ENDING;
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
        
        std::vector<int> breakableBlockXValues;
        std::vector<int> breakableBlockYValues;
        std::vector<int> breakableBlockPowerUpFlags;
        
        handleIntArrayInDocument(d, breakableBlockXValuesKey, breakableBlockXValues, -1);
        handleIntArrayInDocument(d, breakableBlockYValuesKey, breakableBlockYValues, -1);
        handleIntArrayInDocument(d, breakableBlockPowerUpFlagsKey, breakableBlockPowerUpFlags, -1);
        
        int numBreakableBlocks = d[numBreakableBlocksKey].GetInt();
        for(int i = 0; i < numBreakableBlocks; i++)
        {
            m_breakableBlocks.push_back(std::unique_ptr<BreakableBlock>(new BreakableBlock(breakableBlockXValues.at(i), breakableBlockYValues.at(i), breakableBlockPowerUpFlags.at(i))));
        }
    }
}

void GameScreen::clientUpdateForPlayerIndex(rapidjson::Document &d, const char *keyIndex, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame)
{
    if(isBeginGame && d.HasMember(keyIndex))
    {
        const char *username = d[keyIndex].GetString();
        if (std::strcmp(username, m_username.get()) == 0)
        {
            // Now we know which player index the user is
            m_sPlayerIndex = playerIndex;
            m_players.at(playerIndex)->setClientPlayer(true);
        }
        
        m_players.at(playerIndex)->setUsername(username);
    }
    
    if(isBeginGame || playerIndex != m_sPlayerIndex)
    {
        handlePlayerDataUpdate(d, keyIsBot, keyX, keyY, keyDirection, keyAlive, playerIndex, isBeginGame);
    }
}