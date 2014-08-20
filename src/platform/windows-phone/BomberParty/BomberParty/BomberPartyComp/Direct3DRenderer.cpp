//
//  Direct3DRenderer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "DirectXHelper.h"
#include "Direct3DRenderer.h"
#include <Effects.h>
#include "RECTUtils.h"

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
#include "InterfaceOverlay.h"
#include "PowerUpBarItem.h"
#include "BombButton.h"
#include "Font.h"
#include "PlayerAvatar.h"
#include "MiniMapGridType.h"
#include "Line.h"
#include "Rectangle.h"
#include "SpectatorControls.h"

#include <string>

using namespace DirectX;

Direct3DRenderer::Direct3DRenderer(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView *depthStencilView, int deviceScreenWidth, int deviceScreenHeight) : Renderer()
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_renderTargetView = renderTargetView;
	m_depthStencilView = depthStencilView;

	// Create the SpriteBatch
	m_spriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(d3dContext));

	// Initialize Textures
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\map_space.dds", NULL, &m_gameShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\interface.dds", NULL, &m_interfaceShaderResourceView, NULL));

	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_blue.dds", NULL, &m_charBlueShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_green.dds", NULL, &m_charGreenShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_pink.dds", NULL, &m_charPinkShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_red.dds", NULL, &m_charRedShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_white.dds", NULL, &m_charWhiteShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\bot_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL));

	player_sprites_loaded[0] = true;
	player_sprites_loaded[1] = false;
	player_sprites_loaded[2] = false;
	player_sprites_loaded[3] = false;
	player_sprites_loaded[4] = false;
	player_sprites_loaded[5] = false;
	player_sprites_loaded[6] = false;
	player_sprites_loaded[7] = false;

	// Clear the blend state description.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	// To create an alpha enabled blend state description change BlendEnable to TRUE and DestBlend to D3D11_BLEND_INV_SRC_ALPHA.
	// The other settings are set to their default values which can be looked up in the Windows DirectX Graphics Documentation.
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Instead of D3D11_BLEND_ONE
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // Instead of D3D11_BLEND_ZERO
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Instead of D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; //Instead of D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//We then create an alpha enabled blending state using the description we just setup.
	// Create the blend state using the description.
	HRESULT result = d3dDevice->CreateBlendState(&blendDesc, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		// Panic!
	}

	// Set up Stuff for PrimitiveBatch

	m_primitiveBatch = std::unique_ptr<PrimitiveBatch<VertexPositionColor>>(new PrimitiveBatch<VertexPositionColor>(d3dContext));

	m_basicEffect = std::unique_ptr<BasicEffect>(new BasicEffect(d3dDevice));

	m_basicEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0, deviceScreenWidth, deviceScreenHeight, 0, 0, 1));
	m_basicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	d3dDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, shaderByteCode, byteCodeLength, &m_inputLayout);
}

void Direct3DRenderer::loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	m_gameShaderResourceView->Release();

	switch (mapType)
	{
	case MAP_SPACE:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\map_space.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	case MAP_GRASSLANDS:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\map_grasslands.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	case MAP_MOUNTAINS:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\map_mountains.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	case MAP_BASE:
	default:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, L"Assets\\map_base.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	}

	if ((players.at(0)->isBot() && player_sprites_loaded[0]) || (!players.at(0)->isBot() && !player_sprites_loaded[0]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(0)->isBot() ? L"Assets\\bot_black.dds" : L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL));
	}

	if ((players.at(1)->isBot() && player_sprites_loaded[1]) || (!players.at(1)->isBot() && !player_sprites_loaded[1]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(1)->isBot() ? L"Assets\\bot_blue.dds" : L"Assets\\char_blue.dds", NULL, &m_charBlueShaderResourceView, NULL));
	}

	if ((players.at(2)->isBot() && player_sprites_loaded[2]) || (!players.at(2)->isBot() && !player_sprites_loaded[2]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(2)->isBot() ? L"Assets\\bot_green.dds" : L"Assets\\char_green.dds", NULL, &m_charGreenShaderResourceView, NULL));
	}

	if ((players.at(3)->isBot() && player_sprites_loaded[3]) || (!players.at(3)->isBot() && !player_sprites_loaded[3]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(3)->isBot() ? L"Assets\\bot_orange.dds" : L"Assets\\char_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL));
	}

	if ((players.at(4)->isBot() && player_sprites_loaded[4]) || (!players.at(4)->isBot() && !player_sprites_loaded[4]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(4)->isBot() ? L"Assets\\bot_pink.dds" : L"Assets\\char_pink.dds", NULL, &m_charPinkShaderResourceView, NULL));
	}

	if ((players.at(5)->isBot() && player_sprites_loaded[5]) || (!players.at(5)->isBot() && !player_sprites_loaded[5]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(5)->isBot() ? L"Assets\\bot_red.dds" : L"Assets\\char_red.dds", NULL, &m_charRedShaderResourceView, NULL));
	}

	if ((players.at(6)->isBot() && player_sprites_loaded[6]) || (!players.at(6)->isBot() && !player_sprites_loaded[6]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(6)->isBot() ? L"Assets\\bot_white.dds" : L"Assets\\char_white.dds", NULL, &m_charWhiteShaderResourceView, NULL));
	}

	if ((players.at(7)->isBot() && player_sprites_loaded[7]) || (!players.at(7)->isBot() && !player_sprites_loaded[7]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice, players.at(7)->isBot() ? L"Assets\\bot_yellow.dds" : L"Assets\\char_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL));
	}

	player_sprites_loaded[0] = !players.at(0)->isBot();
	player_sprites_loaded[1] = !players.at(1)->isBot();
	player_sprites_loaded[2] = !players.at(2)->isBot();
	player_sprites_loaded[3] = !players.at(3)->isBot();
	player_sprites_loaded[4] = !players.at(4)->isBot();
	player_sprites_loaded[5] = !players.at(5)->isBot();
	player_sprites_loaded[6] = !players.at(6)->isBot();
	player_sprites_loaded[7] = !players.at(7)->isBot();
}

