//
//  Direct3DRenderer.h
//  bomberparty
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#pragma once

#include "Renderer.h"
#include <DDSTextureLoader.h>
#include <SpriteBatch.h>
#include <PrimitiveBatch.h>
#include "CommonStates.h"
#include "VertexTypes.h"
#include <Effects.h>

class Direct3DRenderer : public Renderer
{
public:
	Direct3DRenderer();
	
	void load(Microsoft::WRL::ComPtr<ID3D11Device1> &d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dContext, int deviceScreenWidth, int deviceScreenHeight);

	void cleanUp();

	virtual void renderWorldBackground();

	virtual void renderWorldForeground(std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);

	virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);

	virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs);

	virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions);

	virtual void renderInterface();

	virtual void renderControls(DPadControl &dPadControl, ActiveButton &activeButton);

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	ID3D11ShaderResourceView *m_gameShaderResourceView;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	ID3D11BlendState *m_alphaEnableBlendingState;

	int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;

	virtual void renderGameObject(GameObject &go, TextureRegion tr);

	virtual void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr);
};