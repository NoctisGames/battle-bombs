//
//  MapMountains.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "MapMountains.h"
#include "PlayerDynamicGameObject.h"
#include "GameSession.h"
#include "GameListener.h"
#include "GameEvent.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "ResourceConstants.h"
#include "IceBall.h"
#include "BreakableBlock.h"
#include "FallingObjectShadow.h"
#include "PathFinder.h"
#include "IcePatch.h"
#include "Rectangle.h"
#include "Vector2D.h"

bool MapMountains::updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player)
{
    if(player->isHitByIce(gameSession->getIcePatches()))
    {
        gameSession->getGameListener()->addLocalEventForPlayer(PLAYER_FREEZE, *player);
        
        return true;
    }
    
    return false;
}

short MapMountains::getMusicId()
{
    return MUSIC_PLAY_MAP_MOUNTAINS;
}

void MapMountains::handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan)
{
    for (std::vector < std::unique_ptr < IceBall >> ::iterator itr = gameSession->getIceBalls().begin(); itr != gameSession->getIceBalls().end(); itr++)
    {
        (*itr)->handleTimeSinceSuddenDeathModeBegan(timeSinceSuddenDeathModeBegan);
    }
}

void MapMountains::suddenDeath(GameSession *gameSession)
{
    gameSession->getIceBalls().clear();
    
    static const char *iceBallXValuesChar = "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,11,10,9,8,7,6,5,4,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,13,13,13,13,13,13,13,13,13,13,13,11,10,9,8,7,6,5,4,3,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,5,6,7,8,9,10,11,12,12,12,12,12,12,12,12,12,12,12,12,11,10,9,8,7,6,5,4,3,2,2,2,2,2,2,2,2,2,2,2,3,4,5,6,7,8,9,10,11,11,11,11,11,11,11,11,11,11,11,10,9,8,7,6,5,4,3,3,3,3,3,3,3,3,3,3,4,5,6,7,8,9,10,10,10,10,10,10,10,10,10,9,8,7,6,5,4,4,4,4,4,4,4,4,5,6,7,8,9,9,9,9,9,9,9,8,7,6,5,5,5,5,5,5,6,7,8,8,8,8,8,7,6,6,6,6,7,7,7,7,-1";
    static const char *iceBallYValuesChar = "16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,15,14,13,12,11,10,9,8,7,6,5,4,3,0,0,0,0,0,0,0,0,0,3,4,5,6,7,8,9,10,11,12,13,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,14,13,12,11,10,9,8,7,6,5,4,3,1,1,1,1,1,1,1,1,1,3,4,5,6,7,8,9,10,11,12,13,14,14,14,14,14,14,14,14,14,14,14,14,13,12,11,10,9,8,7,6,5,4,3,2,2,2,2,2,2,2,2,2,3,4,5,6,7,8,9,10,11,12,13,13,13,13,13,13,13,13,13,13,12,11,10,9,8,7,6,5,4,3,3,3,3,3,3,3,3,3,4,5,6,7,8,9,10,11,12,12,12,12,12,12,12,12,11,10,9,8,7,6,5,4,4,4,4,4,4,4,5,6,7,8,9,10,11,11,11,11,11,11,10,9,8,7,6,5,5,5,5,5,6,7,8,9,10,10,10,10,9,8,7,6,6,6,7,8,9,9,8,7,8,-1";
    
    std::vector<int> iceBallXValues;
    std::vector<int> iceBallYValues;
    
    gameSession->readCharArrayIntoIntArray(iceBallXValuesChar, iceBallXValues, -1);
    gameSession->readCharArrayIntoIntArray(iceBallYValuesChar, iceBallYValues, -1);
    
    static int numIceBalls = 238;
    for(int i = 0; i < numIceBalls; i++)
    {
        gameSession->getIceBalls().push_back(std::unique_ptr<IceBall>(new IceBall(iceBallXValues.at(i), iceBallYValues.at(i), i, gameSession->getGameListener(), gameSession->getInsideBlocks(), gameSession->getBreakableBlocks())));
    }
}

void MapMountains::initializeMap(GameSession *gameSession)
{
    Map::initializeMap(gameSession);
    
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(MOUNTAINS_DOOR, GAME_X + GRID_CELL_WIDTH * 7 + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * GRID_CELL_NUM_ROWS + GRID_CELL_HEIGHT / 2, GRID_CELL_WIDTH * 3, GRID_CELL_HEIGHT * 3)));
    
    addCommonInsideBlocks(gameSession->getInsideBlocks());
    
    // BEGIN TOP
    for (int i = TOP_HALF_BOTTOM_GRID_Y + 1; i < GRID_CELL_NUM_ROWS; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            if(i >= GRID_CELL_NUM_ROWS - 2 && j >= 5 && j <= 9)
            {
                continue;
            }
            
            gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
}

void MapMountains::initializeGameGrid(GameSession *gameSession, int (&gameGrid)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
{
    Map::initializeGameGrid(gameSession, gameGrid);
    
    gameGrid[6][GRID_CELL_NUM_ROWS - 1] = 9;
    gameGrid[7][GRID_CELL_NUM_ROWS - 1] = 9;
    gameGrid[8][GRID_CELL_NUM_ROWS - 1] = 9;
}

void MapMountains::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    Map::update(gameSession, deltaTime, isSuddenDeath);
    
    if(isSuddenDeath)
    {
        for (std::vector < std::unique_ptr < IceBall >> ::iterator itr = gameSession->getIceBalls().begin(); itr != gameSession->getIceBalls().end(); )
        {
            (*itr)->update(deltaTime, gameSession->getBreakableBlocks());
            
            if ((*itr)->isTargetReached())
            {
                if((*itr)->getShadow().isTargetOccupiedByInsideBlock())
                {
                    InsideBlock *insideBlock = (*itr)->getShadow().getTargetInsideBlock();
                    insideBlock->onHitByIceBall();
                }
                else if((*itr)->getShadow().isTargetOccupiedByBreakableBlock())
                {
                    BreakableBlock *breakableBlock = (*itr)->getShadow().getTargetBreakableBlock();
                    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = gameSession->getBreakableBlocks().begin(); itr != gameSession->getBreakableBlocks().end(); itr++)
                    {
                        if(breakableBlock == (*itr).get())
                        {
                            // Test to see if breakable block still exists
                            breakableBlock->onHitByIceBall();
                            break;
                        }
                    }
                }
                else
                {
                    gameSession->getIcePatches().push_back(std::unique_ptr<IcePatch>(new IcePatch((*itr)->getGridX(), (*itr)->getGridY())));
                }
                
                PathFinder::getInstance().occupyGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
                
                itr = gameSession->getIceBalls().erase(itr);
            }
            else
            {
                itr++;
            }
        }
        
        for (std::vector < std::unique_ptr < IcePatch >> ::iterator itr = gameSession->getIcePatches().begin(); itr != gameSession->getIcePatches().end(); itr++)
        {
            (**itr).update(deltaTime, gameSession->getBombs());
        }
    }
}