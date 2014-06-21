//
//  BombGameObject.h
//  bomberparty
//
//  Created by Stephen Gowen on 4/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__BombGameObject__
#define __bomberparty__BombGameObject__

#include "pch.h"
#include "DynamicGridGameObject.h"
#include "GameConstants.h"

#include <vector>
#include <memory>

class PlayerDynamicGameObject;
class Explosion;
class InsideBlock;
class BreakableBlock;

class BombGameObject : public DynamicGridGameObject
{
public:
    BombGameObject(PlayerDynamicGameObject *bombOwner, short power, int gridX, int gridY, float width = GRID_CELL_WIDTH * 8 / 5, float height = GRID_CELL_HEIGHT * 8 / 5);
    
    void update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    float getStateTime();
    
    bool isExploding();
    
    bool isDestroyed();

	void pushed(int direction);

	void onPickedUp();
    
private:
    PlayerDynamicGameObject *m_bombOwner;
	int m_iPushedDirection;
    float m_fStateTime;
    float m_fSizeScalar;
	float m_fPushSpeed;
    short m_sPower;
    bool m_isExploding;
    bool m_isDestroyed;
	bool m_isPushed;
	bool m_isPickedUp;

	bool willHitBreakableBlock(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);

	bool willHitInsideBlock(std::vector<std::unique_ptr<InsideBlock >> &insideBlocks);

	bool willTravelOffGameField();

	bool canContinueMoving(std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks);
};

#endif /* defined(__bomberparty__BombGameObject__) */
