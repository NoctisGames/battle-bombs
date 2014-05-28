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
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "GameListener.h"
#include "GameEvent.h"

// For Random Event Generation
#include <stdlib.h>
#include <time.h>

BotPlayerDynamicGameObject::BotPlayerDynamicGameObject(short playerIndex, float x, float y, GameListener *gameListener, int direction, float width, float height) : PlayerDynamicGameObject(playerIndex, x, y, gameListener, direction, true, width, height)
{
    srand(time(NULL));
}

void BotPlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps)
{
    PlayerDynamicGameObject::update(deltaTime, insideBlocks, breakableBlocks, powerUps);

    if (m_fStateTime > 1)
    {
        m_fStateTime = 0;

        short randomEvent = rand() % 5 + 1;

        m_gameListener->addLocalEvent(m_sPlayerIndex * PLAYER_EVENT_BASE + randomEvent);
    }
}