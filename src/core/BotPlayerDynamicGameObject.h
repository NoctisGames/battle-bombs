//
//  BotPlayerDynamicGameObject.h
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__BotPlayerDynamicGameObject__
#define __bomberparty__BotPlayerDynamicGameObject__

#include "PlayerDynamicGameObject.h"
#include "Node.h"

#include <vector>
#include <memory>

class MapSearchNode;

class BotPlayerDynamicGameObject : public PlayerDynamicGameObject
{
public:
    BotPlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction = DIRECTION_RIGHT);

    virtual void update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    virtual bool isBot();
    
private:
    std::vector<std::unique_ptr<MapSearchNode>> m_currentPath;
    std::vector<std::unique_ptr<Node>> m_exploredPath;
    int m_currentPathIndex;
    int m_currentPathType; // 0 for player target pursuit, 1 for evading bomb
    float m_fActionTime;
    float m_fWaitTime;
    PlayerDynamicGameObject *m_playerTarget;
    
    void determinePlayerTarget(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    bool calculatePathToTarget(int x, int y);
    
    void moveInDirection(int direction);
    
    void explore(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    bool isProposedNodeUnexplored(int x, int y);
};

#endif /* defined(__bomberparty__BotPlayerDynamicGameObject__) */
