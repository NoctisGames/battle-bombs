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
#include "InsideBlock.h"
#include "BorderBlock.h"
#include "BreakableBlock.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"

using namespace DirectX;

Direct3DRenderer::Direct3DRenderer() : Renderer()
{

}

void Direct3DRenderer::load(Microsoft::WRL::ComPtr<ID3D11Device1> &d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dContext, int deviceScreenWidth, int deviceScreenHeight)
{
	// Create the SpriteBatch
	m_spriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(d3dContext.Get()));

	// Initialize Textures
	DX::ThrowIfFailed(CreateDDSTextureFromFile(d3dDevice.Get(), L"Assets\\game.dds", NULL, &m_gameShaderResourceView, NULL));

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
	HRESULT result = d3dDevice.Get()->CreateBlendState(&blendDesc, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		// Panic!
	}

	// Set up Stuff for PrimitiveBatch

	m_primitiveBatch = std::unique_ptr<PrimitiveBatch<VertexPositionColor>>(new PrimitiveBatch<VertexPositionColor>(d3dContext.Get()));

	m_basicEffect = std::unique_ptr<BasicEffect>(new BasicEffect(d3dDevice.Get()));

	m_basicEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0, deviceScreenWidth, deviceScreenHeight, 0, 0, 1));
	m_basicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	d3dDevice.Get()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, shaderByteCode, byteCodeLength, &m_inputLayout);
}

void Direct3DRenderer::cleanUp()
{
	m_gameShaderResourceView->Release();
}

void Direct3DRenderer::renderWorldBackground()
{
	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_gameShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, false), &Assets::getWorldBackgroundTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	m_spriteBatch->End();
}

void Direct3DRenderer::renderWorldForeground(std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
	if (insideBlocks.size() > 0)
	{
		m_spriteBatch->Begin();

		for (std::vector<std::unique_ptr<InsideBlock>>::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getInsideBlockTextureRegion());
		}

		for (std::vector<std::unique_ptr<BreakableBlock>>::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getBreakableBlockTextureRegion());
		}

		for (std::vector<std::unique_ptr<PowerUp>>::iterator itr = powerUps.begin(); itr != powerUps.end(); itr++)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getPowerUpTextureRegion((**itr)));
		}

		m_spriteBatch->End();
	}
}

void Direct3DRenderer::renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	m_spriteBatch->Begin();
	for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
	{
		if ((**itr).getPlayerState() != Player_State::DEAD)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getPlayerTextureRegion((**itr)));
		}
	}
	m_spriteBatch->End();
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
		m_spriteBatch->Draw(m_gameShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates((**itr).getPosition().getX(), (**itr).getPosition().getY() - m_fScrollY, (**itr).getWidth(), (**itr).getHeight(), true), &Assets::getExplosionTextureRegion((**itr)).getSourceRECT(), Colors::White, DEGREES_TO_RADIANS_WP((**itr).getAngle()), XMFLOAT2(8, 8), SpriteEffects_None, 0);
	}
	m_spriteBatch->End();
}

void Direct3DRenderer::renderInterface()
{
	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_gameShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(INTERFACE_BACKGROUND_X, INTERFACE_BACKGROUND_Y, INTERFACE_BACKGROUND_WIDTH, INTERFACE_BACKGROUND_HEIGHT, false), &Assets::getInterfaceOverlayTextureRegion().getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	m_spriteBatch->End();
}

void Direct3DRenderer::renderControls(DPadControl &dPadControl, ActiveButton &activeButton)
{
	m_spriteBatch->Begin();
	renderGameObject(dPadControl, Assets::getDPadControlTextureRegion());
	m_spriteBatch->End();

	m_spriteBatch->Begin();
	renderGameObject(activeButton, Assets::getActiveButtonTextureRegion());
	m_spriteBatch->End();
}

void Direct3DRenderer::renderGameObject(GameObject &go, TextureRegion tr)
{
	m_spriteBatch->Draw(m_gameShaderResourceView, RECTUtils::getInstance()->getRECTForGameObject(go, false), &tr.getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
}

void Direct3DRenderer::renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr)
{
	if (m_fScrollY > 0)
	{
		m_spriteBatch->Draw(m_gameShaderResourceView, RECTUtils::getInstance()->getRECTForCoordinates(go.getPosition().getX(), go.getPosition().getY() - m_fScrollY, go.getWidth(), go.getHeight(), false), &tr.getSourceRECT(), Colors::White, 0, XMFLOAT2(0, 0), SpriteEffects_None, 0);
	}
	else
	{
		renderGameObject(go, tr);
	}
}