void Direct3DRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	m_d3dContext->ClearRenderTargetView(m_renderTargetView, color);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_d3dContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void Direct3DRenderer::renderWorldBackground()
{
	m_currentShaderResourceView = m_gameShaderResourceView;

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, false), &Assets::getWorldBackgroundTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	m_spriteBatch->End();
}

void Direct3DRenderer::renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
	if (insideBlocks.size() > 0)
	{
		m_spriteBatch->Begin();

		for (std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBordersFar.begin(); itr != mapBordersFar.end(); itr++)
		{
			if (!(*itr)->isNearFront())
			{
				renderGameObjectWithRespectToPlayer((**itr), Assets::getMapBorderTextureRegion((**itr)));
			}
		}

		for (std::vector<std::unique_ptr<InsideBlock>>::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getInsideBlockTextureRegion());
		}

		for (std::vector<std::unique_ptr<BreakableBlock>>::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getBreakableBlockTextureRegion((**itr)));
		}

		for (std::vector<std::unique_ptr<PowerUp>>::iterator itr = powerUps.begin(); itr != powerUps.end(); itr++)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getPowerUpTextureRegion((**itr)));
		}

		m_spriteBatch->End();
	}
}

void Direct3DRenderer::renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs)
{
	m_spriteBatch->Begin();
	for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
	{
		renderGameObjectWithRespectToPlayer((**itr), Assets::getBombTextureRegion((**itr)));
	}
	m_spriteBatch->End();
}

void Direct3DRenderer::renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions)
{
	m_spriteBatch->Begin();
	for (std::vector<std::unique_ptr<Explosion>>::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
	{
		for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
		{
			m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates((**itr2).getPosition().getX(), (**itr2).getPosition().getY() - m_fScrollY, (**itr2).getWidth(), (**itr2).getHeight(), true), &Assets::getFireTextureRegion((**itr2)).getSourceRECT(), Colors::White, DEGREES_TO_RADIANS_WP((**itr2).getAngle()), XMFLOAT2(16, 16), SpriteEffects_None, 0);
		}
	}
	m_spriteBatch->End();
}

void Direct3DRenderer::renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
	{
		if ((**itr).getPlayerState() != Player_State::DEAD)
		{
			switch ((**itr).getPlayerIndex())
			{
			case 0:
				m_currentShaderResourceView = m_charBlackShaderResourceView;
				break;
			case 1:
				m_currentShaderResourceView = m_charBlueShaderResourceView;
				break;
			case 2:
				m_currentShaderResourceView = m_charGreenShaderResourceView;
				break;
			case 3:
				m_currentShaderResourceView = m_charOrangeShaderResourceView;
				break;
			case 4:
				m_currentShaderResourceView = m_charPinkShaderResourceView;
				break;
			case 5:
				m_currentShaderResourceView = m_charRedShaderResourceView;
				break;
			case 6:
				m_currentShaderResourceView = m_charWhiteShaderResourceView;
				break;
			case 7:
				m_currentShaderResourceView = m_charYellowShaderResourceView;
				break;
			default:
				break;
			}

			m_spriteBatch->Begin();
			renderGameObjectWithRespectToPlayer((**itr), Assets::getPlayerTextureRegion((**itr)));
			m_spriteBatch->End();
		}
	}
}

