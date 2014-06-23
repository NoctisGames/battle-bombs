//
//  BotPlayerDynamicGameObject.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "BotPlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "Explosion.h"
#include "GameListener.h"
#include "GameEvent.h"

// For Random Event Generation
#include <stdlib.h>
#include <time.h>

BotPlayerDynamicGameObject::BotPlayerDynamicGameObject(short playerIndex, float x, float y, GameListener *gameListener, int direction) : PlayerDynamicGameObject(playerIndex, x, y, gameListener, direction)
{
    srand((int)time(NULL));
}

void BotPlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<MapBorder >> &mapBorders, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps, std::vector<std::unique_ptr<Explosion >> &explosions, std::vector<std::unique_ptr<BombGameObject >> &bombs)
{
    PlayerDynamicGameObject::update(deltaTime, mapBorders, insideBlocks, breakableBlocks, powerUps, explosions, bombs);

    // BEGIN TEMPORARY AI
    if (m_playerState == ALIVE && m_fStateTime > 1)
    {
        m_fStateTime = 0;

        short randomEvent = rand() % 5 + 1;

        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + randomEvent);
        
        bool shouldPlantBomb = rand() % 10 > 8;
        if(shouldPlantBomb)
        {
            if(isAbleToDropAdditionalBomb())
            {
                m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_PLANT_BOMB);
            }
        }
    }
    // END TEMPORARY AI

    if (isHitByExplosion(explosions, bombs))
    {
        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + PLAYER_DEATH);
    }
}

bool BotPlayerDynamicGameObject::isBot()
{
    return true;
}