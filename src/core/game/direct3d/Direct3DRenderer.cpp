//
//  Direct3DRenderer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "Direct3DRenderer.h"
#include "GameObject.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "DPadControl.h"
#include "ActiveButton.h"
#include "PlayerDynamicGameObject.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "Vector2D.h"
#include "Fire.h"
#include "GameEvent.h"
#include "WaitingForServerInterface.h"
#include "InterfaceOverlay.h"
#include "PowerUpBarItem.h"
#include "BombButton.h"
#include "Font.h"
#include "PlayerAvatar.h"
#include "MiniMapGridType.h"
#include "Line.h"
#include "Rectangle.h"
#include "PlayerForceFieldState.h"
#include "SpectatorControls.h"
#include "PowerUpType.h"
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "CountDownNumberGameObject.h"
#include "DisplayXMovingGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "BasicReaderWriter.h"
#include "DDSTextureLoader.h"
#include "SpriteBatcher.h"
#include "Direct3DRectangleBatcher.h"
#include "Direct3DSpriteBatcher.h"
#include "DirectXManager.h"

#include <d3d11_1.h>

#include <string>
#include <sstream>

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D11ShaderResourceView> m_mapShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_gameShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_interfaceShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_interface2ShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charBlackShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charBlueShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charGreenShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charOrangeShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charPinkShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charRedShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charWhiteShaderResourceView;
ComPtr<ID3D11ShaderResourceView> m_charYellowShaderResourceView;

Direct3DRenderer::Direct3DRenderer() : Renderer()
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher());
	m_rectangleBatcher = std::unique_ptr<Direct3DRectangleBatcher>(new Direct3DRectangleBatcher(true));

	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\map_space.dds", NULL, &m_mapShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\game.dds", NULL, &m_gameShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\interface.dds", NULL, &m_interfaceShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\interface_2.dds", NULL, &m_interface2ShaderResourceView, NULL);

	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_blue.dds", NULL, &m_charBlueShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_green.dds", NULL, &m_charGreenShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_pink.dds", NULL, &m_charPinkShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_red.dds", NULL, &m_charRedShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_white.dds", NULL, &m_charWhiteShaderResourceView, NULL);
	CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\bot_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL);
	
	m_mapTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_mapShaderResourceView.Get()));
	m_gameTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_gameShaderResourceView.Get()));
	m_interfaceTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_interfaceShaderResourceView.Get()));
	m_interfaceTexture2 = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_interface2ShaderResourceView.Get()));

	m_charBlackTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charBlackShaderResourceView.Get()));
	m_charBlueTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charBlueShaderResourceView.Get()));
	m_charGreenTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charGreenShaderResourceView.Get()));
	m_charOrangeTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charOrangeShaderResourceView.Get()));
	m_charPinkTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charPinkShaderResourceView.Get()));
	m_charRedTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charRedShaderResourceView.Get()));
	m_charWhiteTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charWhiteShaderResourceView.Get()));
	m_charYellowTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_charYellowShaderResourceView.Get()));
}

void Direct3DRenderer::loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	switch (mapType)
	{
	case MAP_SPACE:
		CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\map_space.dds", NULL, &m_mapShaderResourceView, NULL);
		break;
	case MAP_GRASSLANDS:
		CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\map_grasslands.dds", NULL, &m_mapShaderResourceView, NULL);
		break;
	case MAP_MOUNTAINS:
		CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\map_mountains.dds", NULL, &m_mapShaderResourceView, NULL);
		break;
	case MAP_BASE:
	default:
		CreateDDSTextureFromFile(DXManager->m_device, L"Assets\\map_base.dds", NULL, &m_mapShaderResourceView, NULL);
		break;
	}

	if ((players.at(0)->isBot() && player_sprites_loaded[0]) || (!players.at(0)->isBot() && !player_sprites_loaded[0]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(0)->isBot() ? L"Assets\\bot_black.dds" : L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL);
	}

	if ((players.at(1)->isBot() && player_sprites_loaded[1]) || (!players.at(1)->isBot() && !player_sprites_loaded[1]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(1)->isBot() ? L"Assets\\bot_blue.dds" : L"Assets\\char_blue.dds", NULL, &m_charBlueShaderResourceView, NULL);
	}

	if ((players.at(2)->isBot() && player_sprites_loaded[2]) || (!players.at(2)->isBot() && !player_sprites_loaded[2]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(2)->isBot() ? L"Assets\\bot_green.dds" : L"Assets\\char_green.dds", NULL, &m_charGreenShaderResourceView, NULL);
	}

	if ((players.at(3)->isBot() && player_sprites_loaded[3]) || (!players.at(3)->isBot() && !player_sprites_loaded[3]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(3)->isBot() ? L"Assets\\bot_orange.dds" : L"Assets\\char_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL);
	}

	if ((players.at(4)->isBot() && player_sprites_loaded[4]) || (!players.at(4)->isBot() && !player_sprites_loaded[4]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(4)->isBot() ? L"Assets\\bot_pink.dds" : L"Assets\\char_pink.dds", NULL, &m_charPinkShaderResourceView, NULL);
	}

	if ((players.at(5)->isBot() && player_sprites_loaded[5]) || (!players.at(5)->isBot() && !player_sprites_loaded[5]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(5)->isBot() ? L"Assets\\bot_red.dds" : L"Assets\\char_red.dds", NULL, &m_charRedShaderResourceView, NULL);
	}

	if ((players.at(6)->isBot() && player_sprites_loaded[6]) || (!players.at(6)->isBot() && !player_sprites_loaded[6]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(6)->isBot() ? L"Assets\\bot_white.dds" : L"Assets\\char_white.dds", NULL, &m_charWhiteShaderResourceView, NULL);
	}

	if ((players.at(7)->isBot() && player_sprites_loaded[7]) || (!players.at(7)->isBot() && !player_sprites_loaded[7]))
	{
		CreateDDSTextureFromFile(DXManager->m_device, players.at(7)->isBot() ? L"Assets\\bot_yellow.dds" : L"Assets\\char_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL);
	}

	m_mapTexture->texture = m_mapShaderResourceView.Get();
	m_charBlackTexture->texture = m_charBlackShaderResourceView.Get();
	m_charBlueTexture->texture = m_charBlueShaderResourceView.Get();
	m_charGreenTexture->texture = m_charGreenShaderResourceView.Get();
	m_charOrangeTexture->texture = m_charOrangeShaderResourceView.Get();
	m_charPinkTexture->texture = m_charPinkShaderResourceView.Get();
	m_charRedTexture->texture = m_charRedShaderResourceView.Get();
	m_charWhiteTexture->texture = m_charWhiteShaderResourceView.Get();
	m_charYellowTexture->texture = m_charYellowShaderResourceView.Get();

	updatePlayerSpritesLoadedArray(players);
}

void Direct3DRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	// set our new render target object as the active render target
	DXManager->m_deviceContext->OMSetRenderTargets(1, &DXManager->m_renderTargetView, nullptr);

	DXManager->m_deviceContext->ClearRenderTargetView(DXManager->m_renderTargetView, color);
}

void Direct3DRenderer::beginFrame()
{
	// Empty
}

void Direct3DRenderer::endFrame()
{
	// Not Needed Yet
}