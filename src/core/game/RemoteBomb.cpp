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
    m_state = RB_ACTIVATING;
}

void RemoteBomb::update(float deltaTime, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<RegeneratingDoor>> &doors, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    if(m_isExploding)
    {
        BombGameObject::update(deltaTime, explosions, mapBorders, insideBlocks, breakableBlocks, doors, players, bombs);
    }
    else
    {
        m_fStateTime += deltaTime;
        
        if(m_state == RB_ACTIVATING)
        {
            if(m_fStateTime > 0.3f)
            {
                m_state = RB_BROADCASTING;
                m_fStateTime = 0;
            }
        }
        
        updateForPush(deltaTime, mapBorders, insideBlocks, breakableBlocks, players, bombs);
    }
}

void RemoteBomb::detonate()
{
    m_fStateTime = 0;
    m_isExploding = true;
    
    m_bombOwner->onBombExploded();
}

Remote_Bomb_State RemoteBomb::getState()
{
    return m_state;
}