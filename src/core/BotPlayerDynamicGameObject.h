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

class BotPlayerDynamicGameObject : public PlayerDynamicGameObject
{
public:
    BotPlayerDynamicGameObject(short playerIndex, float x, float y, GameListener *gameListener, int direction = DIRECTION_RIGHT);

    virtual void update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs);
    
    virtual bool isBot();
    
private:
    PlayerDynamicGameObject *m_playerTarget;
    
    void determinePlayerTarget(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    bool isAbleToMoveDirection(short direction, bool &shouldPlantBomb, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
    
    bool moveInDirectionIfPossible(short direction, bool &shouldPlantBomb, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks);
};

#endif /* defined(__bomberparty__BotPlayerDynamicGameObject__) */
