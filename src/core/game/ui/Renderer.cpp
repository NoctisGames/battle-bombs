//
//  Renderer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Renderer.h"
#include "GameConstants.h"
#include "ResourceConstants.h"
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "DPadControl.h"
#include "ActiveButton.h"
#include "PlayerDynamicGameObject.h"
#include "MapBorder.h"
#include "SpaceTile.h"
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
#include "PlayerAvatar.h"
#include "Font.h"
#include "MiniMapGridType.h"
#include "Rectangle.h"
#include "PlayerForceFieldState.h"
#include "SpectatorControls.h"
#include "PlayerRow.h"
#include "PlayerRowAvatar.h"
#include "PlayerRowPlatformAvatar.h"
#include "CountDownNumberGameObject.h"
#include "DisplayXMovingGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include <sstream>

Renderer::Renderer()
{
	m_font = std::unique_ptr<Font>(new Font(FONT_TEXTURE_REGION_X, FONT_TEXTURE_REGION_Y, FONT_GLYPHS_PER_ROW, FONT_GLYPH_WIDTH, FONT_GLYPH_HEIGHT, TEXTURE_SIZE_1024x1024, TEXTURE_SIZE_1024x1024));
    
    player_sprites_loaded[0] = true;
    player_sprites_loaded[1] = false;
    player_sprites_loaded[2] = false;
    player_sprites_loaded[3] = false;
    player_sprites_loaded[4] = false;
    player_sprites_loaded[5] = false;
    player_sprites_loaded[6] = false;
    player_sprites_loaded[7] = false;
    
    m_fScrollY = 0;
}

void Renderer::calcScrollYForPlayer(PlayerDynamicGameObject &player)
{
	if (player.getPosition().getY() > (SCREEN_HEIGHT / 2))
	{
		m_fScrollY = player.getPosition().getY() - (SCREEN_HEIGHT / 2);
		if (m_fScrollY > (SCREEN_HEIGHT + GRID_CELL_HEIGHT))
		{
			m_fScrollY = SCREEN_HEIGHT + GRID_CELL_HEIGHT;
		}
	}
	else
	{
		m_fScrollY = 0;
	}
}

void Renderer::renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface, bool renderPlayersList, bool renderMessage)
{
    if(renderPlayersList)
    {
        m_spriteBatcher->beginBatch();
        renderGameObject(waitingForServerInterface, Assets::getWaitingForServerInterfaceTextureRegion());
        
        for (std::vector<std::unique_ptr<PlayerRow>>::iterator itr = waitingForServerInterface.getPlayerRows().begin(); itr != waitingForServerInterface.getPlayerRows().end(); itr++)
        {
            renderGameObject((*itr)->getPlayerRowAvatar(), Assets::getPlayerRowAvatarTextureRegion((*itr)->getPlayerRowAvatar()));
            
            if ((*itr)->isActive() && (*itr)->getPlayerPlatformAvatar().getPlayerPlatform() != PLATFORM_UNKNOWN)
            {
                renderGameObject((*itr)->getPlayerPlatformAvatar(), Assets::getPlayerRowPlatformAvatarTextureRegion((*itr)->getPlayerPlatformAvatar()));
            }
        }
        
        m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture2);
        
        m_spriteBatcher->beginBatch();
        
        static Color playerNameColor = Color(1, 1, 1, 1);
        
        for (std::vector<std::unique_ptr<PlayerRow>>::iterator itr = waitingForServerInterface.getPlayerRows().begin(); itr != waitingForServerInterface.getPlayerRows().end(); itr++)
        {
            std::stringstream ss;
            ss << ((*itr)->isActive() ? (*itr)->getPlayerName() : "Bot");
            std::string playerName = ss.str();
            
            m_font->renderText(*m_spriteBatcher, playerName, (*itr)->getFontX(), (*itr)->getFontY(), (*itr)->getFontGlyphWidth(), (*itr)->getFontGlyphHeight(), playerNameColor);
        }
        
        m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture2);
    }
    
    if(waitingForServerInterface.renderTimeToNextRound() || renderMessage)
    {
        m_spriteBatcher->beginBatch();
        
        if(waitingForServerInterface.renderTimeToNextRound())
        {
            static Color timerColor = Color(1, 1, 1, 1);
            
            std::stringstream ss2;
            ss2 << waitingForServerInterface.getTimeToNextRound();
            std::string timerText = ss2.str();
            
            m_font->renderText(*m_spriteBatcher, timerText, SCREEN_WIDTH - 3, SCREEN_HEIGHT - 3, 2.0f, 1.36842105263158f, timerColor, true);
        }
        
        if(renderMessage)
        {
            static Color interfaceColor = Color(1, 1, 1, 1);
            interfaceColor.alpha -= 0.025f;
            if(interfaceColor.alpha < 0.2f)
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
            
            m_font->renderText(*m_spriteBatcher, waitingText, SCREEN_WIDTH / 2, 0.5f, fontSize, fontSize, interfaceColor, true);
        }
        
        m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    }
}

