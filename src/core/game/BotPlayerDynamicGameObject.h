//
//  BotPlayerDynamicGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__BotPlayerDynamicGameObject__
#define __battlebombs__BotPlayerDynamicGameObject__

#include "PlayerDynamicGameObject.h"
#include "Node.h"

#include <vector>
#include <memory>

class MapSearchNode;
class RegeneratingDoor;

class BotPlayerDynamicGameObject : public PlayerDynamicGameObject
{
public:
    BotPlayerDynamicGameObject(short playerIndex, int gridX, int gridY, GameListener *gameListener, int direction = DIRECTION_RIGHT);

    virtual void update(float deltaTime, GameSession &gameSession);
    
    virtual bool isBot();
    
    PlayerDynamicGameObject * getTarget();
    
private:
	std::vector<MapSearchNode> m_currentPath;
	std::vector<Node> m_exploredPath;
	std::vector<Node> m_badBombEscapeNodes;
    int m_currentPathIndex;
    int m_currentPathType; // 0 for player target pursuit, 1 for evading bomb
    float m_fActionTime;
    float m_fWaitTime;
    PlayerDynamicGameObject *m_playerTarget;
    
    void determinePlayerTarget(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    bool calculatePathToTarget(int x, int y);
    
    void moveInDirection(int direction);
    
    void explore(GameSession &gameSession, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<RegeneratingDoor >> &doors, std::vector<std::unique_ptr<PowerUp >> &powerUps);
    
    bool isProposedNodeUnexplored(int x, int y);
};

#endif /* defined(__battlebombs__BotPlayerDynamicGameObject__) */
