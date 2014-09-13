//
//  PlayerRowPlatformAvatar.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PlayerRowPlatformAvatar.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameEvent.h"

PlayerRowPlatformAvatar::PlayerRowPlatformAvatar(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_iPlayerPlatform = PLATFORM_UNKNOWN;
}

void PlayerRowPlatformAvatar::setPlayerPlatform(int playerPlatform)
{
    m_iPlayerPlatform = playerPlatform;
}

int PlayerRowPlatformAvatar::getPlayerPlatform()
{
    return m_iPlayerPlatform;
}