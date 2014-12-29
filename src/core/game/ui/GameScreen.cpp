//
//  GameScreen.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

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
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
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
#include "DisplayXMovingGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "PlayerRow.h"
#include "PlayerRowAvatar.h"
#include "PlayerRowPlatformAvatar.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"
#include "ScreenState.h"
#include "Map.h"
#include "GameState.h"
#include "GameStateFactory.h"
#include "StartButton.h"
#include "EnableBotButton.h"
#include "EnablePowerUpButton.h"
#include "RegeneratingDoor.h"
#include "BaseTile.h"

#include <sstream>

GameScreen::GameScreen(const char *username, bool isOffline) : GameSession()
{
    int usernameLength = (int) strlen(username);
    
    m_username = std::unique_ptr<char>(new char[usernameLength + 1]);
    
    std::strncpy(m_username.get(), username, usernameLength);
    m_username.get()[usernameLength] = '\0';
    
    m_isOffline = isOffline;
    
    if(m_isOffline)
    {
        m_waitingForLocalSettingsInterface = std::unique_ptr<WaitingForLocalSettingsInterface>(new WaitingForLocalSettingsInterface(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, GRID_CELL_WIDTH * 8.25f, GRID_CELL_HEIGHT * 6.25f));
    }
    
    init();
    
    setGameState(GameStateFactory::getInstance().createGameState(m_isOffline ? GAME_STATE_WAITING_FOR_LOCAL_SETTINGS : GAME_STATE_WAITING_FOR_CONNECTION, m_isOffline));
}

void GameScreen::init()
{
    GameSession::init();
    
    m_touchPoint.release();
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    m_displayXMovingGameObject.release();
    m_displayXMovingGameObject = std::unique_ptr<DisplayXMovingGameObject>(new DisplayXMovingGameObject(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, GRID_CELL_WIDTH * 14, GRID_CELL_HEIGHT * 1.75f, BATTLE));
    m_waitingForServerInterface.release();
    m_waitingForServerInterface = std::unique_ptr<WaitingForServerInterface>(new WaitingForServerInterface(SCREEN_WIDTH / 2, 5.81492537375f, 10.38805970149248f, 11.2298507475f, m_username.get()));
    m_interfaceOverlay.release();
    m_interfaceOverlay = std::unique_ptr<InterfaceOverlay>(new InterfaceOverlay(m_gameListener.get()));
    
    m_waitingForLocalSettingsInterface->reset();
    
    m_player = nullptr;
    m_sPlayerIndex = -1;
    
    m_countDownNumbers.clear();
    m_displayGameOvers.clear();
    
    setGameState(GameStateFactory::getInstance().createGameState(m_isOffline ? GAME_STATE_WAITING_FOR_LOCAL_SETTINGS : GAME_STATE_WAITING_FOR_SERVER, m_isOffline));
    
    m_iScreenState = SCREEN_STATE_NORMAL;
    m_fTimeSinceLastClientEvent = 0;
    m_fTimeSinceLastServerUpdate = 0;
    m_fCountDownTimeLeft = 3;
    m_fTimeSinceGameOver = 0;
    m_fBlackCoverTransitionAlpha = 0;
    
    m_hasDisplayed2 = false;
    m_hasDisplayed1 = false;
    
    m_isGameOver = false;
}

void GameScreen::handleServerUpdate(const char *message)
{
    GameSession::handleServerUpdate(message);
    
    m_fTimeSinceLastServerUpdate = 0;
}

void GameScreen::onResume()
{
    // Empty
}

void GameScreen::onPause()
{
    Assets::getInstance()->setMusicId(MUSIC_STOP);
}

void GameScreen::beginGameOffline(int mapType, int numHumanPlayers, int numSecondsLeftForRound, int chosenBotFlags, int chosenPowerUpFlags)
{
    init();
    
    initializeMap(mapType);
    
    m_map->populateMapWithPlayersAndBreakableBlocks(this, numHumanPlayers, chosenBotFlags, chosenPowerUpFlags);
    
    m_renderer->loadMapType(mapType, m_players);
    
    m_interfaceOverlay->initializeMiniMap(this);
    m_interfaceOverlay->initializePlayerAvatars((int)m_players.size());
    m_interfaceOverlay->setNumSecondsLeft(numSecondsLeftForRound);
    
    PathFinder::getInstance().resetGameGrid();
    PathFinder::getInstance().initializeGameGrid(this, m_map.get());
    
    m_sPlayerIndex = 0;
    m_players.at(0)->setClientPlayer(true);
    m_players.at(0)->setUsername(m_username.get());
    
    beginGame();
}

