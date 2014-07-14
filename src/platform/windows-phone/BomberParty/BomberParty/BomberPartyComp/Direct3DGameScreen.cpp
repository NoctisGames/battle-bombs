//
//  Direct3DGameScreen.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Direct3DGameScreen.h"
#include "RECTUtils.h"
#include "TouchEvent.h"
#include "Global.h"
#include "Direct3DAssets.h"
#include "Direct3DRenderer.h"
#include "GameSound.h"
#include "Assets.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Font.h"

Direct3DGameScreen::Direct3DGameScreen(const char *username, int deviceScreenWidth, int deviceScreenHeight) : GameScreen(username)
{
	m_iDeviceScreenWidth = deviceScreenWidth;
	m_iDeviceScreenHeight = deviceScreenHeight;
	m_fGameScreenToDeviceScreenWidthRatio = deviceScreenWidth / SCREEN_WIDTH;
	m_fGameScreenToDeviceScreenHeightRatio = deviceScreenHeight / SCREEN_HEIGHT;
	m_fDipToPixelRatio = deviceScreenHeight == 768 ? 1.6f : deviceScreenHeight == 720 ? 1.5f : 1.0f;
}

void Direct3DGameScreen::load(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView *depthStencilView)
{
	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer(d3dDevice, d3dContext, renderTargetView, depthStencilView, m_iDeviceScreenWidth, m_iDeviceScreenHeight));

	RECTUtils::getInstance()->setGameScreenToDeviceScreenRatio(m_fGameScreenToDeviceScreenWidthRatio, m_fGameScreenToDeviceScreenHeightRatio);

	// Load Background Music
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(d3dDevice, DXGI_FORMAT_B8G8R8A8_UNORM);
}

void Direct3DGameScreen::platformResume()
{
	Global::getSoundPlayerInstance()->Resume();
}

void Direct3DGameScreen::platformPause()
{
	Global::getSoundPlayerInstance()->Suspend();
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	m_touchPoint->set(touchEvent.getX() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenWidthRatio, SCREEN_HEIGHT - (touchEvent.getY() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenHeightRatio));
}

void Direct3DGameScreen::unload()
{
	if (m_gameState == RUNNING)
	{
		onPause();
	}

	m_mediaPlayer->Shutdown();
	m_renderer->cleanUp();
}

void Direct3DGameScreen::handleSound()
{
	short soundId;
	while ((soundId = Assets::getInstance()->getFirstSoundId()) > 0)
	{
		Assets::getInstance()->eraseFirstSoundId();

		switch (soundId)
		{
		case SOUND_PLANT_BOMB:
			Direct3DAssets::getInstance()->m_plantBombSound->play();
			break;
		case SOUND_EXPLOSION:
			Direct3DAssets::getInstance()->m_explosionSound->play();
			break;
		case SOUND_DEATH:
			Direct3DAssets::getInstance()->m_deathSound->play();
			break;
		default:
			continue;
		}
	}
}

void Direct3DGameScreen::handleMusic()
{
	short musicId = Assets::getInstance()->getMusicId();
	Assets::getInstance()->setMusicId(0);

	switch (musicId)
	{
	case MUSIC_STOP:
		m_mediaPlayer->Pause();
		break;
	case MUSIC_PLAY_MAP_SPACE:
		m_mediaPlayer->SetSource("assets\\map_space.wav");
		m_mediaPlayer->Play();
		break;
	case MUSIC_PLAY_MAP_GRASSLANDS:
		m_mediaPlayer->SetSource("assets\\map_grasslands.wav");
		m_mediaPlayer->Play();
		break;
	case MUSIC_PLAY_MAP_MOUNTAINS:
		m_mediaPlayer->SetSource("assets\\map_mountains.wav");
		m_mediaPlayer->Play();
		break;
	case MUSIC_PLAY_MAP_BASE:
		m_mediaPlayer->SetSource("assets\\map_base.wav");
		m_mediaPlayer->Play();
		break;
	default:
		break;
	}
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	if (m_gameState == Game_State::RUNNING)
	{
		return true;
	}
	return false;
}