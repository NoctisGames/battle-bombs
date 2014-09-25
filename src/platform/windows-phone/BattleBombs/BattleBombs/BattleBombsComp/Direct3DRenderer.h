//
//  Direct3DRenderer.h
//  battlebombs
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
#include "PowerUpType.h"
#include <Effects.h>
#include "Color.h"

class Line;
class Rectangle;
<<<<<<< HEAD
=======
class SpriteBatcher;
class Direct3DRectangleRenderer;
>>>>>>> origin/wp_experimental

class Direct3DRenderer : public Renderer
{
public:
<<<<<<< HEAD
	Direct3DRenderer(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView *depthStencilView, int deviceScreenWidth, int deviceScreenHeight);
=======
	Direct3DRenderer(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, ID3D11RenderTargetView *rendertarget);
>>>>>>> origin/wp_experimental

	virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
	
	virtual void clearScreenWithColor(float r, float g, float b, float a);

	virtual void beginFrame();

	virtual void renderWorldBackground();

	virtual void renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps);

	virtual void renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs);

	virtual void renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions);

	virtual void renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);

	virtual void renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear);

	virtual void renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface);

	virtual void renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface);

	virtual void renderUIEffects(std::vector<std::unique_ptr<CountDownNumberGameObject>> &countDownNumbers, DisplayBattleGameObject &displayBattleGameObject, std::vector<std::unique_ptr<DisplayGameOverGameObject>> &displayGameOverGameObject);

	virtual void renderInterface(InterfaceOverlay &interfaceOverlay);

	virtual void renderSpectatorInterface(InterfaceOverlay &interfaceOverlay);

	virtual void renderGameOverBlackCover(float alpha);

	virtual void renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS]);

	virtual void endFrame();

	virtual void cleanUp();

private:
<<<<<<< HEAD
	ID3D11Device1 *m_d3dDevice;
	ID3D11DeviceContext1 *m_d3dContext;
	ID3D11RenderTargetView *m_renderTargetView;
	ID3D11DepthStencilView *m_depthStencilView;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	ID3D11ShaderResourceView *m_gameShaderResourceView;
	ID3D11ShaderResourceView *m_interfaceShaderResourceView;
	ID3D11ShaderResourceView *m_interface2ShaderResourceView;
	ID3D11ShaderResourceView *m_charBlackShaderResourceView;
	ID3D11ShaderResourceView *m_charBlueShaderResourceView;
	ID3D11ShaderResourceView *m_charGreenShaderResourceView;
	ID3D11ShaderResourceView *m_charOrangeShaderResourceView;
	ID3D11ShaderResourceView *m_charPinkShaderResourceView;
	ID3D11ShaderResourceView *m_charRedShaderResourceView;
	ID3D11ShaderResourceView *m_charWhiteShaderResourceView;
	ID3D11ShaderResourceView *m_charYellowShaderResourceView;

	ID3D11ShaderResourceView *m_currentShaderResourceView;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	ID3D11BlendState *m_alphaEnableBlendingState;

	int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;
=======
	std::unique_ptr<SpriteBatcher> m_spriteBatcher;
	std::unique_ptr<Direct3DRectangleRenderer> m_rectangleRenderer;
>>>>>>> origin/wp_experimental

	virtual void renderGameObject(GameObject &go, TextureRegion tr);

	virtual void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr);

	void renderLine(Line &line, Color &color);

	void renderRectangleStroke(Rectangle &rectangle, Color &color);

	void renderRectangleFill(Rectangle &rectangle, Color &color);
};
