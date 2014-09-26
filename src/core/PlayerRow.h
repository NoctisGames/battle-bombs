//
//  PlayerRow.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/9/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__PlayerRow__
#define __battlebombs__PlayerRow__

#include "GameObject.h"
#include "rapidjson/document.h"

#include <memory>

class PlayerRowAvatar;
class PlayerRowPlatformAvatar;

class PlayerRow
{
public:
	PlayerRow(float x, float y, float width, float height, int playerIndex);
    
    void handlePlayerNameAndPlatform(rapidjson::Document &d, const char *keyName, const char *keyPlatform, int playerIndex);
    
    void reset();
    
    char * getPlayerName();
    
    PlayerRowAvatar & getPlayerRowAvatar();
    
    PlayerRowPlatformAvatar & getPlayerPlatformAvatar();
    
    float getFontX();
    
    float getFontY();
    
    float getFontGlyphWidth();
    
    float getFontGlyphHeight();
    
    bool isActive();
    
private:
    std::unique_ptr<char> m_playerName;
    std::unique_ptr<PlayerRowAvatar> m_playerRowAvatar;
    std::unique_ptr<PlayerRowPlatformAvatar> m_playerRowPlatformAvatar;
    float m_fFontX;
    float m_fFontY;
    float m_fFontGlyphWidth;
    float m_fFontGlyphHeight;
    bool m_isActive;
};

#endif /* defined(__battlebombs__PlayerRow__) */
