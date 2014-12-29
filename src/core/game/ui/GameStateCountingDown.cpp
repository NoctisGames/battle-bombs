//
//  GameStateCountingDown.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateCountingDown.h"
#include "GameSession.h"
#include "GameScreen.h"
#include "GameListener.h"
#include "Renderer.h"
#include "ResourceConstants.h"
#include "DisplayXMovingGameObject.h"
#include "GameConstants.h"
#include "PlayerDynamicGameObject.h"
#include "Assets.h"
#include "Map.h"
#include "CountDownNumberGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"

void GameStateCountingDown::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    updateTimers(gameScreen, deltaTime);
    
    gameScreen->setCountDownTimeLeft(gameScreen->getCountDownTimeLeft() - deltaTime);
    if(gameScreen->getCountDownTimeLeft() < 0)
    {
        gameScreen->getCountDownNumberGameObjects().clear();
        
        gameScreen->getGameListener()->playSound(SOUND_BATTLE);
        gameScreen->getDisplayXMovingGameObject()->begin();
        setGameState(gameScreen, GAME_STATE_RUNNING);
        
        for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = gameScreen->getPlayers().begin(); itr != gameScreen->getPlayers().end(); itr++)
        {
            (*itr)->setIsDisplayingName(false);
        }
        
        Assets::getInstance()->setMusicId(gameScreen->getMap()->getMusicId());
    }
    else if(gameScreen->getCountDownTimeLeft() <= 1 && !gameScreen->hasDisplayed1())
    {
        gameScreen->getGameListener()->playSound(SOUND_COUNT_DOWN_1);
        gameScreen->getCountDownNumberGameObjects().push_back(std::unique_ptr<CountDownNumberGameObject>(new CountDownNumberGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2, SCREEN_WIDTH * 2 * 0.76923076923077f, DISPLAY_1)));
        gameScreen->setHasDisplayed1(true);
    }
    else if(gameScreen->getCountDownTimeLeft() <= 2 && !gameScreen->hasDisplayed2())
    {
        gameScreen->getGameListener()->playSound(SOUND_COUNT_DOWN_2);
        gameScreen->getCountDownNumberGameObjects().push_back(std::unique_ptr<CountDownNumberGameObject>(new CountDownNumberGameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2, SCREEN_WIDTH * 2 * 0.76923076923077f, DISPLAY_2)));
        gameScreen->setHasDisplayed2(true);
    }
    
    for (std::vector < std::unique_ptr < CountDownNumberGameObject >> ::iterator itr = gameScreen->getCountDownNumberGameObjects().begin(); itr != gameScreen->getCountDownNumberGameObjects().end(); )
    {
        (*itr)->update(deltaTime);
        
        if ((*itr)->remove())
        {
            itr = gameScreen->getCountDownNumberGameObjects().erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

void GameStateCountingDown::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // We are only counting down, don't process messages again until state is RUNNING or SPECTATING
}

void GameStateCountingDown::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    // TODO
}

void GameStateCountingDown::present(GameScreen *gameScreen)
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
    
    gameScreen->getRenderer()->endFrame();
}

bool GameStateCountingDown::shouldUpdateGameInterface()
{
    return true;
}