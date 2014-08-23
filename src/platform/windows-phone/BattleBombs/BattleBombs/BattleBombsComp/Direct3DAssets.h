//
//  Direct3DAssets.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#pragma once

#include <memory>

class GameSound;

class Direct3DAssets
{
public:
	std::unique_ptr<GameSound> m_plantBombSound;
	std::unique_ptr<GameSound> m_explosionSound;
	std::unique_ptr<GameSound> m_deathSound;

	static Direct3DAssets * getInstance();

	Direct3DAssets();
};