void GameScreen::beginGame(rapidjson::Document &d)
{
    if(beginCommon(d, true))
    {
        beginGame();
    }
}

void GameScreen::beginSpectate(rapidjson::Document &d)
{
    if(beginCommon(d, true))
    {
        m_sPlayerIndex = 0;
        m_player = m_players.at(m_sPlayerIndex).get();
        
        spectateNextLivePlayer();
        
        beginSpectating();
        
        Assets::getInstance()->setMusicId(m_map->getMusicId());
        
        if(!m_isSuddenDeath)
        {
            static const char *isSuddenDeathModeKey = "isSuddenDeathMode";
            if(d.HasMember(isSuddenDeathModeKey))
            {
                bool isSuddenDeathMode = d[isSuddenDeathModeKey].GetBool();
                if(isSuddenDeathMode)
                {
                    static const char *numBreakableBlocksAtSpawnTimeKey = "numBreakableBlocksAtSpawnTime";
                    m_iNumBreakableBlocksAtSpawnTime = d[numBreakableBlocksAtSpawnTimeKey].GetInt();
                    
                    suddenDeath();
                    
                    static const char *timeSinceSuddenDeathModeBeganKey = "timeSinceSuddenDeathModeBegan";
                    if(d.HasMember(timeSinceSuddenDeathModeBeganKey))
                    {
                        float timeSinceSuddenDeathModeBegan = d[timeSinceSuddenDeathModeBeganKey].GetDouble();
                        m_map->handleSpectatorModeSuddenDeath(this, timeSinceSuddenDeathModeBegan);
                    }
                }
            }
        }
    }
}

void GameScreen::update(float deltaTime, std::vector<TouchEvent> &touchEvents)
{
    m_gameState->update(this, deltaTime);
    m_gameState->updateInput(this, touchEvents);
}

void GameScreen::present()
{
    m_gameState->present(this);
}

void GameScreen::updateRunning(float deltaTime)
{
    m_player->handlePowerUps(m_powerUps);
    
    if(m_player->isHitByExplosion(m_explosions, m_bombs))
    {
        m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, *m_player);
        
        beginSpectating();
    }
    
    if(m_isSuddenDeath)
    {
        if(m_map->updatePlayerForSuddenDeath(this, m_player))
        {
            beginSpectating();
        }
    }
    
    updateForOffline(deltaTime);
    
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
    
    m_interfaceOverlay->update(deltaTime, *m_player, this, m_sPlayerIndex, m_gameState.get());
    
    m_displayXMovingGameObject->update(deltaTime);
    
    updateMap(deltaTime);
}

void GameScreen::updateSpectating(float deltaTime)
{
    updateForOffline(deltaTime);
    
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
    
    m_interfaceOverlay->update(deltaTime, *m_player, this, m_sPlayerIndex, m_gameState.get());
    
    m_displayXMovingGameObject->update(deltaTime);
    
    updateMap(deltaTime);
}

void GameScreen::suddenDeath()
{
    GameSession::suddenDeath();
    
    m_displayXMovingGameObject.release();
    m_displayXMovingGameObject = std::unique_ptr<DisplayXMovingGameObject>(new DisplayXMovingGameObject(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, GRID_CELL_WIDTH * 14, GRID_CELL_HEIGHT * 1.75f, HURRY_UP));
    m_displayXMovingGameObject->begin();
    
    m_gameListener->playSound(SOUND_HURRY);
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
            
            float gameSetYValue = m_player->getGridY() < 3 ? SCREEN_HEIGHT * 0.65f : SCREEN_HEIGHT * 0.25f;
            
            m_displayGameOvers.push_back(std::unique_ptr<DisplayGameOverGameObject>(new DisplayGameOverGameObject(SCREEN_WIDTH / 2, gameSetYValue, GRID_CELL_WIDTH * 14.75f, GRID_CELL_HEIGHT * 1.5f, GAME_SET)));
        }
        else
        {
            m_gameListener->playSound(SOUND_DRAW);
            
            m_displayGameOvers.push_back(std::unique_ptr<DisplayGameOverGameObject>(new DisplayGameOverGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.25f, GRID_CELL_WIDTH * 11.25f, GRID_CELL_HEIGHT * 1.75f, DRAW)));
        }
    }
    
    m_isGameOver = true;
    setGameState(GameStateFactory::getInstance().createGameState(GAME_STATE_ENDING, m_isOffline));
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