void Direct3DRenderer::renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear)
{
	m_currentShaderResourceView = m_gameShaderResourceView;

	m_spriteBatch->Begin();
	for (std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBordersNear.begin(); itr != mapBordersNear.end(); itr++)
	{
		if ((*itr)->isNearFront())
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getMapBorderTextureRegion((**itr)));
		}
	}
	m_spriteBatch->End();
}

void Direct3DRenderer::renderInterface(InterfaceOverlay &interfaceOverlay)
{
	m_currentShaderResourceView = m_interfaceShaderResourceView;

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(INTERFACE_OVERLAY_BACKGROUND_X, INTERFACE_OVERLAY_BACKGROUND_Y, INTERFACE_OVERLAY_BACKGROUND_WIDTH, INTERFACE_OVERLAY_BACKGROUND_HEIGHT, false), &Assets::getInterfaceOverlayTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);

	for (std::vector<std::unique_ptr<PlayerAvatar>>::iterator itr = interfaceOverlay.getPlayerAvatars().begin(); itr != interfaceOverlay.getPlayerAvatars().end(); itr++)
	{
		renderGameObject((**itr), Assets::getPlayerAvatarTextureRegion((**itr)));
	}

	renderGameObject(interfaceOverlay.getDPadControl(), Assets::getDPadControlTextureRegion(interfaceOverlay.getDPadControl()));

	for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
	{
		if ((**itr).getPowerUpType() != NONE)
		{
			renderGameObject((**itr), Assets::getPowerUpBarItemTextureRegion((**itr), interfaceOverlay.getPowerUpBarItemsStateTime()));
		}
	}

	if (interfaceOverlay.getActiveButton().getPowerUpType() == PUSH)
	{
		renderGameObject(interfaceOverlay.getActiveButton(), Assets::getActiveButtonTextureRegion(interfaceOverlay.getActiveButton(), interfaceOverlay.getButtonsStateTime()));
	}

	renderGameObject(interfaceOverlay.getBombButton(), Assets::getBombButtonTextureRegion(interfaceOverlay.getBombButton(), interfaceOverlay.getButtonsStateTime()));

	m_spriteBatch->End();

	std::string timeRemaining = std::to_string(interfaceOverlay.getNumMinutesLeft()) + ":" + std::to_string(interfaceOverlay.getNumSecondsLeftFirstColumn()) + std::to_string(interfaceOverlay.getNumSecondsLeftSecondColumn());
	static DirectX::XMVECTORF32 interfaceColor = { 1, 1, 1, 1 };

	static const float timerX = 0.3554104477903f;
	static const float timerY = 13.0589552253125f;
	static const float timerWidth = 0.40298507462688f;
	static const float timerHeight = 0.425373134375f;

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, m_alphaEnableBlendingState);
	m_font->renderText(*m_spriteBatch, m_currentShaderResourceView, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);

	for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
	{
		if ((**itr).getPowerUpType() != NONE)
		{
			std::string powerUpStack = std::to_string((*itr)->getLevel());
			float x = (*itr)->getPosition().getX() + (*itr)->getWidth() / 2;
			float y = (*itr)->getPosition().getY() - (*itr)->getHeight() / 2;
			m_font->renderText(*m_spriteBatch, m_currentShaderResourceView, powerUpStack, x, y, 0.36f, 0.32f, interfaceColor);
		}
	}

	m_spriteBatch->End();

	for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
		{
			int miniMapGridType = interfaceOverlay.getMiniMapGridType(j, i);
			if (miniMapGridType != MINI_MAP_FREE_SPACE)
			{
				static const float miniMapLeftX = 0.35820895522392f;
				static const float miniMapBottomY = 10.464179105625f;
				static const float miniMapGridWidth = 0.08955223880597f;
				static const float miniMapGridHeight = 0.085074626875f;

				float leftX = miniMapLeftX + miniMapGridWidth * j;
				float bottomY = miniMapBottomY + miniMapGridHeight * i;
				Rectangle miniMapGridCell = Rectangle(leftX, bottomY, miniMapGridWidth, miniMapGridHeight);
				renderRectangleFill(miniMapGridCell, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
			}
		}
	}
}

