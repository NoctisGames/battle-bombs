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
#include "DisplayBattleGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "BasicReaderWriter.h"
#include "DDSTextureLoader.h"
#include "SpriteBatcher.h"
#include "Vertices2D.h"
#include <string>
#include <sstream>

using namespace DirectX;
using namespace Microsoft::WRL;

Direct3DRenderer::Direct3DRenderer(ComPtr<ID3D11Device1> d3dDevice, ComPtr<ID3D11DeviceContext1> d3dContext, ComPtr<ID3D11RenderTargetView> rendertargetIn, ComPtr<ID3D11DepthStencilView> zbufferIn) : Renderer()
{
	dev = d3dDevice;
	devcon = d3dContext;
	rendertarget = rendertargetIn;
	zbuffer = zbufferIn;

	m_spriteBatcher = std::unique_ptr<SpriteBatcher>(new SpriteBatcher(d3dDevice, d3dContext, 4000, false));
	m_spriteBatcherWithColor = std::unique_ptr<SpriteBatcher>(new SpriteBatcher(d3dDevice, d3dContext, 1000, true));

	// Initialize Textures
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\map_space.dds", NULL, &m_gameShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\interface.dds", NULL, &m_interfaceShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\interface_2.dds", NULL, &m_interface2ShaderResourceView, NULL));

	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_blue.dds", NULL, &m_charBlueShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_green.dds", NULL, &m_charGreenShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_pink.dds", NULL, &m_charPinkShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_red.dds", NULL, &m_charRedShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_white.dds", NULL, &m_charWhiteShaderResourceView, NULL));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\bot_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL));

	player_sprites_loaded[0] = true;
	player_sprites_loaded[1] = false;
	player_sprites_loaded[2] = false;
	player_sprites_loaded[3] = false;
	player_sprites_loaded[4] = false;
	player_sprites_loaded[5] = false;
	player_sprites_loaded[6] = false;
	player_sprites_loaded[7] = false;
}

void Direct3DRenderer::loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	switch (mapType)
	{
	case MAP_SPACE:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\map_space.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	case MAP_GRASSLANDS:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\map_grasslands.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	case MAP_MOUNTAINS:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\map_mountains.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	case MAP_BASE:
	default:
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), L"Assets\\map_base.dds", NULL, &m_gameShaderResourceView, NULL));
		break;
	}

	if ((players.at(0)->isBot() && player_sprites_loaded[0]) || (!players.at(0)->isBot() && !player_sprites_loaded[0]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(0)->isBot() ? L"Assets\\bot_black.dds" : L"Assets\\char_black.dds", NULL, &m_charBlackShaderResourceView, NULL));
	}

	if ((players.at(1)->isBot() && player_sprites_loaded[1]) || (!players.at(1)->isBot() && !player_sprites_loaded[1]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(1)->isBot() ? L"Assets\\bot_blue.dds" : L"Assets\\char_blue.dds", NULL, &m_charBlueShaderResourceView, NULL));
	}

	if ((players.at(2)->isBot() && player_sprites_loaded[2]) || (!players.at(2)->isBot() && !player_sprites_loaded[2]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(2)->isBot() ? L"Assets\\bot_green.dds" : L"Assets\\char_green.dds", NULL, &m_charGreenShaderResourceView, NULL));
	}

	if ((players.at(3)->isBot() && player_sprites_loaded[3]) || (!players.at(3)->isBot() && !player_sprites_loaded[3]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(3)->isBot() ? L"Assets\\bot_orange.dds" : L"Assets\\char_orange.dds", NULL, &m_charOrangeShaderResourceView, NULL));
	}

	if ((players.at(4)->isBot() && player_sprites_loaded[4]) || (!players.at(4)->isBot() && !player_sprites_loaded[4]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(4)->isBot() ? L"Assets\\bot_pink.dds" : L"Assets\\char_pink.dds", NULL, &m_charPinkShaderResourceView, NULL));
	}

	if ((players.at(5)->isBot() && player_sprites_loaded[5]) || (!players.at(5)->isBot() && !player_sprites_loaded[5]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(5)->isBot() ? L"Assets\\bot_red.dds" : L"Assets\\char_red.dds", NULL, &m_charRedShaderResourceView, NULL));
	}

	if ((players.at(6)->isBot() && player_sprites_loaded[6]) || (!players.at(6)->isBot() && !player_sprites_loaded[6]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(6)->isBot() ? L"Assets\\bot_white.dds" : L"Assets\\char_white.dds", NULL, &m_charWhiteShaderResourceView, NULL));
	}

	if ((players.at(7)->isBot() && player_sprites_loaded[7]) || (!players.at(7)->isBot() && !player_sprites_loaded[7]))
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(dev.Get(), players.at(7)->isBot() ? L"Assets\\bot_yellow.dds" : L"Assets\\char_yellow.dds", NULL, &m_charYellowShaderResourceView, NULL));
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

	// set our new render target object as the active render target
	devcon->OMSetRenderTargets(1, rendertarget.GetAddressOf(), nullptr);

	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(rendertarget.Get(), color);

	// clear the depth buffer
	devcon->ClearDepthStencilView(zbuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3DRenderer::beginFrame()
{
	// Empty
}

void Direct3DRenderer::renderWorldBackground()
{
	m_spriteBatcher->beginBatch();
	m_spriteBatcher->drawSprite(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, 0, Assets::getWorldBackgroundTextureRegion());
	m_spriteBatcher->endBatchWithTexture(m_gameShaderResourceView);
}

void Direct3DRenderer::renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
	m_spriteBatcher->beginBatch();

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

	m_spriteBatcher->endBatchWithTexture(m_gameShaderResourceView);
}

