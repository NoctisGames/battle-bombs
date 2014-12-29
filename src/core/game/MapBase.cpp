//
//  MapBase.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "MapBase.h"
#include "ResourceConstants.h"
#include "GameSession.h"
#include "InsideBlock.h"
#include "RegeneratingDoor.h"
#include "BaseTile.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "PathFinder.h"
#include "PlayerDynamicGameObject.h"
#include "GameListener.h"
#include "GameEvent.h"

bool MapBase::updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player)
{
    if(player->isTrappedOnExplodingBaseTile(gameSession->getBaseTiles()))
    {
        gameSession->getGameListener()->addLocalEventForPlayer(PLAYER_HIT_BY_FIRE_BALL, *player);
        
        return true;
    }
    
    return false;
}

short MapBase::getMusicId()
{
    return MUSIC_PLAY_MAP_BASE;
}

void MapBase::handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan)
{
    for (std::vector < std::unique_ptr < BaseTile >> ::iterator itr = gameSession->getBaseTiles().begin(); itr != gameSession->getBaseTiles().end(); itr++)
    {
        (*itr)->handleTimeSinceSuddenDeathModeBegan(timeSinceSuddenDeathModeBegan);
    }
}

void MapBase::suddenDeath(GameSession *gameSession)
{
    int index = 0;
    
    int y = GRID_CELL_NUM_ROWS - 1;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 0;
    for(int x = 3; x < NUM_GRID_CELLS_PER_ROW - 3; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 2;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 1;
    for(int x = 3; x < NUM_GRID_CELLS_PER_ROW - 3; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 3;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 2;
    for(int x = 3; x < NUM_GRID_CELLS_PER_ROW - 3; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 4;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 3;
    for(int x = 0; x < NUM_GRID_CELLS_PER_ROW; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 5;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 4;
    for(int x = 0; x < NUM_GRID_CELLS_PER_ROW; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 6;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 5;
    for(int x = 0; x < NUM_GRID_CELLS_PER_ROW; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 7;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 6;
    for(int x = 0; x < NUM_GRID_CELLS_PER_ROW; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = GRID_CELL_NUM_ROWS - 8;
    for(int x = NUM_GRID_CELLS_PER_ROW - 1; x >= 0; x--)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 7;
    for(int x = 0; x < NUM_GRID_CELLS_PER_ROW; x++)
    {
        if(!PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), x, y))
        {
            gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(x, y, index++, gameSession->getGameListener())));
        }
    }
    
    y = 8;
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(1, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(14, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(5, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(13, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(9, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(12, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(10, y, index++, gameSession->getGameListener())));
    gameSession->getBaseTiles().push_back(std::unique_ptr<BaseTile>(new BaseTile(11, y, index++, gameSession->getGameListener())));
}

void MapBase::initializeMap(GameSession *gameSession)
{
    Map::initializeMap(gameSession);
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(7, 0)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(7, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 1)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 2)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 2)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 3)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 3)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(1, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(3, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(5, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(9, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(11, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 4)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 5)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(1, 5)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 5)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 5)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(1, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(5, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(13, 6)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(9, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 7)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(3, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(7, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 8)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 9)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 9)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 9)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(13, 9)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 10)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 10)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 10)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 10)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 11)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(5, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 12)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 13)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(9, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(13, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 14)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(3, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 15)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 16)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(5, 1)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(9, 1)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(3, 2)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(11, 2)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(2, 4)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(7, 4)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(3, 6)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(11, 7)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(1, 8)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(5, 8)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(11, 9)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(14, 9)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(7, 11)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(9, 11)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(1, 12)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(3, 12)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(11, 12)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(13, 12)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(1, 13)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(11, 14)));
    
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(5, 15)));
    gameSession->getRegeneratingDoors().push_back(std::unique_ptr<RegeneratingDoor>(new RegeneratingDoor(7, 15)));
}

void MapBase::initializeGameGrid(GameSession *gameSession, int (&gameGrid)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
{
    Map::initializeGameGrid(gameSession, gameGrid);
    
    for (std::vector < std::unique_ptr < RegeneratingDoor >> ::iterator itr = gameSession->getRegeneratingDoors().begin(); itr != gameSession->getRegeneratingDoors().end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        gameGrid[gridX][gridY] = 9;
    }
}

void MapBase::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    Map::update(gameSession, deltaTime, isSuddenDeath);
    
    for (std::vector < std::unique_ptr < RegeneratingDoor >> ::iterator itr = gameSession->getRegeneratingDoors().begin(); itr != gameSession->getRegeneratingDoors().end(); itr++)
    {
        (*itr)->update(deltaTime, gameSession->getBombs(), gameSession->getExplosions(), gameSession->getPlayers());
    }
    
    if(isSuddenDeath)
    {
        for (std::vector < std::unique_ptr < BaseTile >> ::iterator itr = gameSession->getBaseTiles().begin(); itr != gameSession->getBaseTiles().end(); itr++)
        {
            (*itr)->update(deltaTime, gameSession->getPlayers(), gameSession->getBombs(), gameSession->getBreakableBlocks(), gameSession->getRegeneratingDoors(), gameSession->getPowerUps());
        }
    }
}

bool MapBase::isValidLocationForBreakableBlock(GameSession *gameSession, int j, int i)
{
    if (PathFinder::isLocationOccupiedByRegeneratingDoor(gameSession->getRegeneratingDoors(), j, i))
    {
        return false;
    }
    
    return Map::isValidLocationForBreakableBlock(gameSession, j, i);
}