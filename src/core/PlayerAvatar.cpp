//
//  PlayerAvatar.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 7/14/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PlayerAvatar.h"
#include "Vector2D.h"
#include "Rectangle.h"

PlayerAvatar::PlayerAvatar(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_playerState = ALIVE;
    m_sPlayerIndex = 0;
    m_isBot = false;
}

void PlayerAvatar::setPlayerState(Player_State playerState)
{
    m_playerState = playerState;
}

Player_State PlayerAvatar::getPlayerState()
{
    return m_playerState;
}

void PlayerAvatar::setPlayerIndex(short playerIndex)
{
    m_sPlayerIndex = playerIndex;
}

short PlayerAvatar::getPlayerIndex()
{
    return m_sPlayerIndex;
}

void PlayerAvatar::setIsBot(bool isBot)
{
    m_isBot = isBot;
}

bool PlayerAvatar::isBot()
{
    return m_isBot;
}
