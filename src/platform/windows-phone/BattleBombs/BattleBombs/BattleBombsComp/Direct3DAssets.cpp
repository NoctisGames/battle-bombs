//
//  Direct3DAssets.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2013 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Direct3DAssets.h"
#include "GameSound.h"

Direct3DAssets * Direct3DAssets::getInstance()
{
	static Direct3DAssets *direct3DAssets = new Direct3DAssets();
	return direct3DAssets;
}

Direct3DAssets::Direct3DAssets()
{
	m_plantBombSound = std::unique_ptr<GameSound>(new GameSound("assets\\plant_bomb.wav"));
	m_explosionSound = std::unique_ptr<GameSound>(new GameSound("assets\\explosion.wav"));
	m_powerUpBombSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_bomb.wav"));
	m_powerUpFireSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_fire.wav"));
	m_powerUpSpeedSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_speed.wav"));
	m_powerUpForceFieldSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_force_field.wav"));
	m_powerUpPushSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_push.wav"));
	m_forceFieldDownSound = std::unique_ptr<GameSound>(new GameSound("assets\\force_field_down.wav"));
	m_deathSound = std::unique_ptr<GameSound>(new GameSound("assets\\death.wav"));
}