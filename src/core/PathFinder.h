//
//  PathFinder.h
//  bomberparty
//
//  Created by Stephen Gowen on 7/2/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__PathFinder__
#define __bomberparty__PathFinder__

struct Node
{
    int x, y;
};

#include "GameConstants.h"
#include <vector>

class InsideBlock;
class BombGameObject;
class Explosion;
class BreakableBlock;
class PlayerDynamicGameObject;

class PathFinder
{
public:
    static PathFinder& getInstance()
    {
        static PathFinder instance; // Guaranteed to be destroyed.
        return instance;
    }
    
    static bool isLocationOccupiedByInsideBlock(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, int gridX, int gridY);
    
    static bool isLocationOccupiedByBreakableBlock(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, int gridX, int gridY);
    
    static bool isLocationOccupiedByBombOrExplosionPath(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, int gridX, int gridY);
    
    // Calculate closest safe node, returning true if one was indeed calculated
    // Store the result in the node param
    static bool calculateClosestSafeNodeFromStartingNode(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, Node &node);
    
    static bool calculateClosestNodeToPlayerTarget(PlayerDynamicGameObject *player, Node &node);
    
    static bool shouldPlayerPlantBomb(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player);
    
    void initializeGameGrid(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    void freeGameGridCell(int gridX, int gridY);
    
    void occupyGameGridCell(int gridX, int gridY);
    
    int getGridCellCost(int x, int y);
    
    int getGridCellIndexForCoords(int x, int y);

private:
    int game_grid[NUM_GRID_CELLS_PER_ROW * GRID_CELL_NUM_ROWS];
    
    PathFinder() {}; // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    PathFinder(PathFinder const&);     // Don't Implement
    void operator=(PathFinder const&); // Don't implement
};

#endif /* defined(__bomberparty__PathFinder__) */
