//
//  Direct3DRenderer.cpp
//  bomberparty
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
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_blue.dds", NULL, &m_charBlueShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_green.dds", NULL, &m_charGreenShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_pink.dds", NULL, &m_charPinkShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_red.dds", NULL, &m_charRedShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_white.dds", NULL, &m_charWhiteShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice, L"Assets\\char_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL));

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

void Direct3DRenderer::loadMapType(int mapType)
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
	m_spriteBatch->Begin();
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

			renderGameObjectWithRespectToPlayer((**itr), Assets::getPlayerTextureRegion((**itr)));
		}
	}
	m_spriteBatch->End();
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

void Direct3DRenderer::renderInterface()
{
	m_currentShaderResourceView = m_interfaceShaderResourceView;
	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_currentShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(INTERFACE_OVERLAY_BACKGROUND_X, INTERFACE_OVERLAY_BACKGROUND_Y, INTERFACE_OVERLAY_BACKGROUND_WIDTH, INTERFACE_OVERLAY_BACKGROUND_HEIGHT, false), &Assets::getInterfaceOverlayTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	m_spriteBatch->End();
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