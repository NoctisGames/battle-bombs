//
//  Direct3DGameScreen.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
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
#include "DisplayXMovingGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "Direct3DRenderer.h"
#include "Global.h"
#include "GameSound.h"
#include "SpriteBatcher.h"
#include "Direct3DRectangleBatcher.h"
#include "PlayerRowAvatar.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"
#include "DirectXManager.h"
#include "Map.h"

Direct3DGameScreen::Direct3DGameScreen(const char *username, bool isOffline) : GameScreen(username, isOffline)
{
	// No further setup
}

void Direct3DGameScreen::load(float deviceScreenWidth, float deviceScreenHeight, int deviceScreenDpWidth, int deviceScreenDpHeight)
{
	m_iDeviceScreenWidth = deviceScreenWidth;
	m_iDeviceScreenHeight = deviceScreenHeight;
	m_fGameScreenToDeviceScreenWidthRatio = deviceScreenWidth / SCREEN_WIDTH;
	m_fGameScreenToDeviceScreenHeightRatio = deviceScreenHeight / SCREEN_HEIGHT;
	m_fDipToPixelRatio = (float)deviceScreenWidth / (float)deviceScreenDpWidth;

	DXManager->init(deviceScreenWidth, deviceScreenHeight);

	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer());

	// Load Background Music
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(DXManager->m_device, DXGI_FORMAT_B8G8R8A8_UNORM);

	m_countDown3Sound = std::unique_ptr<GameSound>(new GameSound("assets\\countdown_3.wav"));
	m_countDown2Sound = std::unique_ptr<GameSound>(new GameSound("assets\\countdown_2.wav"));
	m_countDown1Sound = std::unique_ptr<GameSound>(new GameSound("assets\\countdown_1.wav"));
	m_battleSound = std::unique_ptr<GameSound>(new GameSound("assets\\battle.wav"));
	m_plantBombSound = std::unique_ptr<GameSound>(new GameSound("assets\\plant_bomb.wav"));
	m_explosionSound = std::unique_ptr<GameSound>(new GameSound("assets\\explosion.wav"));
	m_powerUpBombSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_bomb.wav"));
	m_powerUpFireSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_fire.wav"));
	m_powerUpSpeedSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_speed.wav"));
	m_powerUpForceFieldSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_force_field.wav"));
	m_powerUpPushSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_push.wav"));
	m_powerUpShieldSound = std::unique_ptr<GameSound>(new GameSound("assets\\pu_shield.wav"));
	m_forceFieldDownSound = std::unique_ptr<GameSound>(new GameSound("assets\\force_field_down.wav"));
	m_deathSound = std::unique_ptr<GameSound>(new GameSound("assets\\death.wav"));
	m_hurrySound = std::unique_ptr<GameSound>(new GameSound("assets\\hurry.wav"));
	m_gameSetSound = std::unique_ptr<GameSound>(new GameSound("assets\\game_set.wav"));
	m_drawSound = std::unique_ptr<GameSound>(new GameSound("assets\\draw.wav"));
	m_raiseShieldSound = std::unique_ptr<GameSound>(new GameSound("assets\\shield_raise.wav"));
	m_dislodgingSpaceTileSound = std::unique_ptr<GameSound>(new GameSound("assets\\dislodging_space_tile.wav"));
	m_fallingSpaceTileSound = std::unique_ptr<GameSound>(new GameSound("assets\\falling_space_tile.wav"));
	m_fallingObjectSound = std::unique_ptr<GameSound>(new GameSound("assets\\falling_object.wav"));
	m_crashingFireBallSound = std::unique_ptr<GameSound>(new GameSound("assets\\crashing_fire_ball.wav"));
	m_crashingIceBallSound = std::unique_ptr<GameSound>(new GameSound("assets\\crashing_ice_ball.wav"));
}

void Direct3DGameScreen::updateForRenderResolutionChange(float width, float height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;

	ID3D11RenderTargetView* nullViews[] = { nullptr };
	DXManager->m_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	DXManager->m_renderTarget = nullptr;
	DXManager->m_renderTargetView = nullptr;
	DXManager->m_deviceContext->Flush();

	DXManager->initWindowSizeDependentResources(width, height);
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
			m_countDown3Sound->play();
			break;
		case SOUND_COUNT_DOWN_2:
			m_countDown2Sound->play();
			break;
		case SOUND_COUNT_DOWN_1:
			m_countDown1Sound->play();
			break;
		case SOUND_BATTLE:
			m_battleSound->play();
			break;
		case SOUND_PLANT_BOMB:
			m_plantBombSound->play();
			break;
		case SOUND_EXPLOSION:
			m_explosionSound->play();
			break;
		case SOUND_PU_BOMB:
			m_powerUpBombSound->play();
			break;
		case SOUND_PU_FIRE:
			m_powerUpFireSound->play();
			break;
		case SOUND_PU_SPEED:
			m_powerUpSpeedSound->play();
			break;
		case SOUND_PU_FORCE_FIELD:
			m_powerUpForceFieldSound->play();
			break;
		case SOUND_PU_PUSH:
			m_powerUpPushSound->play();
			break;
		case SOUND_PU_SHIELD:
			m_powerUpShieldSound->play();
			break;
		case SOUND_FORCE_FIELD_DOWN:
			m_forceFieldDownSound->play();
			break;
		case SOUND_DEATH:
			m_deathSound->play();
			break;
		case SOUND_HURRY:
			m_hurrySound->play();
			break;
		case SOUND_GAME_SET:
			m_gameSetSound->play();
			break;
		case SOUND_DRAW:
			m_drawSound->play();
			break;
		case SOUND_RAISE_SHIELD:
			m_raiseShieldSound->play();
			break;
		case SOUND_DISLODGING_SPACE_TILE:
			m_dislodgingSpaceTileSound->play();
			break;
		case SOUND_FALLING_SPACE_TILE:
			m_fallingSpaceTileSound->play();
			break;
		case SOUND_FALLING_OBJECT:
			m_fallingObjectSound->play();
			break;
		case SOUND_CRASHING_FIRE_BALL:
			m_crashingFireBallSound->play();
			break;
		case SOUND_CRASHING_ICE_BALL:
			m_crashingIceBallSound->play();
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

void Direct3DGameScreen::unload()
{
	onPause();

	m_mediaPlayer->Shutdown();

	m_renderer->cleanUp();

	DXManager->cleanUp();
}

ID3D11Texture2D* Direct3DGameScreen::getTexture()
{
	return DXManager->m_renderTarget;
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	m_touchPoint->set(touchEvent.getX() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenWidthRatio, SCREEN_HEIGHT - (touchEvent.getY() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenHeightRatio));
}

void Direct3DGameScreen::onResume()
{
    GameScreen::onResume();
    
	Global::getSoundPlayerInstance()->Resume();
}

void Direct3DGameScreen::onPause()
{
    GameScreen::onPause();
    
	Global::getSoundPlayerInstance()->Suspend();
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	return false;
}