//
//  PathFinder.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 7/2/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PathFinder.h"
#include "InsideBlock.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "BreakableBlock.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include <iostream>

bool PathFinder::isLocationOccupiedByInsideBlock(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, int gridX, int gridY)
{
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if (gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY())
        {
            return true;
        }
    }
    
    return false;
}

bool PathFinder::isLocationOccupiedByBreakableBlock(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int gridX, int gridY)
{
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if (gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY())
        {
            return true;
        }
    }
    
    return false;
}

bool PathFinder::isLocationOccupiedByBombOrExplosionPath(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, int gridX, int gridY)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        if(gridX == (*itr)->getGridX() && gridY >= (*itr)->getGridY() - (*itr)->getPower() && gridY <= (*itr)->getGridY() + (*itr)->getPower())
        {
            return true;
        }
        else if(gridY == (*itr)->getGridY() && gridX >= (*itr)->getGridX() - (*itr)->getPower() && gridX <= (*itr)->getGridX() + (*itr)->getPower())
        {
            return true;
        }
    }
    
    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
    {
        if(gridX == (*itr)->getGridX() && gridY >= (*itr)->getGridY() - (*itr)->getPower() && gridY <= (*itr)->getGridY() + (*itr)->getPower())
        {
            return true;
        }
        else if(gridY == (*itr)->getGridY() && gridX >= (*itr)->getGridX() - (*itr)->getPower() && gridX <= (*itr)->getGridX() + (*itr)->getPower())
        {
            return true;
        }
    }
    
    return false;
}

bool PathFinder::calculateClosestSafeNodeFromStartingNode(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, Node &node)
{
    int gridRightX = node.x;
    int gridLeftX = node.x;
    int gridTopY = node.y;
    int gridBottomY = node.y;
    
    bool proceedTop = true;
    bool proceedBottom = true;
    bool proceedLeft = true;
    bool proceedRight = true;
    
    int searchDepth = 0;
    
    while (true)
    {
        gridRightX++;
        gridLeftX--;
        gridTopY++;
        gridBottomY--;
        
        if(proceedTop && PathFinder::getInstance().getGridCellCost(node.x, gridTopY) == 9)
        {
            proceedTop = false;
        }
        
        if(proceedBottom && PathFinder::getInstance().getGridCellCost(node.x, gridBottomY) == 9)
        {
            proceedBottom = false;
        }
        
        if(proceedLeft && PathFinder::getInstance().getGridCellCost(gridLeftX, node.y) == 9)
        {
            proceedLeft = false;
        }
        
        if(proceedRight && PathFinder::getInstance().getGridCellCost(gridRightX, node.y) == 9)
        {
            proceedRight = false;
        }
        
        if(proceedTop && gridTopY < GRID_CELL_NUM_ROWS)
        {
            // tX is Traversal X
            for (int tX = gridLeftX; tX <= gridRightX; tX++)
            {
                if(PathFinder::getInstance().getGridCellCost(tX, gridTopY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, tX, gridTopY))
                {
                    node.x = tX;
                    node.y = gridTopY;
                    return true;
                }
            }
        }
        
        if(proceedBottom && gridBottomY >= 0)
        {
            // tX is Traversal X
            for (int tX = gridLeftX; tX <= gridRightX; tX++)
            {
                if(PathFinder::getInstance().getGridCellCost(tX, gridBottomY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, tX, gridBottomY))
                {
                    node.x = tX;
                    node.y = gridBottomY;
                    return true;
                }
            }
        }
        
        if(proceedLeft && gridLeftX >= 0)
        {
            // tY is Traversal Y
            for (int tY = gridBottomY; tY <= gridTopY; tY++)
            {
                if(PathFinder::getInstance().getGridCellCost(gridLeftX, tY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, gridLeftX, tY))
                {
                    node.x = gridLeftX;
                    node.y = tY;
                    return true;
                }
            }
        }
        
        if(proceedRight && gridRightX < NUM_GRID_CELLS_PER_ROW)
        {
            // tY is Traversal Y
            for (int tY = gridBottomY; tY <= gridTopY; tY++)
            {
                if(PathFinder::getInstance().getGridCellCost(gridRightX, tY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, gridRightX, tY))
                {
                    node.x = gridRightX;
                    node.y = tY;
                    return true;
                }
            }
        }
        
        searchDepth++;
        
        if(searchDepth > 4)
        {
            return false;
        }
    }
}

