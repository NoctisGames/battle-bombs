//
//  GameStateEnding.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateEnding.h"
#include "GameScreen.h"
#include "Renderer.h"
#include "DisplayGameOverGameObject.h"
#include "Assets.h"

void GameStateEnding::update(GameScreen *gameScreen, float deltaTime)
{
    GameState::update(gameScreen, deltaTime);
    
    updateTimers(gameScreen, deltaTime);
    
    gameScreen->setTimeSinceGameOver(gameScreen->getTimeSinceGameOver() + deltaTime);
    
    for (std::vector < std::unique_ptr < DisplayGameOverGameObject >> ::iterator itr = gameScreen->getDisplayGameOverGameObjects().begin(); itr != gameScreen->getDisplayGameOverGameObjects().end(); itr++)
    {
        (*itr)->update(deltaTime);
    }
    
    if(gameScreen->getTimeSinceGameOver() > 6)
    {
        gameScreen->setBlackCoverTransitionAlpha(gameScreen->getBlackCoverTransitionAlpha() + deltaTime * 0.40f);
        if(gameScreen->getBlackCoverTransitionAlpha() > 1)
        {
            Assets::getInstance()->setMusicId(MUSIC_STOP);
            
            gameScreen->init();
            
            return;
        }
    }
    
    gameScreen->updateSpectating(deltaTime / (gameScreen->getTimeSinceGameOver() + 1));
}

void GameStateEnding::processServerMessage(GameScreen *gameScreen, rapidjson::Document &d, int eventType)
{
    // We are only fading to black here, no need for processing messages
}

void GameStateEnding::updateInput(GameScreen *gameScreen, std::vector<TouchEvent> &touchEvents)
{
    // TODO
}

void GameStateEnding::present(GameScreen *gameScreen)
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
    gameScreen->getRenderer()->renderGameOverBlackCover(gameScreen->getBlackCoverTransitionAlpha());
    
    gameScreen->getRenderer()->endFrame();
}