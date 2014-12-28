//
//  BombGameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 4/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__BombGameObject__
#define __battlebombs__BombGameObject__

#include "DynamicGridGameObject.h"
#include "GameConstants.h"

#include <vector>
#include <memory>

class PlayerDynamicGameObject;
class Explosion;
class MapBorder;
class InsideBlock;
class BreakableBlock;

class BombGameObject : public DynamicGridGameObject
{
public:
    BombGameObject(PlayerDynamicGameObject *bombOwner, short power, int gridX, int gridY, float width = GRID_CELL_WIDTH * 8 / 5, float height = GRID_CELL_HEIGHT * 8 / 5);
    
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    float getStateTime();
    
    bool isExploding();
    
    bool isDestroyed();
    
    void destroy();

	void pushed(int direction);
    
    short getPower();

	virtual void updateBounds();

	virtual Rectangle & getBoundsForGridLogic();
    
protected:
    PlayerDynamicGameObject *m_bombOwner;
	std::unique_ptr<Rectangle> m_gridBounds;
    float m_fStateTime;
    float m_fSizeScalar;
    int m_iPushedDirection;
    short m_sPower;
    bool m_isExploding;
    bool m_isDestroyed;
	bool m_isPushed;
	bool m_isRebounding;
    
    void updateForPush(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);

	bool isCollision(std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
};

#endif /* defined(__battlebombs__BombGameObject__) */
