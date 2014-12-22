//
//  Map.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Map.h"
#include "GameSession.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "SpaceTile.h"
#include "GameListener.h"
#include "BreakableBlock.h"
#include "BombGameObject.h"
#include "PathFinder.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"
#include "BotPlayerDynamicGameObject.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "Fire.h"

//For RNG purposes
#include <stdlib.h>
#include <time.h>

void Map::initializeMap(GameSession *gameSession)
{
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_TOP, SCREEN_WIDTH / 2, WORLD_HEIGHT - 1.45522388023028f, SCREEN_WIDTH, 2.91044776046056f)));
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_LEFT, 0.7388059701492f, 15.76119403157902f, 1.4776119402984f, 20.0597014947369f)));
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_RIGHT, 23.44029850746264f, 15.76119403157902f, 1.11940298507472f, 20.0597014947369f)));
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM_LEFT, 2.865671641791f, 3.58208955263162f, 5.731343283582f, 4.29850746315789f)));
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM_RIGHT, 21.31343283582084f, 3.58208955263162f, 5.37313432835832f, 4.29850746315789f)));
    gameSession->getMapBorders().push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM, SCREEN_WIDTH / 2, 0.71641791052634f, SCREEN_WIDTH, 1.43283582105267f)));
}

void Map::initializeGameGrid(GameSession *gameSession, int (&gameGrid)[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
{
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = gameSession->getInsideBlocks().begin(); itr != gameSession->getInsideBlocks().end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        gameGrid[gridX][gridY] = 9;
    }
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = gameSession->getBreakableBlocks().begin(); itr != gameSession->getBreakableBlocks().end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        gameGrid[gridX][gridY] = 9;
    }
    
    // For Map Borders
    
    gameGrid[0][0] = 9;
    gameGrid[1][0] = 9;
    gameGrid[2][0] = 9;
    gameGrid[0][1] = 9;
    gameGrid[1][1] = 9;
    gameGrid[2][1] = 9;
    gameGrid[0][2] = 9;
    gameGrid[1][2] = 9;
    gameGrid[2][2] = 9;
    
    gameGrid[NUM_GRID_CELLS_PER_ROW - 3][0] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 2][0] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 1][0] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 3][1] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 2][1] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 1][1] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 3][2] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 2][2] = 9;
    gameGrid[NUM_GRID_CELLS_PER_ROW - 1][2] = 9;
}