void Renderer::renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface)
{
    static Color interfaceColor = Color(1, 1, 1, 1);
    interfaceColor.alpha -= 0.025f;
    if(interfaceColor.alpha < 0.2f)
    {
        interfaceColor.alpha = 1;
    }
    
    m_spriteBatcher->beginBatch();
    
    std::stringstream ss;
    ss << "Tap anywhere to play again!";
    std::string waitingText = ss.str();
    
    m_font->renderText(*m_spriteBatcher, waitingText, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.5f, 0.5f, interfaceColor, true);
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
}

void Renderer::renderWorldBackground()
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, 0, Assets::getWorldBackgroundTextureRegion());
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderSpaceTiles(std::vector<std::unique_ptr<SpaceTile>> &spaceTiles)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<SpaceTile>>::iterator itr = spaceTiles.begin(); itr != spaceTiles.end(); itr++)
    {
        if((*itr)->getSpaceTileState() == ST_NORMAL)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getSpaceTileTextureRegion(**itr));
        }
    }
    
    for (std::vector<std::unique_ptr<SpaceTile>>::iterator itr = spaceTiles.begin(); itr != spaceTiles.end(); itr++)
    {
        if((*itr)->getSpaceTileState() != ST_NORMAL)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getSpaceTileTextureRegion(**itr));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderCraters(std::vector<std::unique_ptr<Crater>> &craters)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Crater>>::iterator itr = craters.begin(); itr != craters.end(); itr++)
    {
        renderGameObjectWithRespectToPlayer((**itr), Assets::getCraterTextureRegion());
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBordersFar.begin(); itr != mapBordersFar.end(); itr++)
    {
        if(!(*itr)->isNearFront())
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getMapBorderTextureRegion((**itr)));
        }
    }
    
    for (std::vector<std::unique_ptr<InsideBlock>>::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if((*itr)->getInsideBlockState() != IB_GONE)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getInsideBlockTextureRegion((**itr)));
        }
    }
    
    for (std::vector<std::unique_ptr<BreakableBlock>>::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if((*itr)->getBreakableBlockState() != DESTROYED)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getBreakableBlockTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<PowerUp>>::iterator itr = powerUps.begin(); itr != powerUps.end(); itr++)
    {
        if((*itr)->getType() != POWER_UP_TYPE_NONE)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getPowerUpTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_gameTexture);
}

void Renderer::renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        renderGameObjectWithRespectToPlayer((**itr), Assets::getBombTextureRegion((**itr)));
    }
    m_spriteBatcher->endBatchWithTexture(*m_gameTexture);
}

void Renderer::renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Explosion>>::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
    {
        for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
        {
            renderGameObjectWithRespectToPlayer((**itr2), Assets::getFireTextureRegion((**itr2)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_gameTexture);
}

void Renderer::renderSuddenDeathMountainsIcePatches(std::vector<std::unique_ptr<IcePatch>> &icePatches)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<IcePatch>>::iterator itr = icePatches.begin(); itr != icePatches.end(); itr++)
    {
        renderGameObjectWithRespectToPlayer((**itr), Assets::getIcePatchTextureRegion((**itr)));
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((**itr).getPlayerState() != Player_State::DEAD)
        {
            m_spriteBatcher->beginBatch();
            renderGameObjectWithRespectToPlayer((**itr), Assets::getPlayerTextureRegion((**itr)));
            
            switch ((**itr).getPlayerIndex())
            {
                case 0:
                    m_spriteBatcher->endBatchWithTexture(*m_charBlackTexture);
                    break;
                case 1:
                    m_spriteBatcher->endBatchWithTexture(*m_charBlueTexture);
                    break;
                case 2:
                    m_spriteBatcher->endBatchWithTexture(*m_charGreenTexture);
                    break;
                case 3:
                    m_spriteBatcher->endBatchWithTexture(*m_charOrangeTexture);
                    break;
                case 4:
                    m_spriteBatcher->endBatchWithTexture(*m_charPinkTexture);
                    break;
                case 5:
                    m_spriteBatcher->endBatchWithTexture(*m_charRedTexture);
                    break;
                case 6:
                    m_spriteBatcher->endBatchWithTexture(*m_charWhiteTexture);
                    break;
                case 7:
                    m_spriteBatcher->endBatchWithTexture(*m_charYellowTexture);
                    break;
                default:
                    break;
            }
        }
    }
}

