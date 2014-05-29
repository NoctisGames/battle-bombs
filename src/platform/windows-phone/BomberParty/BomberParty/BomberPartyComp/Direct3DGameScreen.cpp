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
#include "GameSound.h"
#include "Assets.h"
#include "GameConstants.h"

Direct3DGameScreen::Direct3DGameScreen(const char *username, int deviceScreenWidth, int deviceScreenHeight) : GameScreen(username)
{
	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer());
	m_iDeviceScreenWidth = deviceScreenWidth;
	m_iDeviceScreenHeight = deviceScreenHeight;
	m_fGameScreenToDeviceScreenWidthRatio = deviceScreenWidth / SCREEN_WIDTH;
	m_fGameScreenToDeviceScreenHeightRatio = deviceScreenHeight / SCREEN_HEIGHT;
	m_fDipToPixelRatio = deviceScreenHeight == 768 ? 1.6f : deviceScreenHeight == 720 ? 1.5f : 1.0f;
}

void Direct3DGameScreen::platformInit()
{

}

void Direct3DGameScreen::load(Microsoft::WRL::ComPtr<ID3D11Device1> &d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dContext)
{
	m_renderer->load(d3dDevice, d3dContext, m_iDeviceScreenWidth, m_iDeviceScreenHeight);

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

void Direct3DGameScreen::present(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dContext, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> &renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> &depthStencilView)
{
	float color[] = { 0, 0, 0, 1.0f };

	d3dContext->ClearRenderTargetView(renderTargetView.Get(), color);
	d3dContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	d3dContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	switch (m_gameState)
	{
	case WAITING_FOR_SERVER:
		m_renderer->renderWorldBackground();

		m_renderer->renderWorldForeground(m_insideBlocks, m_breakableBlocks, m_powerUps);
		m_renderer->renderInterface();
		break;
	case RUNNING:
	case SPECTATING:
		m_renderer->calcScrollYForPlayer(*m_player);

		m_renderer->renderWorldBackground();

		m_renderer->renderWorldForeground(m_insideBlocks, m_breakableBlocks, m_powerUps);
		m_renderer->renderBombs(m_bombs);
		m_renderer->renderExplosions(m_explosions);
		m_renderer->renderPlayers(m_players);
		m_renderer->renderInterface();
		m_renderer->renderControls(*m_dPad, *m_activeButton);
	default:
		break;
	}
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	m_touchPoint->set(touchEvent.getX() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenWidthRatio, SCREEN_HEIGHT - (touchEvent.getY() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenHeightRatio));
}

void Direct3DGameScreen::unload()
{
	if(m_gameState == RUNNING)
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
	case MUSIC_PLAY:
		m_mediaPlayer->SetSource("assets\\bg_game.wav");
		m_mediaPlayer->Play();
		break;
	default:
		break;
	}
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	if(m_gameState == Game_State::RUNNING)
	{
		return true;
	}
	return false;
}