void Map::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = gameSession->getBombs().begin(); itr != gameSession->getBombs().end();)
    {
        (**itr).update(deltaTime, gameSession->getExplosions(), gameSession->getMapBorders(), gameSession->getInsideBlocks(), gameSession->getBreakableBlocks(), gameSession->getPlayers(), gameSession->getBombs());
        
        if ((**itr).isDestroyed())
        {
            itr = gameSession->getBombs().erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = gameSession->getInsideBlocks().begin(); itr != gameSession->getInsideBlocks().end(); )
    {
        (**itr).update(deltaTime);
        
        if((*itr)->getInsideBlockState() == IB_GONE)
        {
            itr = gameSession->getInsideBlocks().erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = gameSession->getBreakableBlocks().begin(); itr != gameSession->getBreakableBlocks().end(); )
    {
        (**itr).update(deltaTime);
        
        if ((**itr).getBreakableBlockState() == DESTROYED)
        {
            if ((**itr).hasPowerUp())
            {
                gameSession->getPowerUps().push_back(std::unique_ptr<PowerUp>(new PowerUp((**itr).getGridX(), (**itr).getGridY(), (**itr).getPowerUpFlag())));
            }
            
            if (!(**itr).wasHitByFireBall())
            {
                // Only free the cell for traversal if the breakable block wasn't destroyed by a fire ball
                // This is necessary because in this case, a crater is in its place
                PathFinder::getInstance().freeGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
                
                gameSession->onBreakableBlockDestroyed(**itr);
            }
            
            itr = gameSession->getBreakableBlocks().erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = gameSession->getExplosions().begin(); itr != gameSession->getExplosions().end();)
    {
        (**itr).update(deltaTime, gameSession->getInsideBlocks(), gameSession->getBreakableBlocks(), gameSession->getPlayers());
        
        if ((**itr).isComplete())
        {
            itr = gameSession->getExplosions().erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = gameSession->getPlayers().begin(); itr != gameSession->getPlayers().end(); itr++)
    {
        (**itr).update(deltaTime, gameSession->getMapBorders(), gameSession->getSpaceTiles(), gameSession->getInsideBlocks(), gameSession->getBreakableBlocks(), gameSession->getCraters(), gameSession->getPowerUps(), gameSession->getExplosions(), gameSession->getPlayers(), gameSession->getBombs());
    }
    
    for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = gameSession->getPowerUps().begin(); itr != gameSession->getPowerUps().end();)
    {
        (**itr).update(deltaTime);
        
        if ((**itr).isPickedUp())
        {
            gameSession->onPowerUpPickedUp(**itr);
            
            itr = gameSession->getPowerUps().erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

bool Map::isValidLocationForBreakableBlock(GameSession *gameSession, int j, int i)
{
    // Don't place breakable blocks where inside blocks or players are
    if (PathFinder::isLocationOccupiedByInsideBlock(gameSession->getInsideBlocks(), j, i) || isLocationOccupiedByPlayer(gameSession, j, i))
    {
        return false;
    }
    
    // Don't place breakable blocks where map borders are
    if (i <= 2 && (j <= 2 || j >= NUM_GRID_CELLS_PER_ROW - 3))
    {
        return false;
    }
    
    return true;
}

void Map::populateMapWithPlayersAndBreakableBlocks(GameSession *gameSession, int numHumanPlayers)
{
    srand((int) time(NULL));
    
    int playerStartingPositions[8][2] =
    {
        { PLAYER_1_GRID_X, PLAYER_1_GRID_Y },
        { PLAYER_2_GRID_X, PLAYER_2_GRID_Y },
        { PLAYER_3_GRID_X, PLAYER_3_GRID_Y },
        { PLAYER_4_GRID_X, PLAYER_4_GRID_Y },
        { PLAYER_5_GRID_X, PLAYER_5_GRID_Y },
        { PLAYER_6_GRID_X, PLAYER_6_GRID_Y },
        { PLAYER_7_GRID_X, PLAYER_7_GRID_Y },
        { PLAYER_8_GRID_X, PLAYER_8_GRID_Y }
    };
    
    int n = sizeof (playerStartingPositions) / sizeof (playerStartingPositions[0]);
    randomize(playerStartingPositions, n);
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 1 ?
                                                                                 new PlayerDynamicGameObject(0, playerStartingPositions[0][0], playerStartingPositions[0][1], gameSession->getGameListener(), DIRECTION_RIGHT) :
                                                                                 new BotPlayerDynamicGameObject(0, playerStartingPositions[0][0], playerStartingPositions[0][1], gameSession->getGameListener(), DIRECTION_RIGHT)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 2 ?
                                                                                 new PlayerDynamicGameObject(1, playerStartingPositions[1][0], playerStartingPositions[1][1], gameSession->getGameListener(), DIRECTION_LEFT) :
                                                                                 new BotPlayerDynamicGameObject(1, playerStartingPositions[1][0], playerStartingPositions[1][1], gameSession->getGameListener(), DIRECTION_LEFT)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 3 ?
                                                                                 new PlayerDynamicGameObject(2, playerStartingPositions[2][0], playerStartingPositions[2][1], gameSession->getGameListener(), DIRECTION_RIGHT) :
                                                                                 new BotPlayerDynamicGameObject(2, playerStartingPositions[2][0], playerStartingPositions[2][1], gameSession->getGameListener(), DIRECTION_RIGHT)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 4 ?
                                                                                 new PlayerDynamicGameObject(3, playerStartingPositions[3][0], playerStartingPositions[3][1], gameSession->getGameListener(), DIRECTION_LEFT) :
                                                                                 new BotPlayerDynamicGameObject(3, playerStartingPositions[3][0], playerStartingPositions[3][1], gameSession->getGameListener(), DIRECTION_LEFT)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 5 ?
                                                                                 new PlayerDynamicGameObject(4, playerStartingPositions[4][0], playerStartingPositions[4][1], gameSession->getGameListener(), DIRECTION_UP) :
                                                                                 new BotPlayerDynamicGameObject(4, playerStartingPositions[4][0], playerStartingPositions[4][1], gameSession->getGameListener(), DIRECTION_UP)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 6 ?
                                                                                 new PlayerDynamicGameObject(5, playerStartingPositions[5][0], playerStartingPositions[5][1], gameSession->getGameListener(), DIRECTION_DOWN) :
                                                                                 new BotPlayerDynamicGameObject(5, playerStartingPositions[5][0], playerStartingPositions[5][1], gameSession->getGameListener(), DIRECTION_DOWN)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 7 ?
                                                                                 new PlayerDynamicGameObject(6, playerStartingPositions[6][0], playerStartingPositions[6][1], gameSession->getGameListener(), DIRECTION_UP) :
                                                                                 new BotPlayerDynamicGameObject(6, playerStartingPositions[6][0], playerStartingPositions[6][1], gameSession->getGameListener(), DIRECTION_UP)));
    
    gameSession->getPlayers().push_back(std::unique_ptr<PlayerDynamicGameObject>(numHumanPlayers >= 8 ?
                                                                                 new PlayerDynamicGameObject(7, playerStartingPositions[7][0], playerStartingPositions[7][1], gameSession->getGameListener(), DIRECTION_DOWN) :
                                                                                 new BotPlayerDynamicGameObject(7, playerStartingPositions[7][0], playerStartingPositions[7][1], gameSession->getGameListener(), DIRECTION_DOWN)));
    
    int numBreakableBlocksAtSpawnTime = 0;
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            if(isValidLocationForBreakableBlock(gameSession, j, i))
            {
                // 80% chance there will be a breakable block at all
                if ((rand() % 100 + 1) < 81)
                {
                    int flag = POWER_UP_TYPE_NONE;
                    
                    // Generate a random number from 1 - 100
                    // This will be used to determine which type of powerups will appear
                    int flagRange = (rand() % 100 + 1);
                    
                    if (flagRange <= 55)
                    {
                        flag = rand() % POWER_UP_TYPE_SPEED + 1;
                    }
                    else if (flagRange > 55 && flagRange <= 60)
                    {
                        flag = POWER_UP_TYPE_FORCE_FIELD;
                    }
                    else if (flagRange > 60 && flagRange <= 65)
                    {
                        flag = POWER_UP_TYPE_PUSH;
                    }
                    else if (flagRange > 65 && flagRange <= 70)
                    {
                        flag = POWER_UP_TYPE_SHIELD;
                    }
                    
                    // If these two blocks don't get caught, the flag remains 0 and no powerup will be created
                    gameSession->getBreakableBlocks().push_back(std::unique_ptr<BreakableBlock>(new BreakableBlock(j, i, flag)));
                    
                    numBreakableBlocksAtSpawnTime++;
                }
            }
        }
    }
    
    gameSession->setNumBreakableBlocksAtSpawnTime(numBreakableBlocksAtSpawnTime);
    
    PathFinder::getInstance().resetGameGrid();
    PathFinder::getInstance().initializeGameGrid(gameSession, this);
}

#pragma mark <Protected>

void Map::addCommonInsideBlocks(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks)
{
    // BEGIN BOTTOM CENTER
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 5, 0)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 7, 0)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 9, 0)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 4, 2)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 6, 2)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 8, 2)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 2)));
    
    // BEGIN BOTTOM
    for (int i = 4; i < BOTTOM_HALF_TOP_GRID_Y; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
    
    // BEGIN MIDDLE
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, BOTTOM_HALF_TOP_GRID_Y + 1)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, TOP_HALF_BOTTOM_GRID_Y - 1)));
    
    for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, BOTTOM_HALF_TOP_GRID_Y + 1)));
        insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, TOP_HALF_BOTTOM_GRID_Y - 1)));
    }
    
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, BOTTOM_HALF_TOP_GRID_Y + 1)));
    insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, TOP_HALF_BOTTOM_GRID_Y - 1)));
}

#pragma mark <Private>

bool Map::isLocationOccupiedByPlayer(GameSession *gameSession, int gridX, int gridY)
{
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = gameSession->getPlayers().begin(); itr != gameSession->getPlayers().end(); itr++)
    {
        if ((gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY()) || (gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY() + 1) || (gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY() - 1) || (gridX == (*itr)->getGridX() + 1 && gridY == (*itr)->getGridY()) || (gridX == (*itr)->getGridX() - 1 && gridY == (*itr)->getGridY()))
        {
            return true;
        }
    }
    
    return false;
}

void Map::swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void Map::randomize(int arr[][2], int n)
{
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n - 1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);
        
        // Swap arr[i] with the element at random index
        swap(&arr[i][0], &arr[j][0]);
        swap(&arr[i][1], &arr[j][1]);
    }
}