int GameScreen::getScreenState()
{
	return m_iScreenState;
}

void GameScreen::setScreenState(int screenState)
{
    m_iScreenState = screenState;
}

void GameScreen::clearState()
{
    m_iScreenState = SCREEN_STATE_NORMAL;
}

short GameScreen::getPlayerIndex()
{
    return m_sPlayerIndex;
}

void GameScreen::setPlayerIndex(short playerIndex)
{
    m_sPlayerIndex = playerIndex;
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

void GameScreen::setGameState(GameState *gameState)
{
    m_gameState.release();
    m_gameState = std::unique_ptr<GameState>(gameState);
}

Renderer * GameScreen::getRenderer()
{
    return m_renderer.get();
}

Vector2D * GameScreen::getTouchPoint()
{
    return m_touchPoint.get();
}

PlayerDynamicGameObject * GameScreen::getPlayer()
{
    return m_player;
}

std::vector<std::unique_ptr<CountDownNumberGameObject>> & GameScreen::getCountDownNumberGameObjects()
{
    return m_countDownNumbers;
}

DisplayXMovingGameObject * GameScreen::getDisplayXMovingGameObject()
{
    return m_displayXMovingGameObject.get();
}

std::vector<std::unique_ptr<DisplayGameOverGameObject>> & GameScreen::getDisplayGameOverGameObjects()
{
    return m_displayGameOvers;
}

WaitingForServerInterface * GameScreen::getWaitingForServerInterface()
{
    return m_waitingForServerInterface.get();
}

WaitingForLocalSettingsInterface * GameScreen::getWaitingForLocalSettingsInterface()
{
    return m_waitingForLocalSettingsInterface.get();
}

InterfaceOverlay * GameScreen::getInterfaceOverlay()
{
    return m_interfaceOverlay.get();
}

float GameScreen::getTimeSinceLastClientEvent()
{
    return m_fTimeSinceLastClientEvent;
}

void GameScreen::setTimeSinceLastClientEvent(float timeSinceLastClientEvent)
{
    m_fTimeSinceLastClientEvent = timeSinceLastClientEvent;
}

float GameScreen::getTimeSinceLastServerUpdate()
{
    return m_fTimeSinceLastServerUpdate;
}

void GameScreen::setTimeSinceLastServerUpdate(float timeSinceLastServerUpdate)
{
    m_fTimeSinceLastServerUpdate = timeSinceLastServerUpdate;
}

float GameScreen::getTimeSinceGameOver()
{
    return m_fTimeSinceGameOver;
}

void GameScreen::setTimeSinceGameOver(float timeSinceGameOver)
{
    m_fTimeSinceGameOver = timeSinceGameOver;
}

float GameScreen::getBlackCoverTransitionAlpha()
{
    return m_fBlackCoverTransitionAlpha;
}

void GameScreen::setBlackCoverTransitionAlpha(float blackCoverTransitionAlpha)
{
    m_fBlackCoverTransitionAlpha = blackCoverTransitionAlpha;
}

bool GameScreen::hasDisplayed2()
{
    return m_hasDisplayed2;
}

void GameScreen::setHasDisplayed2(bool hasDisplayed2)
{
    m_hasDisplayed2 = hasDisplayed2;
}

bool GameScreen::hasDisplayed1()
{
    return m_hasDisplayed1;
}

void GameScreen::setHasDisplayed1(bool hasDisplayed1)
{
    m_hasDisplayed1 = hasDisplayed1;
}

#pragma mark <Private>

void GameScreen::beginGame()
{
    if(m_sPlayerIndex != -1)
    {
        m_player = m_players.at(m_sPlayerIndex).get();
        
        setGameState(GameStateFactory::getInstance().createGameState(GAME_STATE_COUNTING_DOWN, m_isOffline));
        
        m_gameListener->playSound(SOUND_COUNT_DOWN_3);
        m_countDownNumbers.push_back(std::unique_ptr<CountDownNumberGameObject>(new CountDownNumberGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2, SCREEN_WIDTH * 2 * 0.76923076923077f, DISPLAY_3)));
        
        for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
        {
            (*itr)->setIsDisplayingName(true);
        }
        
        m_interfaceOverlay->update(0, *m_player, this, m_sPlayerIndex, m_gameState.get());
    }
}