bool PathFinder::calculateClosestNodeToPlayerTarget(PlayerDynamicGameObject *player, Node &node)
{
    int gridRightX = node.x;
    int gridLeftX = node.x;
    int gridTopY = node.y;
    int gridBottomY = node.y;
    
    bool proceedTop = true;
    bool proceedBottom = true;
    bool proceedLeft = true;
    bool proceedRight = true;
    bool nodeUpdated = false;
    
    // Obnoxiously high number, so that the first distance we calculate is definitely shorter than this one
    float shortestDistance = 9000;
    Vector2D vectorTarget = player->getPosition();
    
    gridRightX++;
    gridLeftX--;
    gridTopY++;
    gridBottomY--;
    
    if(proceedTop && PathFinder::getInstance().getGridCellCost(node.x, gridTopY) == 9)
    {
        proceedTop = false;
    }
    
    if(proceedBottom && PathFinder::getInstance().getGridCellCost(node.x, gridBottomY) == 9)
    {
        proceedBottom = false;
    }
    
    if(proceedLeft && PathFinder::getInstance().getGridCellCost(gridLeftX, node.y) == 9)
    {
        proceedLeft = false;
    }
    
    if(proceedRight && PathFinder::getInstance().getGridCellCost(gridRightX, node.y) == 9)
    {
        proceedRight = false;
    }
    
    if(proceedTop && player->getGridY() >= node.y && gridTopY < GRID_CELL_NUM_ROWS)
    {
        // tX is Traversal X
        for (int tX = gridLeftX; tX <= gridRightX; tX++)
        {
            if(PathFinder::getInstance().getGridCellCost(tX, gridTopY) != 9)
            {
                Vector2D vector = Vector2D(tX, gridTopY);
                float distance = vector.dist(vectorTarget);
                if(distance < shortestDistance)
                {
                    shortestDistance = distance;
                    node.x = tX;
                    node.y = gridTopY;
                    nodeUpdated = true;
                }
            }
        }
    }
    
    if(proceedBottom && player->getGridY() <= node.y && gridBottomY >= 0)
    {
        // tX is Traversal X
        for (int tX = gridLeftX; tX <= gridRightX; tX++)
        {
            if(PathFinder::getInstance().getGridCellCost(tX, gridBottomY) != 9)
            {
                Vector2D vector = Vector2D(tX, gridBottomY);
                float distance = vector.dist(vectorTarget);
                if(distance < shortestDistance)
                {
                    shortestDistance = distance;
                    node.x = tX;
                    node.y = gridBottomY;
                    nodeUpdated = true;
                }
            }
        }
    }
    
    if(proceedLeft && player->getGridX() <= node.x && gridLeftX >= 0)
    {
        // tY is Traversal Y
        for (int tY = gridBottomY; tY <= gridTopY; tY++)
        {
            if(PathFinder::getInstance().getGridCellCost(gridLeftX, tY) != 9)
            {
                Vector2D vector = Vector2D(gridLeftX, tY);
                float distance = vector.dist(vectorTarget);
                if(distance < shortestDistance)
                {
                    shortestDistance = distance;
                    node.x = gridLeftX;
                    node.y = tY;
                    nodeUpdated = true;
                }
            }
        }
    }
    
    if(proceedRight && player->getGridX() >= node.x && gridRightX < NUM_GRID_CELLS_PER_ROW)
    {
        // tY is Traversal Y
        for (int tY = gridBottomY; tY <= gridTopY; tY++)
        {
            if(PathFinder::getInstance().getGridCellCost(gridRightX, tY) != 9)
            {
                Vector2D vector = Vector2D(gridRightX, tY);
                float distance = vector.dist(vectorTarget);
                if(distance < shortestDistance)
                {
                    shortestDistance = distance;
                    node.x = gridRightX;
                    node.y = tY;
                    nodeUpdated = true;
                }
            }
        }
    }
    
    return nodeUpdated;
}

