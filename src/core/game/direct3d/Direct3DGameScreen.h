//
//  Direct3DGameScreen.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#pragma once

#include "GameScreen.h"
#include "MediaEnginePlayer.h"
#include <memory>

class GameSound;

class Direct3DGameScreen : public GameScreen
{
public:
	Direct3DGameScreen(const char *username, bool isOffline = false);

	void load(float deviceScreenWidth, float deviceScreenHeight, int deviceScreenDpWidth, int deviceScreenDpHeight);

	void updateForRenderResolutionChange(float width, float height);

	void handleSound();

	void handleMusic();

	void unload();

	ID3D11Texture2D* getTexture();

	virtual void touchToWorld(TouchEvent &touchEvent);
    
    virtual void onResume();
    
    virtual void onPause();

	virtual bool handleOnBackPressed();

private:
	std::unique_ptr<MediaEnginePlayer> m_mediaPlayer;
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
	std::unique_ptr<GameSound> m_powerUpShieldSound;
	std::unique_ptr<GameSound> m_forceFieldDownSound;
	std::unique_ptr<GameSound> m_deathSound;
	std::unique_ptr<GameSound> m_hurrySound;
	std::unique_ptr<GameSound> m_gameSetSound;
	std::unique_ptr<GameSound> m_drawSound;
	std::unique_ptr<GameSound> m_raiseShieldSound;
	std::unique_ptr<GameSound> m_dislodgingSpaceTileSound;
	std::unique_ptr<GameSound> m_fallingSpaceTileSound;
	std::unique_ptr<GameSound> m_fallingObjectSound;
	std::unique_ptr<GameSound> m_crashingFireBallSound;
	std::unique_ptr<GameSound> m_crashingIceBallSound;

	float m_fGameScreenToDeviceScreenWidthRatio;
	float m_fGameScreenToDeviceScreenHeightRatio;
	float m_fDipToPixelRatio;
};