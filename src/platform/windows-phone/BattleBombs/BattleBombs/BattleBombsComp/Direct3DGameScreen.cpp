//
//  Direct3DGameScreen.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Direct3DGameScreen.h"
#include "Vector2D.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Assets.h"
#include "OverlapTester.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"
#include "BotPlayerDynamicGameObject.h"
#include "MapSearchNode.h"
#include "GameListener.h"
#include "Renderer.h"
#include "Fire.h"
#include "Triangle.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "PathFinder.h"
#include "WaitingForServerInterface.h"
#include "WaitingForLocalSettingsInterface.h"
#include "InterfaceOverlay.h"
#include "BombButton.h"
#include "PowerUpBarItem.h"
#include "PlayerAvatar.h"
#include "Font.h"
#include "SpectatorControls.h"
#include "CountDownNumberGameObject.h"
#include "DisplayBattleGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "Direct3DRenderer.h"
#include "RECTUtils.h"
#include "Global.h"
#include "Direct3DAssets.h"
#include "GameSound.h"

Direct3DGameScreen::Direct3DGameScreen(const char *username, int deviceScreenWidth, int deviceScreenHeight, int deviceScreenDpWidth, int deviceScreenDpHeight, bool isOffline) : GameScreen(username, isOffline)
{
	m_iDeviceScreenWidth = deviceScreenWidth;
	m_iDeviceScreenHeight = deviceScreenHeight;
	m_fGameScreenToDeviceScreenWidthRatio = deviceScreenWidth / SCREEN_WIDTH;
	m_fGameScreenToDeviceScreenHeightRatio = deviceScreenHeight / SCREEN_HEIGHT;
	m_fDipToPixelRatio = (float)deviceScreenWidth / (float)deviceScreenDpWidth;
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
		case SOUND_COUNT_DOWN_3:
			Direct3DAssets::getInstance()->m_countDown3Sound->play();
			break;
		case SOUND_COUNT_DOWN_2:
			Direct3DAssets::getInstance()->m_countDown2Sound->play();
			break;
		case SOUND_COUNT_DOWN_1:
			Direct3DAssets::getInstance()->m_countDown1Sound->play();
			break;
		case SOUND_BATTLE:
			Direct3DAssets::getInstance()->m_battleSound->play();
			break;
		case SOUND_PLANT_BOMB:
			Direct3DAssets::getInstance()->m_plantBombSound->play();
			break;
		case SOUND_EXPLOSION:
			Direct3DAssets::getInstance()->m_explosionSound->play();
			break;
		case SOUND_PU_BOMB:
			Direct3DAssets::getInstance()->m_powerUpBombSound->play();
			break;
		case SOUND_PU_FIRE:
			Direct3DAssets::getInstance()->m_powerUpFireSound->play();
			break;
		case SOUND_PU_SPEED:
			Direct3DAssets::getInstance()->m_powerUpSpeedSound->play();
			break;
		case SOUND_PU_FORCE_FIELD:
			Direct3DAssets::getInstance()->m_powerUpForceFieldSound->play();
			break;
		case SOUND_PU_PUSH:
			Direct3DAssets::getInstance()->m_powerUpPushSound->play();
			break;
		case SOUND_FORCE_FIELD_DOWN:
			Direct3DAssets::getInstance()->m_forceFieldDownSound->play();
			break;
		case SOUND_DEATH:
			Direct3DAssets::getInstance()->m_deathSound->play();
			break;
		case SOUND_GAME_SET:
			Direct3DAssets::getInstance()->m_gameSetSound->play();
			break;
		case SOUND_DRAW:
			Direct3DAssets::getInstance()->m_drawSound->play();
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
	return false;
}