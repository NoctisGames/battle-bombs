//
//  PathFinder.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 7/2/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "PathFinder.h"
#include "InsideBlock.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "BreakableBlock.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "GameEvent.h"
#include "Map.h"
#include "GameSession.h"

// For Randomness
#include <stdlib.h>
#include <time.h>

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

bool PathFinder::isLocationOccupiedByOtherPlayer(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player, int gridX, int gridY)
{
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if ((*itr).get() != player && gridX == (*itr)->getGridX() && gridY == (*itr)->getGridY())
        {
            return true;
        }
    }
    
    return false;
}

bool PathFinder::isLocationOccupiedByBombOrExplosionPath(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, int gridX, int gridY, bool isCurrentlyTakingCover)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        if(gridX == (*itr)->getGridX() && gridY >= (*itr)->getGridY() - (*itr)->getPower() && gridY <= (*itr)->getGridY() + (*itr)->getPower())
        {
            if(isCurrentlyTakingCover)
            {
                return (*itr)->getStateTime() > 1.8f;
            }
            else
            {
                return true;
            }
        }
        else if(gridY == (*itr)->getGridY() && gridX >= (*itr)->getGridX() - (*itr)->getPower() && gridX <= (*itr)->getGridX() + (*itr)->getPower())
        {
            if(isCurrentlyTakingCover)
            {
                return (*itr)->getStateTime() > 1.8f;
            }
            else
            {
                return true;
            }
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

bool PathFinder::calculateClosestSafeNodeFromStartingNode(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player, std::vector<Node> &badBombEscapeNodes, Node &node)
{
    srand((int)time(NULL));
    
    int gridRightX = node.x;
    int gridLeftX = node.x;
    int gridTopY = node.y;
    int gridBottomY = node.y;
    
    bool proceedTop = true;
    bool proceedBottom = true;
    bool proceedLeft = true;
    bool proceedRight = true;
    
    // Obnoxiously high number, so that the first distance we calculate is definitely shorter than this one
    float shortestDistanceSquared = 9000;
    Vector2D vectorTarget = Vector2D(player->getGridX(), player->getGridY());
    bool nodeUpdated = false;
    
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
        
        if(proceedTop)
        {
            // tX is Traversal X
            for (int tX = gridLeftX; tX <= gridRightX; tX++)
            {
                if(PathFinder::getInstance().getGridCellCost(tX, gridTopY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, tX, gridTopY, false))
                {
                    if(!hasBombEscapeNodeBeenUsedAlready(badBombEscapeNodes, tX, gridTopY))
                    {
                        Vector2D vector = Vector2D(tX, gridTopY);
                        float distance = vector.distSquared(vectorTarget);
                        if(distance < shortestDistanceSquared || (distance == shortestDistanceSquared && rand() % 2 == 1))
                        {
                            shortestDistanceSquared = distance;
                            node.x = tX;
                            node.y = gridTopY;
                            nodeUpdated = true;
                        }
                    }
                }
            }
        }
        
        if(proceedBottom)
        {
            // tX is Traversal X
            for (int tX = gridLeftX; tX <= gridRightX; tX++)
            {
                if(PathFinder::getInstance().getGridCellCost(tX, gridBottomY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, tX, gridBottomY, false))
                {
                    if(!hasBombEscapeNodeBeenUsedAlready(badBombEscapeNodes, tX, gridBottomY))
                    {
                        Vector2D vector = Vector2D(tX, gridBottomY);
                        float distance = vector.distSquared(vectorTarget);
                        if(distance < shortestDistanceSquared || (distance == shortestDistanceSquared && rand() % 2 == 1))
                        {
                            shortestDistanceSquared = distance;
                            node.x = tX;
                            node.y = gridBottomY;
                            nodeUpdated = true;
                        }
                    }
                }
            }
        }
        
        if(proceedLeft)
        {
            // tY is Traversal Y
            for (int tY = gridBottomY; tY <= gridTopY; tY++)
            {
                if(PathFinder::getInstance().getGridCellCost(gridLeftX, tY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, gridLeftX, tY, false))
                {
                    if(!hasBombEscapeNodeBeenUsedAlready(badBombEscapeNodes, gridLeftX, tY))
                    {
                        Vector2D vector = Vector2D(gridLeftX, tY);
                        float distance = vector.distSquared(vectorTarget);
                        if(distance < shortestDistanceSquared || (distance == shortestDistanceSquared && rand() % 2 == 1))
                        {
                            shortestDistanceSquared = distance;
                            node.x = gridLeftX;
                            node.y = tY;
                            nodeUpdated = true;
                        }
                    }
                }
            }
        }
        
        if(proceedRight)
        {
            // tY is Traversal Y
            for (int tY = gridBottomY; tY <= gridTopY; tY++)
            {
                if(PathFinder::getInstance().getGridCellCost(gridRightX, tY) != 9 && !isLocationOccupiedByBombOrExplosionPath(bombs, explosions, gridRightX, tY, false))
                {
                    if(!hasBombEscapeNodeBeenUsedAlready(badBombEscapeNodes, gridRightX, tY))
                    {
                        Vector2D vector = Vector2D(gridRightX, tY);
                        float distance = vector.distSquared(vectorTarget);
                        if(distance < shortestDistanceSquared || (distance == shortestDistanceSquared && rand() % 2 == 1))
                        {
                            shortestDistanceSquared = distance;
                            node.x = gridRightX;
                            node.y = tY;
                            nodeUpdated = true;
                        }
                    }
                }
            }
        }
        
        searchDepth++;
        
        if(searchDepth > 4)
        {
            return nodeUpdated;
        }
    }
}

