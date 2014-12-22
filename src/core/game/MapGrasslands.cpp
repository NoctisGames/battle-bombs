//
//  MapGrasslands.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "MapGrasslands.h"
#include "PlayerDynamicGameObject.h"
#include "GameSession.h"
#include "GameListener.h"
#include "GameEvent.h"
#include "ResourceConstants.h"
#include "FireBall.h"
#include "FallingObjectShadow.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "Crater.h"
#include "Rectangle.h"
#include "Vector2D.h"

bool MapGrasslands::updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player)
{
    if(player->isHitByFireBall(gameSession->getCraters()))
    {
        gameSession->getGameListener()->addLocalEventForPlayer(PLAYER_DEATH, *player);
        
        return true;
    }
    
    return false;
}

short MapGrasslands::getMusicId()
{
    return MUSIC_PLAY_MAP_GRASSLANDS;
}

void MapGrasslands::handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan)
{
    for (std::vector < std::unique_ptr < FireBall >> ::iterator itr = gameSession->getFireBalls().begin(); itr != gameSession->getFireBalls().end(); itr++)
    {
        (*itr)->handleTimeSinceSuddenDeathModeBegan(timeSinceSuddenDeathModeBegan);
    }
}

void MapGrasslands::suddenDeath(GameSession *gameSession)
{
    gameSession->getFireBalls().clear();
    
    static const char *fireBallXValuesChar1 = "4,1,12,10,9,7,5,4,13,14,12,13,8,8,8,0,10,12,13,1,10,3,14,9,0,6,6,9,7,8,13,4,2,0,1,9,1,0,7,2,13,2,3,2,11,10,13,13,10,6,0,11,8,2,7,3,14,12,11,2,5,1,12,6,9,7,7,13,6,5,9,10,2,5,12,10,9,13,4,6,8,5,5,6,7,0,9,7,10,13,13,10,1,0,7,8,6,4,11,5,5,11,8,12,9,7,0,7,4,8,2,7,11,8,12,13,1,11,2,6,11,0,3,0,14,3,5,8,3,1,14,5,14,4,3,6,11,4,5,9,12,4,11,4,3,10,12,11,14,9,11,7,7,13,11,1,3,11,12,0,6,9,8,6,0,6,4,1,14,2,10,5,2,9,3,3,7,10,14,4,11,9,1,5,13,4,3,8,10,11,10,12,3,4,9,14,10,1,5,8,3,9,7,10,11,8,3,0,14,4,7,3,2,14,6,8,1,12,10,9,2,4,14,2,12,0,3,6,5,14,6,8,4,5,1,5,6,-1";
    static const char *fireBallYValuesChar1 = "16,16,5,1,12,8,5,0,12,14,12,11,3,0,10,5,15,3,14,4,13,2,16,6,4,7,13,10,1,15,13,14,4,11,3,2,8,13,11,7,15,3,4,16,14,7,8,16,6,11,8,4,4,8,6,8,5,4,7,14,4,10,7,0,11,13,16,5,16,12,16,9,13,13,8,12,13,7,15,8,12,15,16,9,3,6,0,15,11,9,6,14,6,9,7,5,6,9,16,1,0,10,8,13,5,5,16,12,11,13,6,0,1,7,9,4,11,6,15,2,2,7,13,12,15,5,10,6,3,13,6,9,9,2,14,15,0,12,6,8,10,7,12,8,15,0,6,13,4,1,9,2,14,10,3,9,9,8,16,15,10,9,11,5,14,14,4,7,7,10,5,2,12,4,10,12,10,4,12,6,5,14,5,7,3,10,1,9,2,11,8,14,16,13,3,3,10,15,3,14,0,7,4,16,15,1,11,3,10,1,9,6,5,8,4,2,14,11,3,15,9,5,11,11,15,10,7,1,8,13,12,16,3,14,12,11,3,-1";
    
    static const char *fireBallXValuesChar2 = "9,4,6,14,1,10,8,5,8,5,4,6,11,13,7,6,12,7,3,12,11,1,8,1,14,0,11,12,1,2,0,6,0,9,11,12,8,12,7,4,7,12,14,6,5,14,2,13,5,10,6,14,13,5,0,10,10,3,4,10,11,0,8,12,3,3,6,4,13,1,10,3,8,3,2,4,11,2,10,10,13,5,6,9,14,4,0,13,2,14,2,1,5,8,6,14,2,9,10,5,2,3,3,3,0,13,9,6,13,6,8,11,1,2,9,11,8,8,2,8,1,7,9,6,8,9,14,9,11,10,7,11,5,14,13,9,9,1,7,9,5,5,7,2,6,13,5,2,12,4,11,11,5,6,10,4,10,5,5,12,8,11,0,4,2,7,1,14,13,7,10,10,1,11,3,10,10,8,5,0,13,7,9,12,14,4,8,6,7,13,0,0,2,4,8,11,4,3,10,11,12,11,14,3,7,3,9,4,7,0,5,7,3,7,0,12,3,4,0,3,9,14,4,4,7,6,9,1,9,12,1,1,13,12,8,3,6,-1";
    static const char *fireBallYValuesChar2 = "15,15,1,8,13,1,0,6,5,1,1,12,6,12,0,7,13,7,4,8,3,8,10,6,4,8,7,14,16,7,6,11,12,1,8,15,8,11,5,16,11,3,3,16,13,6,15,9,3,14,0,16,14,8,15,15,13,5,6,2,15,10,6,5,6,10,6,13,7,11,4,8,12,9,12,3,2,6,8,12,6,9,15,0,13,7,4,3,3,11,10,14,10,2,3,15,14,11,9,4,8,1,3,16,14,4,10,2,15,13,3,4,7,13,12,1,1,9,4,15,9,6,5,14,4,7,9,13,9,3,2,11,5,14,11,3,4,4,13,9,16,14,14,9,9,8,11,16,10,0,10,0,2,4,11,5,16,0,12,12,11,12,9,11,5,1,15,10,13,9,6,5,3,13,12,10,7,13,15,7,5,4,16,9,12,12,7,8,10,16,13,3,11,9,14,14,4,14,0,16,4,5,7,15,12,2,14,8,3,5,7,15,13,16,11,16,0,2,16,7,6,5,10,14,8,5,8,5,2,6,10,12,10,7,16,11,10,-1";
    
    std::vector<int> fireBallXValues;
    std::vector<int> fireBallYValues;
    
    bool isNumBreakableBlocksAtSpawnTimeEven = gameSession->getNumBreakableBlocksAtSpawnTime() % 2 == 0;
    
    gameSession->readCharArrayIntoIntArray(isNumBreakableBlocksAtSpawnTimeEven ? fireBallXValuesChar1 : fireBallXValuesChar2, fireBallXValues, -1);
    gameSession->readCharArrayIntoIntArray(isNumBreakableBlocksAtSpawnTimeEven ? fireBallYValuesChar1 : fireBallYValuesChar2, fireBallYValues, -1);
    
    static int numFireBalls = 237;
    for(int i = 0; i < numFireBalls; i++)
    {
        gameSession->getFireBalls().push_back(std::unique_ptr<FireBall>(new FireBall(fireBallXValues.at(i), fireBallYValues.at(i), i, gameSession->getGameListener(), gameSession->getInsideBlocks(), gameSession->getBreakableBlocks())));
    }
}

