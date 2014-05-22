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

Renderer::Renderer()
{
    m_fScrollY = 0;
}

void Renderer::calcScrollYForPlayer(PlayerDynamicGameObject &player)
{
	if (player.getPosition().getY() > 7.9f)
	{
		m_fScrollY = player.getPosition().getY() - 7.9f;
		if (m_fScrollY > 12)
		{
			m_fScrollY = 12;
		}
	}
	else
	{
		m_fScrollY = 0;
	}
}