//
//  FallingObjectShadow.h
//  battlebombs
//
//  Created by Stephen Gowen on 10/4/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__FallingObjectShadow__
#define __battlebombs__FallingObjectShadow__

#include "GridGameObject.h"
#include "FallingObjectShadowState.h"

#include <memory>
#include <vector>

class InsideBlock;
class BreakableBlock;

class FallingObjectShadow : public GridGameObject
{
public:
    FallingObjectShadow(int gridX, int gridY, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    void update(float deltaTime, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    float getStateTime();
    
    Falling_Object_Shadow_State getFallingObjectShadowState();
    
    InsideBlock * getTargetInsideBlock();
    
    BreakableBlock * getTargetBreakableBlock();
    
    bool isTargetOccupiedByInsideBlock();
    
    bool isTargetOccupiedByBreakableBlock();
    
    void handleTimeSinceSuddenDeathModeBegan(float timeSinceSuddenDeath);
    
private:
    float m_fStateTime;
    Falling_Object_Shadow_State m_fallingObjectShadowState;
    InsideBlock *m_targetInsideBlock;
    BreakableBlock *m_targetBreakableBlock;
    bool m_isTargetInsideBlock;
    bool m_isTargetBreakableBlock;
    
    bool isTargetInsideBlock(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks);
    
    bool isTargetBreakableBlock(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
};

#endif /* defined(__battlebombs__FallingObjectShadow__) */