bool calculateClosestNodeToPlayerTarget(PlayerDynamicGameObject *player, Node &node)
{
    int gridRightX = node.x;
    int gridLeftX = node.x;
    int gridTopY = node.y;
    int gridBottomY = node.y;
    
    bool nodeUpdated = false;
    
    // Obnoxiously high number, so that the first distance we calculate is definitely shorter than this one
    float shortestDistanceSquared = 9000;
    Vector2D vectorTarget = Vector2D(player->getGridX(), player->getGridY());
    
    int searchDepth = 0;
    
    while(true)
    {
        gridRightX++;
        gridLeftX--;
        gridTopY++;
        gridBottomY--;
        
        // tX is Traversal X
        for (int tX = gridLeftX; tX <= gridRightX; tX++)
        {
            if(PathFinder::getInstance().getGridCellCost(tX, gridTopY))
            {
                Vector2D vector = Vector2D(node.x, gridTopY);
                float distance = vector.distSquared(vectorTarget);
                if(distance < shortestDistanceSquared)
                {
                    shortestDistanceSquared = distance;
                    node.x = tX;
                    node.y = gridTopY;
                    nodeUpdated = true;
                }
            }
        }
        
        // tX is Traversal X
        for (int tX = gridLeftX; tX <= gridRightX; tX++)
        {
            if(PathFinder::getInstance().getGridCellCost(tX, gridBottomY))
            {
                Vector2D vector = Vector2D(node.x, gridBottomY);
                float distance = vector.distSquared(vectorTarget);
                if(distance < shortestDistanceSquared)
                {
                    shortestDistanceSquared = distance;
                    node.x = tX;
                    node.y = gridBottomY;
                    nodeUpdated = true;
                }
            }
        }
        
        // tY is Traversal Y
        for (int tY = gridBottomY; tY <= gridTopY; tY++)
        {
            if(PathFinder::getInstance().getGridCellCost(gridLeftX, tY))
            {
                Vector2D vector = Vector2D(node.x, gridTopY);
                float distance = vector.distSquared(vectorTarget);
                if(distance < shortestDistanceSquared)
                {
                    shortestDistanceSquared = distance;
                    node.x = gridLeftX;
                    node.y = tY;
                    nodeUpdated = true;
                }
            }
        }
        
        // tY is Traversal Y
        for (int tY = gridBottomY; tY <= gridTopY; tY++)
        {
            if(PathFinder::getInstance().getGridCellCost(gridRightX, tY) != 9)
            {
                Vector2D vector = Vector2D(node.x, gridTopY);
                float distance = vector.distSquared(vectorTarget);
                if(distance < shortestDistanceSquared)
                {
                    shortestDistanceSquared = distance;
                    node.x = gridRightX;
                    node.y = tY;
                    nodeUpdated = true;
                }
            }
        }
        
        searchDepth++;
        
        if(searchDepth > 8)
        {
            break;
        }
    }
    
    return nodeUpdated;
}

