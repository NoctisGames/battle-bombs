//
//  PlayerRowAvatar.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/10/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PlayerRowAvatar.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameEvent.h"

PlayerRowAvatar::PlayerRowAvatar(float x, float y, float width, float height, int playerIndex) : GameObject(x, y, width, height, 0)
{
    m_iPlayerIndex = playerIndex;
    m_isBot = true;
}

void PlayerRowAvatar::setPlayerIndex(int playerIndex)
{
    m_iPlayerIndex = playerIndex;
}

int PlayerRowAvatar::getPlayerIndex()
{
    return m_iPlayerIndex;
}

void PlayerRowAvatar::setIsBot(bool isBot)
{
    m_isBot = isBot;
}

bool PlayerRowAvatar::isBot()
{
    return m_isBot;
}