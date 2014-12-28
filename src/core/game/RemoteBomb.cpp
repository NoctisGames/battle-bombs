//
//  RemoteBomb.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "RemoteBomb.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "PlayerDynamicGameObject.h"
#include "Explosion.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "OverlapTester.h"
#include "Fire.h"

RemoteBomb::RemoteBomb(PlayerDynamicGameObject *bombOwner, short power, int gridX, int gridY) : BombGameObject(bombOwner, power, gridX, gridY)
{
    // Empty
}

void RemoteBomb::update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    if(m_isExploding)
    {
        BombGameObject::update(deltaTime, explosions, mapBorders, insideBlocks, breakableBlocks, players, bombs);
    }
    else
    {
        m_fStateTime += deltaTime;
        
        updateForPush(deltaTime, mapBorders, insideBlocks, breakableBlocks, players, bombs);
    }
}

void RemoteBomb::detonate()
{
    m_fStateTime = 0;
    m_isExploding = true;
    
    m_bombOwner->onBombExploded();
}