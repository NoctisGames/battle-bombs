//
//  Explosion.h
//  battlebombs
//
//  Created by Stephen Gowen on 4/30/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__Explosion__
#define __battlebombs__Explosion__

#include "GridGameObject.h"
#include "GameConstants.h"

#include <vector>
#include <memory>

class InsideBlock;
class BreakableBlock;
class PlayerDynamicGameObject;
class Fire;

class Explosion : public GridGameObject
{
public:
    Explosion(short power, int gridX, int gridY, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, float width = GRID_CELL_WIDTH, float height = GRID_CELL_HEIGHT);
    
    void update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    std::vector<std::unique_ptr<Fire >> & getFireParts();
    
    float getStateTime();
    
    short getPower();
    
    bool isComplete();
    
private:
    std::vector<std::unique_ptr<Fire >> m_fireParts;
    float m_fStateTime;
    float m_fTravelTime;
    float m_fExplosionTime;
    short m_sPower;
    short m_sPowerRemaining;
    short m_sFrames;
    bool m_travelingRight;
    bool m_travelingUp;
    bool m_travelingLeft;
    bool m_travelingDown;
    bool m_destroyedBlockRight;
    bool m_destroyedBlockUp;
    bool m_destroyedBlockLeft;
    bool m_destroyedBlockDown;
    int m_iRightGridX;
    int m_iUpGridY;
    int m_iLeftGridX;
    int m_iDownGridY;
    bool m_isComplete;
    
    void runBlockLogic(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    void runPostBlockLogic();
};

#endif /* defined(__battlebombs__Explosion__) */
