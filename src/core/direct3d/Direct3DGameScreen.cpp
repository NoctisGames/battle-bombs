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
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"

ComPtr<ID3D11Device1> dev;                      // the device interface
ComPtr<ID3D11DeviceContext1> devcon;            // the device context interface
ComPtr<ID3D11Texture2D> m_renderTarget;         // the render target texture
ComPtr<ID3D11RenderTargetView> rendertarget;    // the render target interface

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

	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers with this flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description. All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	D3D11CreateDevice(
		nullptr, // Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags, // Set set debug and Direct2D compatibility flags.
		featureLevels, // List of feature levels this app can support.
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION.
		&device, // Returns the Direct3D device created.
		&m_featureLevel, // Returns feature level of device created.
		&context // Returns the device immediate context.
		);

	// Get the Direct3D 11.1 API device and context interfaces.
	device.As(&dev);
	context.As(&devcon);

	// Create a descriptor for the render target buffer.
	CD3D11_TEXTURE2D_DESC renderTargetDesc(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		static_cast<UINT>(deviceScreenWidth),
		static_cast<UINT>(deviceScreenHeight),
		1,
		1,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);
	renderTargetDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;

	// Allocate a 2-D surface as the render target buffer.
	dev->CreateTexture2D(&renderTargetDesc, nullptr, &m_renderTarget);
	dev->CreateRenderTargetView(m_renderTarget.Get(), nullptr, &rendertarget);

	// set the viewport
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(deviceScreenWidth);
	viewport.Height = static_cast<float>(deviceScreenHeight);

	devcon->RSSetViewports(1, &viewport);

	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer(dev.Get(), devcon.Get(), rendertarget.Get()));

	// Load Background Music
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(dev.Get(), DXGI_FORMAT_B8G8R8A8_UNORM);

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
	m_forceFieldDownSound = std::unique_ptr<GameSound>(new GameSound("assets\\force_field_down.wav"));
	m_deathSound = std::unique_ptr<GameSound>(new GameSound("assets\\death.wav"));
	m_hurrySound = std::unique_ptr<GameSound>(new GameSound("assets\\hurry.wav"));
	m_gameSetSound = std::unique_ptr<GameSound>(new GameSound("assets\\game_set.wav"));
	m_drawSound = std::unique_ptr<GameSound>(new GameSound("assets\\draw.wav"));
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
			m_powerUpPushSound->play();
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
	if (m_gameState == RUNNING)
	{
		onPause();
	}

	m_mediaPlayer->Shutdown();
}

ID3D11Texture2D* Direct3DGameScreen::getTexture()
{
	return m_renderTarget.Get();
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	m_touchPoint->set(touchEvent.getX() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenWidthRatio, SCREEN_HEIGHT - (touchEvent.getY() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenHeightRatio));
}

void Direct3DGameScreen::platformResume()
{
	Global::getSoundPlayerInstance()->Resume();
}

void Direct3DGameScreen::platformPause()
{
	Global::getSoundPlayerInstance()->Suspend();
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	return false;
}