void Direct3DRenderer::renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
	for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
	{
		if ((**itr).getPlayerState() != Player_State::DEAD)
		{
			m_spriteBatcher->beginBatch();
			renderGameObjectWithRespectToPlayer((**itr), Assets::getPlayerTextureRegion((**itr)));

			switch ((**itr).getPlayerIndex())
			{
			case 0:
				m_spriteBatcher->endBatchWithTexture(m_charBlackShaderResourceView);
				break;
			case 1:
				m_spriteBatcher->endBatchWithTexture(m_charBlueShaderResourceView);
				break;
			case 2:
				m_spriteBatcher->endBatchWithTexture(m_charGreenShaderResourceView);
				break;
			case 3:
				m_spriteBatcher->endBatchWithTexture(m_charOrangeShaderResourceView);
				break;
			case 4:
				m_spriteBatcher->endBatchWithTexture(m_charPinkShaderResourceView);
				break;
			case 5:
				m_spriteBatcher->endBatchWithTexture(m_charRedShaderResourceView);
				break;
			case 6:
				m_spriteBatcher->endBatchWithTexture(m_charWhiteShaderResourceView);
				break;
			case 7:
				m_spriteBatcher->endBatchWithTexture(m_charYellowShaderResourceView);
				break;
			default:
				break;
			}
		}
	}

	m_spriteBatcher->beginBatch();

	for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
	{
		if ((**itr).getPlayerState() != Player_State::DEAD && (**itr).getPlayerForceFieldState() != PLAYER_FORCE_FIELD_STATE_OFF)
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getForceFieldTextureRegion((**itr).getPlayerForceFieldState(), (**itr).getPlayerForceFieldStateTime()));
		}

		if ((*itr)->isDisplayingName())
		{
			static TextureRegion tr = Assets::getPlayerNameBubbleTextureRegion();
			static float playerYModifier = GRID_CELL_HEIGHT * 7 / 8;

			std::stringstream ss;
			ss << (*itr)->getUsername();
			std::string playerNameText = ss.str();

			float increaseBubbleModifier = 0;
			int playerNameTextLength = playerNameText.length();
			if (playerNameTextLength > 6)
			{
				playerNameTextLength -= 6;
				increaseBubbleModifier = GRID_CELL_WIDTH * 2 / 7;
			}

			m_spriteBatcher->drawSprite((*itr)->getPosition().getX(), (*itr)->getPosition().getY() + playerYModifier - m_fScrollY, GRID_CELL_WIDTH * 2 + (increaseBubbleModifier * playerNameTextLength), GRID_CELL_HEIGHT / 2, 0, tr);
		}
		else if ((*itr)->isDisplayingPointer())
		{
			static TextureRegion tr = Assets::getPlayerPointerTextureRegion();
			static float playerYModifier = GRID_CELL_HEIGHT * 7 / 8;
			m_spriteBatcher->drawSprite((*itr)->getPosition().getX(), (*itr)->getPosition().getY() + playerYModifier - m_fScrollY, GRID_CELL_WIDTH / 2, GRID_CELL_HEIGHT / 2, 0, tr);
		}
	}

	m_spriteBatcher->endBatchWithTexture(m_interfaceShaderResourceView);

	m_spriteBatcherWithColor->beginBatch();

	for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
	{
		if ((*itr)->isDisplayingName())
		{
			static Color playerNameColor = Color(1, 1, 1, 1);

			std::stringstream ss;
			ss << (*itr)->getUsername();
			std::string playerNameText = ss.str();

			static float playerYModifier = GRID_CELL_HEIGHT * 7 / 8;
			static float playerNameFontGlyphWidth = GRID_CELL_WIDTH * 2 / 7;
			static float playerNameFontGlyphHeight = playerNameFontGlyphWidth * 0.68421052631579;

			m_font->renderText(*m_spriteBatcherWithColor, playerNameText, (*itr)->getPosition().getX(), (*itr)->getPosition().getY() + playerYModifier - m_fScrollY, playerNameFontGlyphWidth, playerNameFontGlyphHeight, playerNameColor, true);
		}
	}

	m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceShaderResourceView);
}