void Renderer::renderSuddenDeathGrasslandsFireBalls(std::vector<std::unique_ptr<FireBall>> &fireBalls)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<FireBall>>::iterator itr = fireBalls.begin(); itr != fireBalls.end(); itr++)
    {
        if((*itr)->isVisible())
        {
            renderGameObjectWithRespectToPlayer((*itr)->getShadow(), Assets::getFallingObjectShadowTextureRegion((*itr)->getShadow()));
        }
    }
    
    for (std::vector<std::unique_ptr<FireBall>>::iterator itr = fireBalls.begin(); itr != fireBalls.end(); itr++)
    {
        if((*itr)->isDescending() || (*itr)->isTargetReached())
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getFireBallTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderSuddenDeathMountainsIceBalls(std::vector<std::unique_ptr<IceBall>> &iceBalls)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<IceBall>>::iterator itr = iceBalls.begin(); itr != iceBalls.end(); itr++)
    {
        if((*itr)->isVisible())
        {
            renderGameObjectWithRespectToPlayer((*itr)->getShadow(), Assets::getFallingObjectShadowTextureRegion((*itr)->getShadow()));
        }
    }
    
    for (std::vector<std::unique_ptr<IceBall>>::iterator itr = iceBalls.begin(); itr != iceBalls.end(); itr++)
    {
        if((*itr)->isDescending())
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getIceBallTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBordersNear.begin(); itr != mapBordersNear.end(); itr++)
    {
        if((*itr)->isNearFront())
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getMapBorderTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(*m_mapTexture);
}

void Renderer::renderUIEffects(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, std::vector<std::unique_ptr<CountDownNumberGameObject>> &countDownNumbers, DisplayXMovingGameObject &displayXMovingGameObject, std::vector<std::unique_ptr<DisplayGameOverGameObject>> &displayGameOverGameObject)
{
    m_spriteBatcher->beginBatch();
    
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((**itr).getPlayerState() != Player_State::DEAD && (**itr).getPlayerForceFieldState() != PLAYER_FORCE_FIELD_STATE_OFF)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getForceFieldTextureRegion((**itr).getPlayerForceFieldState(), (**itr).getPlayerForceFieldStateTime()));
        }
        
        if((*itr)->isDisplayingName())
        {
            static TextureRegion tr = Assets::getPlayerNameBubbleTextureRegion();
            static float playerYModifier = GRID_CELL_HEIGHT * 7 / 8;
            
            std::stringstream ss;
            ss << (*itr)->getUsername();
            std::string playerNameText = ss.str();
            
            float increaseBubbleModifier = 0;
            unsigned long playerNameTextLength = playerNameText.length();
            if(playerNameTextLength > 6)
            {
                playerNameTextLength -= 6;
                increaseBubbleModifier = GRID_CELL_WIDTH * 2 / 7;
            }
            
            m_spriteBatcher->drawSprite((*itr)->getPosition().getX(), (*itr)->getPosition().getY() + playerYModifier - m_fScrollY, GRID_CELL_WIDTH * 2 + (increaseBubbleModifier * playerNameTextLength), GRID_CELL_HEIGHT / 2, 0, tr);
        }
        else if((*itr)->isDisplayingPointer())
        {
            static TextureRegion tr = Assets::getPlayerPointerTextureRegion();
            static float playerYModifier = GRID_CELL_HEIGHT * 7 / 8;
            m_spriteBatcher->drawSprite((*itr)->getPosition().getX(), (*itr)->getPosition().getY() + playerYModifier - m_fScrollY, GRID_CELL_WIDTH / 2, GRID_CELL_HEIGHT / 2, 0, tr);
        }
    }
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    
    m_spriteBatcher->beginBatch();
    
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((*itr)->isDisplayingName())
        {
			static Color playerNameColor = Color(1, 1, 1, 1);
            
            std::stringstream ss;
            ss << (*itr)->getUsername();
            std::string playerNameText = ss.str();
            
            static float playerYModifier = GRID_CELL_HEIGHT * 7 / 8;
            static float playerNameFontGlyphWidth = GRID_CELL_WIDTH * 2 / 7;
            static float playerNameFontGlyphHeight = playerNameFontGlyphWidth * 0.68421052631579;
            
            m_font->renderText(*m_spriteBatcher, playerNameText, (*itr)->getPosition().getX(), (*itr)->getPosition().getY() + playerYModifier - m_fScrollY, playerNameFontGlyphWidth, playerNameFontGlyphHeight, playerNameColor, true);
        }
    }
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    
    m_spriteBatcher->beginBatch();
    
    for (std::vector<std::unique_ptr<CountDownNumberGameObject>>::iterator itr = countDownNumbers.begin(); itr != countDownNumbers.end(); itr++)
    {
        renderGameObject((**itr), Assets::getCountDownNumberTextureRegion(**itr));
    }
    
    renderGameObject(displayXMovingGameObject, Assets::getDisplayXMovingTextureRegion(displayXMovingGameObject));
    
    for (std::vector<std::unique_ptr<DisplayGameOverGameObject>>::iterator itr = displayGameOverGameObject.begin(); itr != displayGameOverGameObject.end(); itr++)
    {
        renderGameObject((**itr), Assets::getDisplayGameOverTextureRegion(**itr));
    }
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture2);
}

