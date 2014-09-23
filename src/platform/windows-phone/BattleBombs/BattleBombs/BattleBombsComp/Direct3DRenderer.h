//
//  Direct3DRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#pragma once

#include "Renderer.h"
#include "Color.h"

class Line;
class Rectangle;
class SpriteBatcher;

using namespace Microsoft::WRL;

class Direct3DRenderer : public Renderer
{
public:
	Direct3DRenderer(ComPtr<ID3D11Device1> d3dDevice, ComPtr<ID3D11DeviceContext1> d3dContext, ComPtr<ID3D11RenderTargetView> rendertargetIn, ComPtr<ID3D11DepthStencilView> zbufferIn);

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
	// Direct3D Objects.
	ComPtr<ID3D11Device1> dev;                      // the device interface
	ComPtr<ID3D11DeviceContext1> devcon;            // the device context interface
	ComPtr<ID3D11RenderTargetView> rendertarget;    // the render target interface
	ComPtr<ID3D11DepthStencilView> zbuffer;         // the depth buffer interface

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

	std::unique_ptr<SpriteBatcher> m_spriteBatcher;
	std::unique_ptr<SpriteBatcher> m_spriteBatcherWithColor;

	virtual void renderGameObject(GameObject &go, TextureRegion tr);

	virtual void renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr);

	void renderLine(Line &line, Color &color);

	void renderRectangleStroke(Rectangle &rectangle, Color &color);

	void renderRectangleFill(Rectangle &rectangle, Color &color);

	void InitPipeline();
	void InitGraphics();
};