void Direct3DRenderer::renderSpectatorInterface(InterfaceOverlay &interfaceOverlay)
{
	m_currentShaderResourceView = m_interfaceShaderResourceView;

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_X, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_Y, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_WIDTH, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_HEIGHT, false), &Assets::getSpectatorInterfaceOverlayTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	renderGameObject(interfaceOverlay.getSpectatorControls(), Assets::getSpectatorControlsTextureRegion(interfaceOverlay.getSpectatorControls()));
	m_spriteBatch->End();

	static DirectX::XMVECTORF32 interfaceColor = { 1, 1, 1, 1 };

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, m_alphaEnableBlendingState);

	std::string timeRemaining = std::to_string(interfaceOverlay.getNumMinutesLeft()) + ":" + std::to_string(interfaceOverlay.getNumSecondsLeftFirstColumn()) + std::to_string(interfaceOverlay.getNumSecondsLeftSecondColumn());

	static const float timerX = 3.43731343283576f;
	static const float timerY = 0.71044776125f;
	static const float timerWidth = 0.40298507462688f;
	static const float timerHeight = 0.425373134375f;

	m_font->renderText(*m_spriteBatch, m_currentShaderResourceView, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);

	static const float spectatingWhoX = 12.17910447761184f;
	static const float spectatingWhoY = 0.51044776125f;
	static const float spectatingWhoWidth = 0.52611940298496f;
	static const float spectatingWhoHeight = 0.680597015f;

	m_font->renderText(*m_spriteBatch, m_currentShaderResourceView, interfaceOverlay.getSpectatingUsername(), spectatingWhoX, spectatingWhoY, spectatingWhoWidth, spectatingWhoHeight, interfaceColor, true);

	m_spriteBatch->End();

	for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
		{
			int miniMapGridType = interfaceOverlay.getMiniMapGridType(j, i);
			if (miniMapGridType != MINI_MAP_FREE_SPACE)
			{
				static const float miniMapLeftX = 0.7611940298508f;
				static const float miniMapBottomY = 0.085074626875f;
				static const float miniMapGridWidth = 0.08955223880597f;
				static const float miniMapGridHeight = 0.085074626875f;

				float leftX = miniMapLeftX + miniMapGridWidth * j;
				float bottomY = miniMapBottomY + miniMapGridHeight * i;
				Rectangle miniMapGridCell = Rectangle(leftX, bottomY, miniMapGridWidth, miniMapGridHeight);
				renderRectangleFill(miniMapGridCell, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
			}
		}
	}
}

void Direct3DRenderer::renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
{
	m_currentShaderResourceView = m_interfaceShaderResourceView;

	m_spriteBatch->Begin();
	for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
		{
			if (game_grid[j][i] == 1)
			{
				m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(GAME_X + GRID_CELL_WIDTH * j + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * i + GRID_CELL_HEIGHT / 2.0f - m_fScrollY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, false), &Assets::getOneTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
			}
			else
			{
				m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(GAME_X + GRID_CELL_WIDTH * j + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * i + GRID_CELL_HEIGHT / 2.0f - m_fScrollY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, false), &Assets::getNineTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
			}
		}
	}m_spriteBatch->End();
}

void Direct3DRenderer::endFrame()
{
	// Not Needed Yet
}

void Direct3DRenderer::cleanUp()
{
	m_gameShaderResourceView->Release();
	m_interfaceShaderResourceView->Release();
	m_charBlackShaderResourceView->Release();
	m_charBlueShaderResourceView->Release();
	m_charGreenShaderResourceView->Release();
	m_charOrangeShaderResourceView->Release();
	m_charPinkShaderResourceView->Release();
	m_charRedShaderResourceView->Release();
	m_charWhiteShaderResourceView->Release();
	m_charYellowShaderResourceView->Release();
}

// Private

void Direct3DRenderer::renderGameObject(GameObject &go, TextureRegion tr)
{
	m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForGameObject(go, false), &tr.getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
}

void Direct3DRenderer::renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr)
{
	if (m_fScrollY > 0)
	{
		m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(go.getPosition().getX(), go.getPosition().getY() - m_fScrollY, go.getWidth(), go.getHeight(), false), &tr.getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	}
	else
	{
		renderGameObject(go, tr);
	}
}

void Direct3DRenderer::renderLine(Line &line, Color &color)
{
	Vector2D origin = line.getOrigin();
	float originX = origin.getX();
	float originY = origin.getY();

	float scaledOriginX = RECTUtils::getInstance()->convertGameXToScreenX(originX);
	float scaledOriginY = RECTUtils::getInstance()->convertGameYToScreenY(originY);

	XMVECTORF32 originVector = { scaledOriginX, scaledOriginY, 0.0f };
	XMVECTORF32 colorXMVECTORF32 = { color.red, color.green, color.blue, color.alpha };
	VertexPositionColor originVertexPositionColor = VertexPositionColor(originVector, colorXMVECTORF32);

	Vector2D end = line.getEnd();
	float endX = end.getX();
	float endY = end.getY();

	float scaledEndX = RECTUtils::getInstance()->convertGameXToScreenX(endX);
	float scaledEndY = RECTUtils::getInstance()->convertGameYToScreenY(endY);

	XMVECTORF32 endVector = { scaledEndX, scaledEndY, 0.0f };
	VertexPositionColor endVertexPositionColor = VertexPositionColor(endVector, colorXMVECTORF32);

	m_basicEffect->SetAlpha(colorXMVECTORF32.f[3]);
	m_basicEffect->Apply(m_d3dContext);
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->Begin();

	m_primitiveBatch->DrawLine(originVertexPositionColor, endVertexPositionColor);

	m_primitiveBatch->End();
}