void Renderer::renderInterface(InterfaceOverlay &interfaceOverlay)
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
        if((**itr).getPowerUpType() != POWER_UP_TYPE_NONE)
        {
            renderGameObject((**itr), Assets::getPowerUpBarItemTextureRegion((**itr), interfaceOverlay.getPowerUpBarItemsStateTime()));
        }
    }
    
    if(interfaceOverlay.getActiveButton().getPowerUpType() != POWER_UP_TYPE_NONE)
    {
        renderGameObject(interfaceOverlay.getActiveButton(), Assets::getActiveButtonTextureRegion(interfaceOverlay.getActiveButton(), interfaceOverlay.getButtonsStateTime()));
    }
    
    renderGameObject(interfaceOverlay.getBombButton(), Assets::getBombButtonTextureRegion(interfaceOverlay.getBombButton(), interfaceOverlay.getButtonsStateTime()));
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    
    std::stringstream ss;
    ss << interfaceOverlay.getNumMinutesLeft() << ":" << interfaceOverlay.getNumSecondsLeftFirstColumn() << interfaceOverlay.getNumSecondsLeftSecondColumn();
    std::string timeRemaining = ss.str();
	static Color interfaceColor = Color(1, 1, 1, 1);
    
    static const float timerX = 0.3554104477903f;
    static const float timerY = 13.0589552253125f;
    static const float timerWidth = 0.40298507462688f;
    static const float timerHeight = 0.425373134375f;
    
    m_spriteBatcher->beginBatch();
    m_font->renderText(*m_spriteBatcher, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);
    
    for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
    {
        if((**itr).getPowerUpType() != POWER_UP_TYPE_NONE)
        {
            std::stringstream ss2;
            ss2 << (*itr)->getLevel();
            std::string powerUpStack = ss2.str();
            float x = (*itr)->getPosition().getX() + (*itr)->getWidth() / 2;
            float y = (*itr)->getPosition().getY() - (*itr)->getHeight() / 2;
            m_font->renderText(*m_spriteBatcher, powerUpStack, x, y, 0.36f, 0.32f, interfaceColor);
        }
    }
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    
    m_rectangleBatcher->beginBatch();
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            int miniMapGridType = interfaceOverlay.getMiniMapGridType(j, i);
            if(miniMapGridType != MINI_MAP_FREE_SPACE)
            {
                static const float miniMapLeftX = 0.35820895522392f;
                static const float miniMapBottomY = 10.464179105625f;
                static const float miniMapGridWidth = 0.08955223880597f;
                static const float miniMapGridHeight = 0.085074626875f;
                
                float leftX = miniMapLeftX + miniMapGridWidth * j;
                float bottomY = miniMapBottomY + miniMapGridHeight * i;
                m_rectangleBatcher->renderRectangle(leftX, bottomY, leftX + miniMapGridWidth, bottomY + miniMapGridHeight, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
            }
        }
    }
    m_rectangleBatcher->endBatch();
}