void Direct3DRenderer::renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs)
{
	m_spriteBatcher->beginBatch();
	for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
	{
		renderGameObjectWithRespectToPlayer((**itr), Assets::getBombTextureRegion((**itr)));
	}
	m_spriteBatcher->endBatchWithTexture(m_gameShaderResourceView);
}

void Direct3DRenderer::renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions)
{
	m_spriteBatcher->beginBatch();
	for (std::vector<std::unique_ptr<Explosion>>::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
	{
		for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
		{
			m_spriteBatcher->drawSprite((**itr2).getPosition().getX(), (**itr2).getPosition().getY() - m_fScrollY, (**itr2).getWidth(), (**itr2).getHeight(), (**itr2).getAngle(), Assets::getFireTextureRegion((**itr2)));
		}
	}
	m_spriteBatcher->endBatchWithTexture(m_gameShaderResourceView);
}

void Direct3DRenderer::renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear)
{
	m_spriteBatcher->beginBatch();
	for (std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBordersNear.begin(); itr != mapBordersNear.end(); itr++)
	{
		if ((*itr)->isNearFront())
		{
			renderGameObjectWithRespectToPlayer((**itr), Assets::getMapBorderTextureRegion((**itr)));
		}
	}
	m_spriteBatcher->endBatchWithTexture(m_gameShaderResourceView);
}