void Direct3DRenderer::renderRectangleStroke(Rectangle &rectangle, Color &color)
{
	Vector2D lowerLeft = rectangle.getLowerLeft();
	float lowerLeftX = lowerLeft.getX();
	float lowerLeftY = lowerLeft.getY();
	float width = rectangle.getWidth();
	float height = rectangle.getHeight();

	RECT rect = RECTUtils::getInstance()->getRECTForCoordinates(lowerLeftX + width / 2, lowerLeftY + height / 2, width, height, false);

	XMVECTORF32 topLeft = { rect.left, rect.top, 0.0f };
	XMVECTORF32 topRight = { rect.right, rect.top, 0.0f };
	XMVECTORF32 bottomRight = { rect.right, rect.bottom, 0.0f };
	XMVECTORF32 bottomLeft = { rect.left, rect.bottom, 0.0f };

	XMVECTORF32 colorXMVECTORF32 = { color.red, color.green, color.blue, color.alpha };

	VertexPositionColor topLeftVertexPositionColor = VertexPositionColor(topLeft, colorXMVECTORF32);
	VertexPositionColor topRightVertexPositionColor = VertexPositionColor(topRight, colorXMVECTORF32);
	VertexPositionColor bottomRightVertexPositionColor = VertexPositionColor(bottomRight, colorXMVECTORF32);
	VertexPositionColor bottomLeftVertexPositionColor = VertexPositionColor(bottomLeft, colorXMVECTORF32);

	m_basicEffect->SetAlpha(colorXMVECTORF32.f[3]);
	m_basicEffect->Apply(m_d3dContext);
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->Begin();

	m_primitiveBatch->DrawLine(topLeftVertexPositionColor, topRightVertexPositionColor);
	m_primitiveBatch->DrawLine(topRightVertexPositionColor, bottomRightVertexPositionColor);
	m_primitiveBatch->DrawLine(bottomRightVertexPositionColor, bottomLeftVertexPositionColor);
	m_primitiveBatch->DrawLine(bottomLeftVertexPositionColor, topLeftVertexPositionColor);

	m_primitiveBatch->End();
}

void Direct3DRenderer::renderRectangleFill(Rectangle &rectangle, Color &color)
{
	Vector2D lowerLeft = rectangle.getLowerLeft();
	float lowerLeftX = lowerLeft.getX();
	float lowerLeftY = lowerLeft.getY();
	float width = rectangle.getWidth();
	float height = rectangle.getHeight();

	RECT rect = RECTUtils::getInstance()->getRECTForCoordinates(lowerLeftX + width / 2, lowerLeftY + height / 2, width, height, false);

	XMVECTORF32 topLeft = { rect.left, rect.top, 0.0f };
	XMVECTORF32 topRight = { rect.right, rect.top, 0.0f };
	XMVECTORF32 bottomRight = { rect.right, rect.bottom, 0.0f };
	XMVECTORF32 bottomLeft = { rect.left, rect.bottom, 0.0f };

	XMVECTORF32 colorXMVECTORF32 = { color.red, color.green, color.blue, color.alpha }; 
	
	VertexPositionColor topLeftVertexPositionColor = VertexPositionColor(topLeft, colorXMVECTORF32);
	VertexPositionColor topRightVertexPositionColor = VertexPositionColor(topRight, colorXMVECTORF32);
	VertexPositionColor bottomRightVertexPositionColor = VertexPositionColor(bottomRight, colorXMVECTORF32);
	VertexPositionColor bottomLeftVertexPositionColor = VertexPositionColor(bottomLeft, colorXMVECTORF32);

	m_basicEffect->SetAlpha(colorXMVECTORF32.f[3]);
	m_basicEffect->Apply(m_d3dContext);
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->Begin();

	m_primitiveBatch->DrawQuad(topLeftVertexPositionColor, topRightVertexPositionColor, bottomRightVertexPositionColor, bottomLeftVertexPositionColor);

	m_primitiveBatch->End();
}