void MapGrasslands::initializeMap(GameSession *gameSession)
{
    Map::initializeMap(gameSession);
    
    addCommonInsideBlocks(gameSession->getInsideBlocks());
    
    // BEGIN TOP
    for (int i = TOP_HALF_BOTTOM_GRID_Y + 1; i < GRID_CELL_NUM_ROWS; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
}

void MapGrasslands::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    Map::update(gameSession, deltaTime, isSuddenDeath);
    
    if(isSuddenDeath)
    {
        for (std::vector < std::unique_ptr < FireBall >> ::iterator itr = gameSession->getFireBalls().begin(); itr != gameSession->getFireBalls().end(); )
        {
            (*itr)->update(deltaTime, gameSession->getBreakableBlocks());
            
            bool continueIteration = true;
            
            if ((*itr)->isDescending() && (*itr)->isTargetReached())
            {
                if((*itr)->getShadow().isTargetOccupiedByInsideBlock())
                {
                    InsideBlock *insideBlock = (*itr)->getShadow().getTargetInsideBlock();
                    insideBlock->onDestroy();
                }
                else if((*itr)->getShadow().isTargetOccupiedByBreakableBlock())
                {
                    BreakableBlock *breakableBlock = (*itr)->getShadow().getTargetBreakableBlock();
                    breakableBlock->onHitByFireBall();
                }
                
                gameSession->getCraters().push_back(std::unique_ptr<Crater>(new Crater((*itr)->getGridX(), (*itr)->getGridY())));
            }
            
            if((*itr)->isExplosionCompleted())
            {
                itr = gameSession->getFireBalls().erase(itr);
                continueIteration = false;
            }
            
            if (continueIteration)
            {
                itr++;
            }
        }
    }
}