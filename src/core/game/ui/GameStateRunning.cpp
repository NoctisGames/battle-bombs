//
//  GameStateRunning.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateRunning.h"
#include "GameScreen.h"
#include "Renderer.h"
#include "GameEvent.h"
#include "TouchEvent.h"
#include "InterfaceOverlay.h"

void GameStateRunning::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    updateTimers(gameScreen, deltaTime);
    
    gameScreen->updateRunning(deltaTime);
}

void GameStateRunning::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
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

void GameStateRunning::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
    {
        gameScreen->touchToWorld((*itr));
        
        switch (itr->getTouchType())
        {
            case DOWN:
                gameScreen->getInterfaceOverlay()->handleTouchDownInputRunning(*gameScreen->getTouchPoint(), *gameScreen->getPlayer(), gameScreen->getPlayers(), gameScreen->getBombs());
                continue;
            case DRAGGED:
                gameScreen->getInterfaceOverlay()->handleTouchDraggedInputRunning(*gameScreen->getTouchPoint(), *gameScreen->getPlayer());
                continue;
            case UP:
                gameScreen->getInterfaceOverlay()->handleTouchUpInputRunning(*gameScreen->getTouchPoint(), *gameScreen->getPlayer());
                return;
        }
    }
}

void GameStateRunning::present(GameScreen *gameScreen)
{
    gameScreen->getRenderer()->calcScrollYForPlayer(*gameScreen->getPlayer());
    
    gameScreen->getRenderer()->clearScreenWithColor(0, 0, 0, 1);
    
    gameScreen->getRenderer()->beginFrame();
    
    gameScreen->getRenderer()->renderWorldBackground();
    gameScreen->getRenderer()->renderCraters(gameScreen->getCraters());
    gameScreen->getRenderer()->renderSpaceTiles(gameScreen->getSpaceTiles());
    gameScreen->getRenderer()->renderExplosions(gameScreen->getExplosions());
    gameScreen->getRenderer()->renderBombs(gameScreen->getBombs());
    gameScreen->getRenderer()->renderWorldForeground(gameScreen->getMapBorders(), gameScreen->getInsideBlocks(), gameScreen->getBreakableBlocks(), gameScreen->getRegeneratingDoors(), gameScreen->getPowerUps());
    gameScreen->getRenderer()->renderSuddenDeathMountainsIcePatches(gameScreen->getIcePatches());
    gameScreen->getRenderer()->renderSuddenDeathBaseTiles(gameScreen->getBaseTiles());
    gameScreen->getRenderer()->renderPlayers(gameScreen->getPlayers());
    gameScreen->getRenderer()->renderSuddenDeathGrasslandsFireBalls(gameScreen->getFireBalls());
    gameScreen->getRenderer()->renderSuddenDeathMountainsIceBalls(gameScreen->getIceBalls());
    gameScreen->getRenderer()->renderMapBordersNear(gameScreen->getMapBorders());
    gameScreen->getRenderer()->renderUIEffects(gameScreen->getPlayers(), gameScreen->getCountDownNumberGameObjects(), *gameScreen->getDisplayXMovingGameObject(), gameScreen->getDisplayGameOverGameObjects());
    gameScreen->getRenderer()->renderInterface(*gameScreen->getInterfaceOverlay());
    
    gameScreen->getRenderer()->endFrame();
}

bool GameStateRunning::shouldUpdateGameInterface()
{
    return true;
}