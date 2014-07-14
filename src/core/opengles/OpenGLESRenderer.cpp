//
//  OpenGLESRenderer.cpp
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "OpenGLESRenderer.h"
#include "SpriteBatcher.h"
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
#include "PlayerAvatar.h"
#include "Font.h"

#include <sstream>

extern "C"
{
#include "asset_utils.h"
}

OpenGLESRenderer::OpenGLESRenderer(int width, int height) : Renderer()
{
    glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
    
	glLoadIdentity();
    
	glMatrixMode(GL_PROJECTION);
	glOrthof(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
	glMatrixMode(GL_MODELVIEW);
    
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    m_spriteBatcher = std::unique_ptr<SpriteBatcher>(new SpriteBatcher(4000, false));
    m_spriteBatcherWithColor = std::unique_ptr<SpriteBatcher>(new SpriteBatcher(1000, true));
    
    // TODO, this gameTexture loading is temporary
    m_gameTexture = load_png_asset_into_texture("map_space.png");
    
    m_interfaceTexture = load_png_asset_into_texture("interface.png");
    
    m_charBlackTexture = load_png_asset_into_texture("char_black.png");
    m_charBlueTexture = load_png_asset_into_texture("char_blue.png");
    m_charGreenTexture = load_png_asset_into_texture("char_green.png");
    m_charOrangeTexture = load_png_asset_into_texture("char_orange.png");
    m_charPinkTexture = load_png_asset_into_texture("char_pink.png");
    m_charRedTexture = load_png_asset_into_texture("char_red.png");
    m_charWhiteTexture = load_png_asset_into_texture("char_white.png");
    m_charYellowTexture = load_png_asset_into_texture("char_yellow.png");
}

OpenGLESRenderer::~OpenGLESRenderer()
{
    cleanUp();
}

void OpenGLESRenderer::loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    glDeleteTextures(1, &m_gameTexture);
    
    switch (mapType)
    {
        case MAP_SPACE:
            m_gameTexture = load_png_asset_into_texture("map_space.png");
            break;
        case MAP_GRASSLANDS:
            m_gameTexture = load_png_asset_into_texture("map_grasslands.png");
            break;
        case MAP_MOUNTAINS:
            m_gameTexture = load_png_asset_into_texture("map_mountains.png");
            break;
        case MAP_BASE:
        default:
            m_gameTexture = load_png_asset_into_texture("map_base.png");
            break;
    }
    
    // TODO, optimize this so that only textures that NEED to change are deleted
    
    glDeleteTextures(1, &m_charBlackTexture);
    glDeleteTextures(1, &m_charBlueTexture);
    glDeleteTextures(1, &m_charGreenTexture);
    glDeleteTextures(1, &m_charOrangeTexture);
    glDeleteTextures(1, &m_charPinkTexture);
    glDeleteTextures(1, &m_charRedTexture);
    glDeleteTextures(1, &m_charWhiteTexture);
    glDeleteTextures(1, &m_charYellowTexture);
    
    m_charBlackTexture = load_png_asset_into_texture(players.at(0)->isBot() ? "bot_black.png" : "char_black.png");
    m_charBlueTexture = load_png_asset_into_texture(players.at(1)->isBot() ? "bot_blue.png" : "char_blue.png");
    m_charGreenTexture = load_png_asset_into_texture(players.at(2)->isBot() ? "bot_green.png" : "char_green.png");
    m_charOrangeTexture = load_png_asset_into_texture(players.at(3)->isBot() ? "bot_orange.png" : "char_orange.png");
    m_charPinkTexture = load_png_asset_into_texture(players.at(4)->isBot() ? "bot_pink.png" : "char_pink.png");
    m_charRedTexture = load_png_asset_into_texture(players.at(5)->isBot() ? "bot_red.png" : "char_red.png");
    m_charWhiteTexture = load_png_asset_into_texture(players.at(6)->isBot() ? "bot_white.png" : "char_white.png");
    m_charYellowTexture = load_png_asset_into_texture(players.at(7)->isBot() ? "bot_yellow.png" : "char_yellow.png");
}

void OpenGLESRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLESRenderer::renderWorldBackground()
{
    glEnable(GL_TEXTURE_2D);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, 0, Assets::getWorldBackgroundTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if(insideBlocks.size() > 0)
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
        
        m_spriteBatcher->endBatchWithTexture(m_gameTexture);
    }
}

