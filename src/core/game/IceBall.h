//
//  IceBall.h
//  battlebombs
//
//  Created by Stephen Gowen on 10/3/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__IceBall__
#define __battlebombs__IceBall__

#include "DynamicGridGameObject.h"

#include <memory>
#include <vector>

class GameListener;
class InsideBlock;
class BreakableBlock;
class FallingObjectShadow;

class IceBall : public DynamicGridGameObject
{
public:
    IceBall(int gridX, int gridY, int index, GameListener *gameListener, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    void update(float deltaTime, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    float getStateTime();
    
    bool isVisible();
    
    bool isDescending();
    
    bool isTargetReached();
    
    FallingObjectShadow & getShadow();
    
    void handleTimeSinceSuddenDeathModeBegan(float timeSinceSuddenDeath);
    
private:
    GameListener *m_gameListener;
    std::unique_ptr<FallingObjectShadow> m_fallingObjectShadow;
    float m_fStateTime;
    float m_fTimeUntilAppearance;
    float m_fTargetY;
    bool m_isVisible;
    bool m_isDescending;
    bool m_isTargetReached;
};

#endif /* defined(__battlebombs__IceBall__) */
