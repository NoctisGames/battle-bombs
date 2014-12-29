//
//  PathFinder.h
//  battlebombs
//
//  Created by Stephen Gowen on 7/2/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__PathFinder__
#define __battlebombs__PathFinder__

#include "GameConstants.h"
#include "Node.h"
#include <vector>
#include <memory>

class InsideBlock;
class BombGameObject;
class Explosion;
class BreakableBlock;
class PlayerDynamicGameObject;
class Map;
class GameSession;
class RegeneratingDoor;

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
    
    static bool isLocationOccupiedByDoor(std::vector<std::unique_ptr<RegeneratingDoor >> &doors, int gridX, int gridY);
    
    static bool isLocationOccupiedByRegeneratingDoor(std::vector<std::unique_ptr<RegeneratingDoor >> &doors, int gridX, int gridY);
    
    static bool isLocationOccupiedByOtherPlayer(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player, int gridX, int gridY);
    
    static bool isLocationOccupiedByPlayer(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, int gridX, int gridY);
    
    static bool isLocationOccupiedByBombOrExplosionPath(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, int gridX, int gridY, bool isCurrentlyTakingCover);
    
    // Calculate closest safe node, returning true if one was indeed calculated
    // Store the result in the node param
    static bool calculateClosestSafeNodeFromStartingNode(std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player, std::vector<Node> &badBombEscapeNodes, Node &node);
    
    static bool shouldPlayerPlantBomb(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, PlayerDynamicGameObject *player);
    
    int _gameGrid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS];
    
    void resetGameGrid();
    
    void initializeGameGrid(GameSession *gameSession, Map *map);
    
    void freeGameGridCell(int gridX, int gridY);
    
    void occupyGameGridCell(int gridX, int gridY);
    
    int getGridCellCost(int x, int y);

private:
	static bool hasBombEscapeNodeBeenUsedAlready(std::vector<Node> &badBombEscapeNodes, int gridX, int gridY);
    PathFinder() {}; // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    PathFinder(PathFinder const&);     // Don't Implement
    void operator=(PathFinder const&); // Don't implement
};

#endif /* defined(__battlebombs__PathFinder__) */