void GameScreen::beginSpectating()
{
    setGameState(GameStateFactory::getInstance().createGameState(GAME_STATE_SPECTATING, m_isOffline));
    m_iScreenState = SCREEN_STATE_ENTERED_SPECTATOR_MODE;
}

void GameScreen::updateForOffline(float deltaTime)
{
    if(m_isOffline)
    {
        updateBots();
        
        int numAlive = 0;
        int winningPlayerIndex = -1;
        for (int i = 0; i < m_players.size(); i++)
        {
            if(m_players.at(i)->getPlayerState() == ALIVE)
            {
                winningPlayerIndex = i;
                numAlive++;
            }
        }
        
        if(!m_isGameOver)
        {
            if(numAlive <= 1)
            {
                bool hasWinner = numAlive == 1;
                
                rapidjson::Document d;
                std::stringstream ss;
                ss << "{\"eventType\": " << GAME_OVER << ", \"hasWinner\": " << (hasWinner ? "true" : "false") << ", \"winningPlayerIndex\": " << winningPlayerIndex << "}";
                std::string gameOverMessage = ss.str();
                
                d.Parse<0>(gameOverMessage.c_str());
                
                gameOver(d);
            }
            else if(getNumSecondsLeft() <= 0)
            {
                rapidjson::Document d;
                std::stringstream ss;
                ss << "{\"eventType\": " << GAME_OVER << ", \"hasWinner\": false, \"winningPlayerIndex\": " << -1 << "}";
                std::string gameOverMessage = ss.str();
                
                d.Parse<0>(gameOverMessage.c_str());
                
                gameOver(d);
            }
        }
        
        if(!m_isSuddenDeath)
        {
            if(getNumSecondsLeft() <= 60)
            {
                suddenDeath();
            }
        }
    }
}

// Server Stuff

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
            m_players.push_back(std::unique_ptr<PlayerDynamicGameObject>(new PlayerDynamicGameObject(i, -5, 0, m_gameListener.get())));
        }
        
        if(d.HasMember(numClientBotsKey))
        {
            int numClientBots = d[numClientBotsKey].GetInt();
            for(int i = 0; i < numClientBots; i++)
            {
                m_players.push_back(std::unique_ptr<BotPlayerDynamicGameObject>(new BotPlayerDynamicGameObject(i + numPlayers, -5, 0, m_gameListener.get())));
            }
        }
        
        clientUpdate(d, isBeginGame);
        
        handleBreakableBlocksArrayInDocument(d);
        
        int mapType = d[mapTypeKey].GetInt();
        initializeMap(mapType);
        m_renderer->loadMapType(mapType, m_players);
        
        if(d.HasMember(numSecondsLeftForRoundKey))
        {
            int numSecondsLeftForRound = d[numSecondsLeftForRoundKey].GetInt();
            m_interfaceOverlay->setNumSecondsLeft(numSecondsLeftForRound);
        }
        
        PathFinder::getInstance().resetGameGrid();
        PathFinder::getInstance().initializeGameGrid(this, m_map.get());
        m_interfaceOverlay->initializeMiniMap(this);
        m_interfaceOverlay->initializePlayerAvatars((int)m_players.size());
        
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
        
        std::vector<int> breakableBlockXValues;
        std::vector<int> breakableBlockYValues;
        std::vector<int> breakableBlockPowerUpFlags;
        
        handleIntArrayInDocument(d, breakableBlockXValuesKey, breakableBlockXValues, -1);
        handleIntArrayInDocument(d, breakableBlockYValuesKey, breakableBlockYValues, -1);
        handleIntArrayInDocument(d, breakableBlockPowerUpFlagsKey, breakableBlockPowerUpFlags, -1);
        
        m_iNumBreakableBlocksAtSpawnTime = d[numBreakableBlocksKey].GetInt();
        for(int i = 0; i < m_iNumBreakableBlocksAtSpawnTime; i++)
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
    
    if(m_gameState->shouldHandlePlayerDataUpdate(this, playerIndex, isBeginGame))
    {
        handlePlayerDataUpdate(d, keyIsBot, keyX, keyY, keyDirection, keyAlive, playerIndex, isBeginGame);
    }
}