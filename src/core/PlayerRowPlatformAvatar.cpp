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

PlayerRowPlatformAvatar::PlayerRowPlatformAvatar(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    // No further setup required
}

void PlayerRowPlatformAvatar::setPlayerPlatform(int playerPlatform)
{
    m_iPlayerPlatform = playerPlatform;
}

int PlayerRowPlatformAvatar::getPlayerPlatform()
{
    return m_iPlayerPlatform;
}