bool PathFinder::shouldPlayerPlantBomb(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player)
{
    int pGridX = player->getGridX();
    int pGridY = player->getGridY();
    int gridRightX = pGridX + 1;
    int gridLeftX = pGridX - 1;
    int gridTopY = pGridY + 1;
    int gridBottomY = pGridY - 1;
    
    if(player->getDirection() == DIRECTION_RIGHT && isLocationOccupiedByBreakableBlock(breakableBlocks, gridRightX, pGridY))
    {
        return true;
    }
    else if(player->getDirection() == DIRECTION_LEFT && isLocationOccupiedByBreakableBlock(breakableBlocks, gridLeftX, pGridY))
    {
        return true;
    }
    else if(player->getDirection() == DIRECTION_UP && isLocationOccupiedByBreakableBlock(breakableBlocks, pGridX, gridTopY))
    {
        return true;
    }
    else if(player->getDirection() == DIRECTION_DOWN && isLocationOccupiedByBreakableBlock(breakableBlocks, pGridX, gridBottomY))
    {
        return true;
    }
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((*itr).get() != player && (*itr)->getPlayerState() == ALIVE)
        {
            if(pGridX == (*itr)->getGridX() && pGridY >= (*itr)->getGridY() - player->getFirePower() && pGridY <= (*itr)->getGridY() + player->getFirePower())
            {
                return true;
            }
            else if(pGridY == (*itr)->getGridY() && pGridX >= (*itr)->getGridX() - player->getFirePower() && pGridX <= (*itr)->getGridX() + player->getFirePower())
            {
                return true;
            }
        }
    }
    
    return false;
}

void PathFinder::initializeGameGrid(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            game_grid[getGridCellIndexForCoords(j, i)] = 1;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        game_grid[getGridCellIndexForCoords(gridX, gridY)] = 9;
    }
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        int gridX = (*itr)->getGridX();
        int gridY = (*itr)->getGridY();
        game_grid[getGridCellIndexForCoords(gridX, gridY)] = 9;
    }
    
    // For Map Borders
    
    game_grid[getGridCellIndexForCoords(0, 0)] = 9;
    game_grid[getGridCellIndexForCoords(1, 0)] = 9;
    game_grid[getGridCellIndexForCoords(2, 0)] = 9;
    game_grid[getGridCellIndexForCoords(0, 1)] = 9;
    game_grid[getGridCellIndexForCoords(1, 1)] = 9;
    game_grid[getGridCellIndexForCoords(2, 1)] = 9;
    game_grid[getGridCellIndexForCoords(0, 2)] = 9;
    game_grid[getGridCellIndexForCoords(1, 2)] = 9;
    game_grid[getGridCellIndexForCoords(2, 2)] = 9;
    
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 3, 0)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 2, 0)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 1, 0)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 3, 1)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 2, 1)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 1, 1)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 3, 2)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 2, 2)] = 9;
    game_grid[getGridCellIndexForCoords(NUM_GRID_CELLS_PER_ROW - 1, 2)] = 9;
    
    using namespace std;
    cout << "game_grid size : (" << sizeof(game_grid) / sizeof(int) << ")" << endl;
}

void PathFinder::freeGameGridCell(int gridX, int gridY)
{
    game_grid[getGridCellIndexForCoords(gridX, gridY)] = 1;
}

int PathFinder::getGridCellCost(int x, int y)
{
    if(x < 0 || x >= NUM_GRID_CELLS_PER_ROW || y < 0 || y >= GRID_CELL_NUM_ROWS)
	{
		return 9;
	}
    
	return game_grid[getGridCellIndexForCoords(x, y)];
}

int PathFinder::getGridCellIndexForCoords(int x, int y)
{
    return ((GRID_CELL_NUM_ROWS - y - 1) * NUM_GRID_CELLS_PER_ROW) + x;
}