void Direct3DRenderer::renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface)
{
	if (waitingForServerInterface.renderPlayersList())
	{
		m_spriteBatcher->beginBatch();
		renderGameObject(waitingForServerInterface, Assets::getWaitingForServerInterfaceTextureRegion());

		for (std::vector<std::unique_ptr<PlayerRow>>::iterator itr = waitingForServerInterface.getPlayerRows().begin(); itr != waitingForServerInterface.getPlayerRows().end(); itr++)
		{
			if ((*itr)->isActive() && (*itr)->getPlayerPlatformAvatar().getPlayerPlatform() != PLATFORM_UNKNOWN)
			{
				renderGameObject((*itr)->getPlayerPlatformAvatar(), Assets::getPlayerRowPlatformAvatarTextureRegion((*itr)->getPlayerPlatformAvatar()));
			}
		}

		m_spriteBatcher->endBatchWithTexture(m_interface2ShaderResourceView);

		m_spriteBatcherWithColor->beginBatch();

		static Color playerNameColor = Color(1, 1, 1, 1);

		for (std::vector<std::unique_ptr<PlayerRow>>::iterator itr = waitingForServerInterface.getPlayerRows().begin(); itr != waitingForServerInterface.getPlayerRows().end(); itr++)
		{
			if ((*itr)->isActive())
			{
				std::stringstream ss;
				ss << (*itr)->getPlayerName();
				std::string playerName = ss.str();

				m_font->renderText(*m_spriteBatcherWithColor, playerName, (*itr)->getFontX(), (*itr)->getFontY(), (*itr)->getFontGlyphWidth(), (*itr)->getFontGlyphHeight(), playerNameColor, true);
			}
		}

		m_spriteBatcherWithColor->endBatchWithTexture(m_interface2ShaderResourceView);
	}

	if (waitingForServerInterface.renderTimeToNextRound() || waitingForServerInterface.renderMessage())
	{
		m_spriteBatcherWithColor->beginBatch();

		if (waitingForServerInterface.renderTimeToNextRound())
		{
			static Color timerColor = Color(1, 1, 1, 1);

			std::stringstream ss2;
			ss2 << waitingForServerInterface.getTimeToNextRound();
			std::string timerText = ss2.str();

			m_font->renderText(*m_spriteBatcherWithColor, timerText, SCREEN_WIDTH - 3, SCREEN_HEIGHT - 3, 2.0f, 1.36842105263158f, timerColor, true);
		}

		if (waitingForServerInterface.renderMessage())
		{
			static Color interfaceColor = Color(1, 1, 1, 1);
			interfaceColor.alpha -= 0.025f;
			if (interfaceColor.alpha < 0.2f)
			{
				interfaceColor.alpha = 1;
			}

			float fontSize = 0.5f;

			std::stringstream ss;
			switch (waitingForServerInterface.getPreGamePhase())
			{
			case CONNECTING:
				ss << "Connecting as " << waitingForServerInterface.getUsername();
				break;
			case CONNECTION_ERROR:
				ss << "There was an error connecting to Battle Bombs...";
				fontSize = 0.42f;
				break;
			case BATTLE_BOMBS_BETA_CLOSED:
				ss << "Get the non-Beta Battle Bombs on the app store!";
				fontSize = 0.42f;
				break;
			case FINDING_ROOM_TO_JOIN:
				ss << "Finding a room to join";
				break;
			case ROOM_JOINED_WAITING_FOR_SERVER:
				ss << "Waiting for next round";
				break;
			case DEFAULT:
			default:
				ss << "...";
				break;
			}

			std::string waitingText = ss.str();

			m_font->renderText(*m_spriteBatcherWithColor, waitingText, SCREEN_WIDTH / 2, 0.5f, fontSize, fontSize, interfaceColor, true);
		}

		m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceShaderResourceView);
	}
}

void Direct3DRenderer::renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface)
{
	static Color interfaceColor = Color(1, 1, 1, 1);
	interfaceColor.alpha -= 0.025f;
	if (interfaceColor.alpha < 0.2f)
	{
		interfaceColor.alpha = 1;
	}

	m_spriteBatcherWithColor->beginBatch();

	std::stringstream ss;
	ss << "Tap anywhere to play again!";
	std::string waitingText = ss.str();

	m_font->renderText(*m_spriteBatcherWithColor, waitingText, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.5f, 0.5f, interfaceColor, true);

	m_spriteBatcherWithColor->endBatchWithTexture(m_interface2ShaderResourceView);
}