bool PathFinder::shouldPlayerPlantBomb(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player)
{
    int pGridX = player->getGridX();
    int pGridY = player->getGridY();
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((*itr).get() != player && (*itr)->getPlayerState() == ALIVE)
        {
            if(pGridX == (*itr)->getGridX() && pGridY >= (*itr)->getGridY() - player->getFirePower() && pGridY <= (*itr)->getGridY() + player->getFirePower())
            {
                if(pGridY > (*itr)->getGridY())
                {
                    for(int i = (*itr)->getGridY(); i < pGridY; i++)
                    {
                        if(PathFinder::getInstance().getGridCellCost(pGridX, i) == 9 && !PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, pGridX, i))
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    for(int i = pGridY; i < (*itr)->getGridY(); i++)
                    {
                        if(PathFinder::getInstance().getGridCellCost(pGridX, i) == 9 && !PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, pGridX, i))
                        {
                            return false;
                        }
                    }
                }
                
                return true;
            }
            else if(pGridY == (*itr)->getGridY() && pGridX >= (*itr)->getGridX() - player->getFirePower() && pGridX <= (*itr)->getGridX() + player->getFirePower())
            {
                if(pGridX > (*itr)->getGridX())
                {
                    for(int i = (*itr)->getGridX(); i < pGridX; i++)
                    {
                        if(PathFinder::getInstance().getGridCellCost(i, pGridY) == 9 && !PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, i, pGridY))
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    for(int i = pGridX; i < (*itr)->getGridX(); i++)
                    {
                        if(PathFinder::getInstance().getGridCellCost(i, pGridY) == 9 && !PathFinder::isLocationOccupiedByBreakableBlock(breakableBlocks, i, pGridY))
                        {
                            return false;
                        }
                    }
                }
                
                return true;
            }
        }
    }
    
    return false;
}

void PathFinder::resetGameGrid()
{
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            _gameGrid[j][i] = 1;
        }
    }
}

void PathFinder::initializeGameGrid(GameSession *gameSession, Map *map)
{
    map->initializeGameGrid(gameSession, _gameGrid);
}

void PathFinder::freeGameGridCell(int gridX, int gridY)
{
    _gameGrid[gridX][gridY] = 1;
}

void PathFinder::occupyGameGridCell(int gridX, int gridY)
{
    _gameGrid[gridX][gridY] = 9;
}

int PathFinder::getGridCellCost(int x, int y)
{
    if(x < 0 || x >= NUM_GRID_CELLS_PER_ROW || y < 0 || y >= GRID_CELL_NUM_ROWS)
	{
		return 9;
	}
    
	return _gameGrid[x][y];
}

bool PathFinder::hasBombEscapeNodeBeenUsedAlready(std::vector<Node> &badBombEscapeNodes, int gridX, int gridY)
{
	for (std::vector<Node>::iterator itr = badBombEscapeNodes.begin(); itr != badBombEscapeNodes.end(); itr++)
    {
        if((*itr).x == gridX && (*itr).y == gridY)
        {
            return true;
        }
    }
    
    return false;
}