void OpenGLESRenderer::renderPlayers(std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
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
                    m_spriteBatcher->endBatchWithTexture(m_charBlackTexture);
                    break;
                case 1:
                    m_spriteBatcher->endBatchWithTexture(m_charBlueTexture);
                    break;
                case 2:
                    m_spriteBatcher->endBatchWithTexture(m_charGreenTexture);
                    break;
                case 3:
                    m_spriteBatcher->endBatchWithTexture(m_charOrangeTexture);
                    break;
                case 4:
                    m_spriteBatcher->endBatchWithTexture(m_charPinkTexture);
                    break;
                case 5:
                    m_spriteBatcher->endBatchWithTexture(m_charRedTexture);
                    break;
                case 6:
                    m_spriteBatcher->endBatchWithTexture(m_charWhiteTexture);
                    break;
                case 7:
                    m_spriteBatcher->endBatchWithTexture(m_charYellowTexture);
                    break;
                default:
                    break;
            }
        }
    }
}

void OpenGLESRenderer::renderBombs(std::vector<std::unique_ptr<BombGameObject>> &bombs)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<BombGameObject>>::iterator itr = bombs.begin(); itr != bombs.end(); itr++)
    {
        renderGameObjectWithRespectToPlayer((**itr), Assets::getBombTextureRegion((**itr)));
    }
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderExplosions(std::vector<std::unique_ptr<Explosion>> &explosions)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Explosion>>::iterator itr = explosions.begin(); itr != explosions.end(); itr++)
    {
        for (std::vector<std::unique_ptr<Fire>>::iterator itr2 = (*itr)->getFireParts().begin(); itr2 != (*itr)->getFireParts().end(); itr2++)
        {
            renderGameObjectWithRespectToPlayer((**itr2), Assets::getFireTextureRegion((**itr2)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderMapBordersNear(std::vector<std::unique_ptr<MapBorder>> &mapBordersNear)
{
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<MapBorder>>::iterator itr = mapBordersNear.begin(); itr != mapBordersNear.end(); itr++)
    {
        if((*itr)->isNearFront())
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getMapBorderTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderInterface(InterfaceOverlay &interfaceOverlay)
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
        if((**itr).getPowerUpType() != NONE)
        {
            renderGameObject((**itr), Assets::getPowerUpBarItemTextureRegion((**itr), interfaceOverlay.getPowerUpBarItemsStateTime()));
        }
    }
    
    if(interfaceOverlay.getActiveButton().getPowerUpType() == PUSH)
    {
        renderGameObject(interfaceOverlay.getActiveButton(), Assets::getActiveButtonTextureRegion(interfaceOverlay.getActiveButton(), interfaceOverlay.getButtonsStateTime()));
    }
    
    renderGameObject(interfaceOverlay.getBombButton(), Assets::getBombButtonTextureRegion(interfaceOverlay.getBombButton(), interfaceOverlay.getButtonsStateTime()));
    
    m_spriteBatcher->endBatchWithTexture(m_interfaceTexture);
    
    std::stringstream ss;
    ss << interfaceOverlay.getNumMinutesLeft() << ":" << interfaceOverlay.getNumSecondsLeftFirstColumn() << interfaceOverlay.getNumSecondsLeftSecondColumn();
    std::string timeRemaining = ss.str();
    static Color interfaceColor = { 1, 1, 1, 1 };
    
    m_spriteBatcherWithColor->beginBatch();
    m_font->renderText(*m_spriteBatcherWithColor, timeRemaining, 0.3554104477903f, 12.9738805984375f, 0.40298507462688f, 0.425373134375f, interfaceColor);
    
    for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
    {
        if((**itr).getPowerUpType() != NONE)
        {
            std::stringstream ss2;
            ss2 << (*itr)->getLevel();
            std::string powerUpStack = ss2.str();
            float x = (*itr)->getPosition().getX() + (*itr)->getWidth() / 2;
            float y = (*itr)->getPosition().getY() - (*itr)->getHeight() / 2;
            m_font->renderText(*m_spriteBatcherWithColor, powerUpStack, x, y, 0.36f, 0.32f, interfaceColor);
        }
    }
    
    m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceTexture);
}

void OpenGLESRenderer::renderGameGrid(int game_grid[NUM_GRID_CELLS_PER_ROW][GRID_CELL_NUM_ROWS])
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
    
    m_spriteBatcher->endBatchWithTexture(m_interfaceTexture);
}

void OpenGLESRenderer::endFrame()
{
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::cleanUp()
{
    glDeleteTextures(1, &m_gameTexture);
    glDeleteTextures(1, &m_interfaceTexture);
    glDeleteTextures(1, &m_charBlackTexture);
    glDeleteTextures(1, &m_charBlueTexture);
    glDeleteTextures(1, &m_charGreenTexture);
    glDeleteTextures(1, &m_charOrangeTexture);
    glDeleteTextures(1, &m_charPinkTexture);
    glDeleteTextures(1, &m_charRedTexture);
    glDeleteTextures(1, &m_charWhiteTexture);
    glDeleteTextures(1, &m_charYellowTexture);
}

#pragma mark <Private>

void OpenGLESRenderer::renderGameObject(GameObject &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void OpenGLESRenderer::renderGameObjectWithRespectToPlayer(GameObject &go, TextureRegion tr)
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