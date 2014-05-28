//
//  BotPlayerDynamicGameObject.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"

BotPlayerDynamicGameObject::BotPlayerDynamicGameObject(float x, float y, int direction, float width, float height) : PlayerDynamicGameObject(x, y, direction, width, height)
{
    // No further setup yet
}

void BotPlayerDynamicGameObject::update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PowerUp >> &powerUps)
{
    PlayerDynamicGameObject::update(deltaTime, insideBlocks, breakableBlocks, powerUps);

    // TODO
}