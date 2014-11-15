//
//  FireBall.h
//  battlebombs
//
//  Created by Stephen Gowen on 10/13/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__FireBall__
#define __battlebombs__FireBall__

#include "DynamicGridGameObject.h"

#include <memory>
#include <vector>

class GameListener;
class InsideBlock;
class BreakableBlock;
class FallingObjectShadow;

class FireBall : public DynamicGridGameObject
{
public:
    FireBall(int gridX, int gridY, int index, GameListener *gameListener, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    void update(float deltaTime, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    float getStateTime();
    
    bool isVisible();
    
    bool isDescending();
    
    bool isTargetReached();
    
    bool isExplosionCompleted();
    
    FallingObjectShadow & getShadow();
    
private:
    GameListener *m_gameListener;
    std::unique_ptr<FallingObjectShadow> m_fallingObjectShadow;
    float m_fStateTime;
    float m_fTimeUntilAppearance;
    float m_fTargetY;
    bool m_isVisible;
    bool m_isDescending;
    bool m_isTargetReached;
    bool m_isExplosionCompleted;
};

#endif /* defined(__battlebombs__FireBall__) */
