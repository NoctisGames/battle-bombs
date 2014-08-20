//
//  Renderer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Renderer.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Font.h"
#include "ResourceConstants.h"

Renderer::Renderer()
{
    m_font = std::unique_ptr<Font>(new Font(FONT_TEXTURE_REGION_X, FONT_TEXTURE_REGION_Y, FONT_GLYPHS_PER_ROW, FONT_GLYPH_WIDTH, FONT_GLYPH_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    m_fScrollY = 0;
}

void Renderer::calcScrollYForPlayer(PlayerDynamicGameObject &player)
{
	if (player.getPosition().getY() > (SCREEN_HEIGHT / 2 + GRID_CELL_HEIGHT))
	{
		m_fScrollY = player.getPosition().getY() - (SCREEN_HEIGHT / 2 + GRID_CELL_HEIGHT);
		if (m_fScrollY > SCREEN_HEIGHT)
		{
			m_fScrollY = SCREEN_HEIGHT;
		}
	}
	else
	{
		m_fScrollY = 0;
	}
}