void Renderer::renderSpectatorInterface(InterfaceOverlay &interfaceOverlay)
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_X, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_Y, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_WIDTH, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_HEIGHT, 0, Assets::getSpectatorInterfaceOverlayTextureRegion());
    renderGameObject(interfaceOverlay.getSpectatorControls(), Assets::getSpectatorControlsTextureRegion(interfaceOverlay.getSpectatorControls()));
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    
	static Color interfaceColor = Color(1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    
    std::stringstream ss;
    ss << interfaceOverlay.getNumMinutesLeft() << ":" << interfaceOverlay.getNumSecondsLeftFirstColumn() << interfaceOverlay.getNumSecondsLeftSecondColumn();
    std::string timeRemaining = ss.str();
    
    static const float timerX = 3.43731343283576f;
    static const float timerY = 0.71044776125f;
    static const float timerWidth = 0.40298507462688f;
    static const float timerHeight = 0.425373134375f;
    
    m_font->renderText(*m_spriteBatcher, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);
    
    static const float spectatingWhoX = 12.17910447761184f;
    static const float spectatingWhoY = 0.51044776125f;
    static const float spectatingWhoWidth = 0.52611940298496f;
    static const float spectatingWhoHeight = 0.680597015f;
    
    m_font->renderText(*m_spriteBatcher, interfaceOverlay.getSpectatingUsername(), spectatingWhoX, spectatingWhoY, spectatingWhoWidth, spectatingWhoHeight, interfaceColor, true);
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
    
    m_rectangleBatcher->beginBatch();
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            int miniMapGridType = interfaceOverlay.getMiniMapGridType(j, i);
            if(miniMapGridType != MINI_MAP_FREE_SPACE)
            {
                static const float miniMapLeftX = 0.7611940298508f;
                static const float miniMapBottomY = 0.085074626875f;
                static const float miniMapGridWidth = 0.08955223880597f;
                static const float miniMapGridHeight = 0.085074626875f;
                
                float leftX = miniMapLeftX + miniMapGridWidth * j;
                float bottomY = miniMapBottomY + miniMapGridHeight * i;
                m_rectangleBatcher->renderRectangle(leftX, bottomY, leftX + miniMapGridWidth, bottomY + miniMapGridHeight, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
            }
        }
    }
    m_rectangleBatcher->endBatch();
}

void Renderer::renderGameOverBlackCover(float alpha)
{
	static Color transitionCoverColor = Color(0, 0, 0, 0);
    transitionCoverColor.alpha = alpha;
    
    m_rectangleBatcher->beginBatch();
    m_rectangleBatcher->renderRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, transitionCoverColor);
    m_rectangleBatcher->endBatch();
}

void Renderer::renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
{
    m_spriteBatcher->beginBatch();
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            if(game_grid[j][i] == 1)
            {
                m_spriteBatcher->drawSprite(GAME_X + GRID_CELL_WIDTH * j + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * i + GRID_CELL_HEIGHT / 2.0f - m_fScrollY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, 0, Assets::getOneTextureRegion());
            }
            else
            {
                m_spriteBatcher->drawSprite(GAME_X + GRID_CELL_WIDTH * j + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * i + GRID_CELL_HEIGHT / 2.0f - m_fScrollY, GRID_CELL_WIDTH, GRID_CELL_HEIGHT, 0, Assets::getNineTextureRegion());
            }
        }
    }
    
    m_spriteBatcher->endBatchWithTexture(*m_interfaceTexture);
}

void Renderer::renderGameObject(GameObject &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Renderer::renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr)
{
    if(m_fScrollY > 0)
    {
        m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY() - m_fScrollY, go.getWidth(), go.getHeight(), go.getAngle(), tr);
    }
    else
    {
        renderGameObject(go, tr);
    }
}

void Renderer::updatePlayerSpritesLoadedArray(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    player_sprites_loaded[0] = !players.at(0)->isBot();
    player_sprites_loaded[1] = !players.at(1)->isBot();
    player_sprites_loaded[2] = !players.at(2)->isBot();
    player_sprites_loaded[3] = !players.at(3)->isBot();
    player_sprites_loaded[4] = !players.at(4)->isBot();
    player_sprites_loaded[5] = !players.at(5)->isBot();
    player_sprites_loaded[6] = !players.at(6)->isBot();
    player_sprites_loaded[7] = !players.at(7)->isBot();
}