void Direct3DRenderer::renderUIEffects(std::vector<std::unique_ptr<CountDownNumberGameObject>> &countDownNumbers, DisplayBattleGameObject &displayBattleGameObject, std::vector<std::unique_ptr<DisplayGameOverGameObject>> &displayGameOverGameObject)
{
	m_spriteBatcher->beginBatch();

	for (std::vector<std::unique_ptr<CountDownNumberGameObject>>::iterator itr = countDownNumbers.begin(); itr != countDownNumbers.end(); itr++)
	{
		renderGameObject((**itr), Assets::getCountDownNumberTextureRegion(**itr));
	}

	renderGameObject(displayBattleGameObject, Assets::getDisplayBattleTextureRegion());

	for (std::vector<std::unique_ptr<DisplayGameOverGameObject>>::iterator itr = displayGameOverGameObject.begin(); itr != displayGameOverGameObject.end(); itr++)
	{
		renderGameObject((**itr), Assets::getDisplayGameOverTextureRegion(**itr));
	}

	m_spriteBatcher->endBatchWithTexture(m_interface2ShaderResourceView);
}

void Direct3DRenderer::renderInterface(InterfaceOverlay &interfaceOverlay)
{
	m_spriteBatcher->beginBatch();
	m_spriteBatcher->drawSprite(INTERFACE_OVERLAY_BACKGROUND_X, INTERFACE_OVERLAY_BACKGROUND_Y, INTERFACE_OVERLAY_BACKGROUND_WIDTH, INTERFACE_OVERLAY_BACKGROUND_HEIGHT, 0, Assets::getInterfaceOverlayTextureRegion());

	for (std::vector<std::unique_ptr<PlayerAvatar>>::iterator itr = interfaceOverlay.getPlayerAvatars().begin(); itr != interfaceOverlay.getPlayerAvatars().end(); itr++)
	{
		renderGameObject((**itr), Assets::getPlayerAvatarTextureRegion((**itr)));
	}

	renderGameObject(interfaceOverlay.getDPadControl(), Assets::getDPadControlTextureRegion(interfaceOverlay.getDPadControl()));

	for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
	{
		if ((**itr).getPowerUpType() != POWER_UP_TYPE_NONE)
		{
			renderGameObject((**itr), Assets::getPowerUpBarItemTextureRegion((**itr), interfaceOverlay.getPowerUpBarItemsStateTime()));
		}
	}

	if (interfaceOverlay.getActiveButton().getPowerUpType() == POWER_UP_TYPE_PUSH)
	{
		renderGameObject(interfaceOverlay.getActiveButton(), Assets::getActiveButtonTextureRegion(interfaceOverlay.getActiveButton(), interfaceOverlay.getButtonsStateTime()));
	}

	renderGameObject(interfaceOverlay.getBombButton(), Assets::getBombButtonTextureRegion(interfaceOverlay.getBombButton(), interfaceOverlay.getButtonsStateTime()));

	m_spriteBatcher->endBatchWithTexture(m_interfaceShaderResourceView);

	std::stringstream ss;
	ss << interfaceOverlay.getNumMinutesLeft() << ":" << interfaceOverlay.getNumSecondsLeftFirstColumn() << interfaceOverlay.getNumSecondsLeftSecondColumn();
	std::string timeRemaining = ss.str();
	static Color interfaceColor = Color(1, 1, 1, 1);

	static const float timerX = 0.3554104477903f;
	static const float timerY = 13.0589552253125f;
	static const float timerWidth = 0.40298507462688f;
	static const float timerHeight = 0.425373134375f;

	m_spriteBatcherWithColor->beginBatch();
	m_font->renderText(*m_spriteBatcherWithColor, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);

	for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
	{
		if ((**itr).getPowerUpType() != POWER_UP_TYPE_NONE)
		{
			std::stringstream ss2;
			ss2 << (*itr)->getLevel();
			std::string powerUpStack = ss2.str();
			float x = (*itr)->getPosition().getX() + (*itr)->getWidth() / 2;
			float y = (*itr)->getPosition().getY() - (*itr)->getHeight() / 2;
			m_font->renderText(*m_spriteBatcherWithColor, powerUpStack, x, y, 0.36f, 0.32f, interfaceColor);
		}
	}

	m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceShaderResourceView);

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
				//m_rectangleRenderer->renderRectangle(leftX, bottomY, leftX + miniMapGridWidth, bottomY + miniMapGridHeight, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
			}
		}
	}
}

