//
//  GameStateSpectating.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateSpectating.h"
#include "GameScreen.h"
#include "Renderer.h"
#include "GameEvent.h"
#include "TouchEvent.h"
#include "InterfaceOverlay.h"

void GameStateSpectating::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    updateTimers(gameScreen, deltaTime);
    
    gameScreen->updateSpectating(deltaTime);
}

void GameStateSpectating::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    switch (eventType)
    {
        case CLIENT_UPDATE:
            gameScreen->clientUpdate(d, false);
            break;
        case SUDDEN_DEATH:
            gameScreen->suddenDeath();
            break;
        case GAME_OVER:
            gameScreen->gameOver(d);
            break;
        case HARD_UPDATE:
            gameScreen->hardUpdate(d);
            break;
        default:
            break;
    }
}

void GameStateSpectating::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
    {
        gameScreen->touchToWorld((*itr));
        
        switch (itr->getTouchType())
        {
            case DOWN:
                if(gameScreen->getInterfaceOverlay()->handleTouchDownInputSpectating(*gameScreen->getTouchPoint()))
                {
                    continue;
                }
                
                // Basically, if the user isn't touching the arrows,
                // assume they want to pan the game world using their finger
                continue;
            case DRAGGED:
                if(gameScreen->getInterfaceOverlay()->handleTouchDraggedInputSpectating(*gameScreen->getTouchPoint()))
                {
                    continue;
                }
                
                // Basically, if the user isn't touching the arrows,
                // assume they want to pan the game world using their finger
                continue;
            case UP:
                Spectator_Control_State spectatorControlState = gameScreen->getInterfaceOverlay()->handleTouchUpInputSpectating(*gameScreen->getTouchPoint());
                if(spectatorControlState == LEFT_ARROW_HIGHLIGHTED)
                {
                    gameScreen->spectatePreviousLivePlayer();
                }
                else if(spectatorControlState == RIGHT_ARROW_HIGHLIGHTED)
                {
                    gameScreen->spectateNextLivePlayer();
                }
                return;
        }
    }
}

void GameStateSpectating::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->calcScrollYForPlayer(*gameScreen->getPlayer());
    
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWorldBackground();
    gameScreen->getRenderer()->renderCraters(gameScreen->getCraters());
    gameScreen->getRenderer()->renderSpaceTiles(gameScreen->getSpaceTiles());
    gameScreen->getRenderer()->renderExplosions(gameScreen->getExplosions());
    gameScreen->getRenderer()->renderBombs(gameScreen->getBombs(), gameScreen->getLandmines());
    gameScreen->getRenderer()->renderWorldForeground(gameScreen->getMapBorders(), gameScreen->getInsideBlocks(), gameScreen->getBreakableBlocks(), gameScreen->getRegeneratingDoors(), gameScreen->getPowerUps());
    gameScreen->getRenderer()->renderSuddenDeathMountainsIcePatches(gameScreen->getIcePatches());
    gameScreen->getRenderer()->renderSuddenDeathBaseTiles(gameScreen->getBaseTiles());
    gameScreen->getRenderer()->renderPlayers(gameScreen->getPlayers());
    gameScreen->getRenderer()->renderSuddenDeathGrasslandsFireBalls(gameScreen->getFireBalls());
    gameScreen->getRenderer()->renderSuddenDeathMountainsIceBalls(gameScreen->getIceBalls());
    gameScreen->getRenderer()->renderMapBordersNear(gameScreen->getMapBorders());
    gameScreen->getRenderer()->renderUIEffects(gameScreen->getPlayers(), gameScreen->getCountDownNumberGameObjects(), *gameScreen->getDisplayXMovingGameObject(), gameScreen->getDisplayGameOverGameObjects());
    gameScreen->getRenderer()->renderSpectatorInterface(*gameScreen->getInterfaceOverlay());
    
    gameScreen->getRenderer()->endFrame();
}

bool GameStateSpectating::shouldUpdateSpectatorInterface()
{
    return true;
}

bool GameStateSpectating::shouldHandlePlayerDataUpdate(GameScreen *gameScreen, short playerIndex, bool isBeginGame)
{
    return true;
}