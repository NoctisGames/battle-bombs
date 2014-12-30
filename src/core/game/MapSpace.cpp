//
//  MapSpace.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "MapSpace.h"
#include "GameSession.h"
#include "PlayerDynamicGameObject.h"
#include "GameListener.h"
#include "GameEvent.h"
#include "SpaceTile.h"
#include "InsideBlock.h"
#include "ResourceConstants.h"
#include "Rectangle.h"
#include "Vector2D.h"

bool MapSpace::updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player)
{
    if(player->isTrappedOnFallingSpaceTile(gameSession->getSpaceTiles()))
    {
        gameSession->getGameListener()->addLocalEventForPlayer(PLAYER_ABOUT_TO_FALL, *player);
    }
    else if(player->isFallingDueToSpaceTile(gameSession->getSpaceTiles()))
    {
        gameSession->getGameListener()->addLocalEventForPlayer(PLAYER_FALL, *player);
        
        return true;
    }
    
    return false;
}

short MapSpace::getMusicId()
{
    return MUSIC_PLAY_MAP_SPACE;
}

void MapSpace::handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan)
{
    for (std::vector < std::unique_ptr < SpaceTile >> ::iterator itr = gameSession->getSpaceTiles().begin(); itr != gameSession->getSpaceTiles().end(); itr++)
    {
        (*itr)->handleTimeSinceSuddenDeathModeBegan(timeSinceSuddenDeathModeBegan);
    }
}

void MapSpace::suddenDeath(GameSession *gameSession)
{
    // Currently not needed
}

void MapSpace::initializeMap(GameSession *gameSession)
{
    Map::initializeMap(gameSession);
    
    // BEGIN TOP
    int fallingIndex = 118;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 1, tempFallingIndex, gameSession->getGameListener())));
    }
    
    fallingIndex = 114;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + (j / 2) : fallingIndex + (7 - j / 2);
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 2, tempFallingIndex, gameSession->getGameListener())));
    }
    
    fallingIndex = 106;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 3, tempFallingIndex, gameSession->getGameListener())));
    }
    
    fallingIndex = 102;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + (j / 2) : fallingIndex + (7 - j / 2);
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 4, tempFallingIndex, gameSession->getGameListener())));
    }
    
    fallingIndex = 94;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 5, tempFallingIndex, gameSession->getGameListener())));
    }
    
    fallingIndex = 90;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + (j / 2) : fallingIndex + (7 - j / 2);
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 6, tempFallingIndex, gameSession->getGameListener())));
    }
    
    fallingIndex = 82;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 7, tempFallingIndex, gameSession->getGameListener())));
    }
    
    // BEGIN MIDDLE
    fallingIndex = 80;
    for (int j = 2; j < (NUM_GRID_CELLS_PER_ROW - 2); j += 2)
    {
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, TOP_HALF_BOTTOM_GRID_Y - 1, fallingIndex + 1, gameSession->getGameListener())));
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y + 1, fallingIndex, gameSession->getGameListener())));
    }
    
    // BEGIN BOTTOM
    fallingIndex = 79;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y, fallingIndex--, gameSession->getGameListener())));
    }
    
    fallingIndex = 57;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 1, fallingIndex++, gameSession->getGameListener())));
    }
    
    fallingIndex = 56;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 2, fallingIndex--, gameSession->getGameListener())));
    }
    
    fallingIndex = 35;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 3, fallingIndex++, gameSession->getGameListener())));
    }
    
    fallingIndex = 34;
    for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
    {
        gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 4, fallingIndex--, gameSession->getGameListener())));
    }
    
    // BEGIN BOTTOM CENTER
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(3, 2, 15, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(5, 2, 16, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(7, 2, 17, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(9, 2, 18, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(11, 2, 19, gameSession->getGameListener())));
    
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(3, 1, 14, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(4, 1, 13, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(5, 1, 12, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(6, 1, 11, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(7, 1, 10, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(8, 1, 9, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(9, 1, 8, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(10, 1, 7, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(11, 1, 6, gameSession->getGameListener())));
    
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(3, 0, 0, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(4, 0, 1, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(6, 0, 2, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(8, 0, 3, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(10, 0, 4, gameSession->getGameListener())));
    gameSession->getSpaceTiles().push_back(std::unique_ptr<SpaceTile>(new SpaceTile(11, 0, 5, gameSession->getGameListener())));
    
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

void MapSpace::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    Map::update(gameSession, deltaTime, isSuddenDeath);
    
    if(isSuddenDeath)
    {
        for (std::vector < std::unique_ptr < SpaceTile >> ::iterator itr = gameSession->getSpaceTiles().begin(); itr != gameSession->getSpaceTiles().end(); itr++)
        {
            (*itr)->update(deltaTime, isSuddenDeath, gameSession->getPlayers(), gameSession->getBombs(), gameSession->getInsideBlocks(), gameSession->getBreakableBlocks(), gameSession->getPowerUps());
        }
    }
}