void Direct3DRenderer::renderSpectatorInterface(InterfaceOverlay &interfaceOverlay)
{
	m_spriteBatcher->beginBatch();
	m_spriteBatcher->drawSprite(INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_X, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_Y, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_WIDTH, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_HEIGHT, 0, Assets::getSpectatorInterfaceOverlayTextureRegion());
	renderGameObject(interfaceOverlay.getSpectatorControls(), Assets::getSpectatorControlsTextureRegion(interfaceOverlay.getSpectatorControls()));
	m_spriteBatcher->endBatchWithTexture(m_interfaceShaderResourceView);

	static Color interfaceColor = Color(1, 1, 1, 1 );

	m_spriteBatcherWithColor->beginBatch();

	std::stringstream ss;
	ss << interfaceOverlay.getNumMinutesLeft() << ":" << interfaceOverlay.getNumSecondsLeftFirstColumn() << interfaceOverlay.getNumSecondsLeftSecondColumn();
	std::string timeRemaining = ss.str();

	static const float timerX = 3.43731343283576f;
	static const float timerY = 0.71044776125f;
	static const float timerWidth = 0.40298507462688f;
	static const float timerHeight = 0.425373134375f;

	m_font->renderText(*m_spriteBatcherWithColor, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);

	static const float spectatingWhoX = 12.17910447761184f;
	static const float spectatingWhoY = 0.51044776125f;
	static const float spectatingWhoWidth = 0.52611940298496f;
	static const float spectatingWhoHeight = 0.680597015f;

	m_font->renderText(*m_spriteBatcherWithColor, interfaceOverlay.getSpectatingUsername(), spectatingWhoX, spectatingWhoY, spectatingWhoWidth, spectatingWhoHeight, interfaceColor, true);

	m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceShaderResourceView);

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
				//m_rectangleRenderer->renderRectangle(leftX, bottomY, leftX + miniMapGridWidth, bottomY + miniMapGridHeight, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
			}
		}
	}
}

void Direct3DRenderer::renderGameOverBlackCover(float alpha)
{
	static Color transitionCoverColor = Color(0, 0, 0, 0);
	transitionCoverColor.alpha = alpha;

	static Rectangle gameOverBlackCoverRectangle = Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderRectangleFill(gameOverBlackCoverRectangle, transitionCoverColor);
}

void Direct3DRenderer::renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
{
	m_spriteBatcher->beginBatch();
	for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
		{
			if (game_grid[j][i] == 1)
			{
				m_spriteBatcher->drawSprite(GAME_X + GRID_CELL_WIDTH * j + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * i + GRID_CELL_HEIGHT / 2.0f - m_fScrollY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, 0, Assets::getOneTextureRegion());
			}
			else
			{
				m_spriteBatcher->drawSprite(GAME_X + GRID_CELL_WIDTH * j + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * i + GRID_CELL_HEIGHT / 2.0f - m_fScrollY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, 0, Assets::getNineTextureRegion());
			}
		}
	}

	m_spriteBatcher->endBatchWithTexture(m_interfaceShaderResourceView);
}

void Direct3DRenderer::endFrame()
{
	// Not Needed Yet
}

void Direct3DRenderer::cleanUp()
{
	// Empty
}

// Private

void Direct3DRenderer::renderGameObject(GameObject &go, TextureRegion tr)
{
	m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Direct3DRenderer::renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr)
{
	if (m_fScrollY > 0)
	{
		m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY() - m_fScrollY, go.getWidth(), go.getHeight(), go.getAngle(), tr);
	}
	else
	{
		renderGameObject(go, tr);
	}
}

void Direct3DRenderer::renderLine(Line &line, Color &color)
{
	/*Vector2D origin = line.getOrigin();
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

	m_primitiveBatch->End();*/
}

void Direct3DRenderer::renderRectangleStroke(Rectangle &rectangle, Color &color)
{
	/*Vector2D lowerLeft = rectangle.getLowerLeft();
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

	m_primitiveBatch->End();*/
}

void Direct3DRenderer::renderRectangleFill(Rectangle &rectangle, Color &color)
{
	/*Vector2D lowerLeft = rectangle.getLowerLeft();
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

	m_primitiveBatch->End();*/
}