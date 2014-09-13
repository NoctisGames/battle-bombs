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
	std::unique_ptr<GameSound> m_countDown3Sound;
	std::unique_ptr<GameSound> m_countDown2Sound;
	std::unique_ptr<GameSound> m_countDown1Sound;
	std::unique_ptr<GameSound> m_battleSound;
	std::unique_ptr<GameSound> m_plantBombSound;
	std::unique_ptr<GameSound> m_explosionSound;
	std::unique_ptr<GameSound> m_powerUpBombSound;
	std::unique_ptr<GameSound> m_powerUpFireSound;
	std::unique_ptr<GameSound> m_powerUpSpeedSound;
	std::unique_ptr<GameSound> m_powerUpForceFieldSound;
	std::unique_ptr<GameSound> m_powerUpPushSound;
	std::unique_ptr<GameSound> m_forceFieldDownSound;
	std::unique_ptr<GameSound> m_deathSound;
	std::unique_ptr<GameSound> m_gameSetSound;
	std::unique_ptr<GameSound> m_drawSound;

	static Direct3DAssets * getInstance();

	Direct3DAssets();
};