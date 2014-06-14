//
//  Renderer.cpp
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Renderer.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "GameConstants.h"

Renderer::Renderer()
{
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