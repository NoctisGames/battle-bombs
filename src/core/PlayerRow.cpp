//
//  PlayerRow.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/9/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameEvent.h"

PlayerRow::PlayerRow(float x, float y, float width, float height)
{
    m_fFontX = x + height * 3 / 4;
    m_fFontY = y;
    m_fFontGlyphWidth = (width - height) / 12;
    m_fFontGlyphHeight = m_fFontGlyphWidth * 0.68421052631579;
    m_playerRowPlatformAvatar = std::unique_ptr<PlayerRowPlatformAvatar>(new PlayerRowPlatformAvatar(x - width / 2 + height / 2, y, height, height));
    m_isActive = false;
}

void PlayerRow::handlePlayerNameAndPlatform(rapidjson::Document &d, const char *keyName, const char *keyPlatform, int playerIndex)
{
    if(d.HasMember(keyName) && d.HasMember(keyPlatform))
    {
        const char *username = d[keyName].GetString();
        int usernameLength = (int) strlen(username);

		m_playerName.release();
        
        m_playerName = std::unique_ptr<char>(new char[usernameLength]);
        
        std::strncpy(m_playerName.get(), username, usernameLength);
		m_playerName.get()[usernameLength] = '\0';
        
        m_playerRowPlatformAvatar->setPlayerPlatform(d[keyPlatform].GetInt());
        
        m_isActive = true;
    }
}

void PlayerRow::reset()
{
    m_playerRowPlatformAvatar->setPlayerPlatform(PLATFORM_UNKNOWN);
    
    m_isActive = false;
}

char * PlayerRow::getPlayerName()
{
    return m_playerName.get();
}

PlayerRowPlatformAvatar & PlayerRow::getPlayerPlatformAvatar()
{
    return *m_playerRowPlatformAvatar;
}

float PlayerRow::getFontX()
{
    return m_fFontX;
}

float PlayerRow::getFontY()
{
    return m_fFontY;
}

float PlayerRow::getFontGlyphWidth()
{
    return m_fFontGlyphWidth;
}

float PlayerRow::getFontGlyphHeight()
{
    return m_fFontGlyphHeight;
}

bool PlayerRow::isActive()
{
    return m_isActive;
}
