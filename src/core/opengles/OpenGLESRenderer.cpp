//
//  OpenGLESRenderer.cpp
//  battlebombs
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
#include "WaitingForServerInterface.h"
#include "InterfaceOverlay.h"
#include "PowerUpBarItem.h"
#include "BombButton.h"
#include "PlayerAvatar.h"
#include "Font.h"
#include "OpenGLESRectangleRenderer.h"
#include "MiniMapGridType.h"
#include "Rectangle.h"
#include "Vertices2D.h"
#include "PlayerForceFieldState.h"
#include "SpectatorControls.h"

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
    m_rectangleRenderer = std::unique_ptr<OpenGLESRectangleRenderer>(new OpenGLESRectangleRenderer(true, true));
    
    m_interfaceTexture = load_png_asset_into_texture("interface.png");
    m_interfaceTexture2 = load_png_asset_into_texture("interface_2.png");
    
    m_charBlackTexture = load_png_asset_into_texture("char_black.png");
    m_charBlueTexture = load_png_asset_into_texture("bot_blue.png");
    m_charGreenTexture = load_png_asset_into_texture("bot_green.png");
    m_charOrangeTexture = load_png_asset_into_texture("bot_orange.png");
    m_charPinkTexture = load_png_asset_into_texture("bot_pink.png");
    m_charRedTexture = load_png_asset_into_texture("bot_red.png");
    m_charWhiteTexture = load_png_asset_into_texture("bot_white.png");
    m_charYellowTexture = load_png_asset_into_texture("bot_yellow.png");
    
    player_sprites_loaded[0] = true;
    player_sprites_loaded[1] = false;
    player_sprites_loaded[2] = false;
    player_sprites_loaded[3] = false;
    player_sprites_loaded[4] = false;
    player_sprites_loaded[5] = false;
    player_sprites_loaded[6] = false;
    player_sprites_loaded[7] = false;
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
    
    if((players.at(0)->isBot() && player_sprites_loaded[0]) || (!players.at(0)->isBot() && !player_sprites_loaded[0]))
    {
        glDeleteTextures(1, &m_charBlackTexture);
        m_charBlackTexture = load_png_asset_into_texture(players.at(0)->isBot() ? "bot_black.png" : "char_black.png");
    }
    
    if((players.at(1)->isBot() && player_sprites_loaded[1]) || (!players.at(1)->isBot() && !player_sprites_loaded[1]))
    {
        glDeleteTextures(1, &m_charBlueTexture);
        m_charBlueTexture = load_png_asset_into_texture(players.at(1)->isBot() ? "bot_blue.png" : "char_blue.png");
    }
    
    if((players.at(2)->isBot() && player_sprites_loaded[2]) || (!players.at(2)->isBot() && !player_sprites_loaded[2]))
    {
        glDeleteTextures(1, &m_charGreenTexture);
        m_charGreenTexture = load_png_asset_into_texture(players.at(2)->isBot() ? "bot_green.png" : "char_green.png");
    }
    
    if((players.at(3)->isBot() && player_sprites_loaded[3]) || (!players.at(3)->isBot() && !player_sprites_loaded[3]))
    {
        glDeleteTextures(1, &m_charOrangeTexture);
        m_charOrangeTexture = load_png_asset_into_texture(players.at(3)->isBot() ? "bot_orange.png" : "char_orange.png");
    }
    
    if((players.at(4)->isBot() && player_sprites_loaded[4]) || (!players.at(4)->isBot() && !player_sprites_loaded[4]))
    {
        glDeleteTextures(1, &m_charPinkTexture);
        m_charPinkTexture = load_png_asset_into_texture(players.at(4)->isBot() ? "bot_pink.png" : "char_pink.png");
    }
    
    if((players.at(5)->isBot() && player_sprites_loaded[5]) || (!players.at(5)->isBot() && !player_sprites_loaded[5]))
    {
        glDeleteTextures(1, &m_charRedTexture);
        m_charRedTexture = load_png_asset_into_texture(players.at(5)->isBot() ? "bot_red.png" : "char_red.png");
    }
    
    if((players.at(6)->isBot() && player_sprites_loaded[6]) || (!players.at(6)->isBot() && !player_sprites_loaded[6]))
    {
        glDeleteTextures(1, &m_charWhiteTexture);
        m_charWhiteTexture = load_png_asset_into_texture(players.at(6)->isBot() ? "bot_white.png" : "char_white.png");
    }
    
    if((players.at(7)->isBot() && player_sprites_loaded[7]) || (!players.at(7)->isBot() && !player_sprites_loaded[7]))
    {
        glDeleteTextures(1, &m_charYellowTexture);
        m_charYellowTexture = load_png_asset_into_texture(players.at(7)->isBot() ? "bot_yellow.png" : "char_yellow.png");
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

void OpenGLESRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLESRenderer::beginFrame()
{
    glEnable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::renderWorldBackground()
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, 0, Assets::getWorldBackgroundTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    
    m_spriteBatcher->beginBatch();
    
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((**itr).getPlayerState() != Player_State::DEAD && (**itr).getPlayerForceFieldState() != PLAYER_FORCE_FIELD_STATE_OFF)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getForceFieldTextureRegion((**itr).getPlayerForceFieldState(), (**itr).getPlayerForceFieldStateTime()));
        }
    }
    
    m_spriteBatcher->endBatchWithTexture(m_interfaceTexture);
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

void OpenGLESRenderer::renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_spriteBatcher->beginBatch();
    renderGameObject(waitingForServerInterface, Assets::getWaitingForServerInterfaceTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_interfaceTexture2);
    
    m_spriteBatcherWithColor->beginBatch();
    
    if(waitingForServerInterface.getTimeToNextRound() > 0)
    {
        static Color timerColor = { 1, 1, 1, 1 };
        
        std::stringstream ss2;
        ss2 << waitingForServerInterface.getTimeToNextRound();
        std::string timerText = ss2.str();
        
        m_font->renderText(*m_spriteBatcherWithColor, timerText, SCREEN_WIDTH - 3, SCREEN_HEIGHT - 2.5f, 1.5f, 1.5f, timerColor, true);
    }
    
    static Color interfaceColor = { 1, 1, 1, 1 };
    interfaceColor.alpha -= 0.025f;
    if(interfaceColor.alpha < 0.2f)
    {
        interfaceColor.alpha = 1;
    }
    
    std::stringstream ss;
    ss << "Waiting for next Round";
    std::string waitingText = ss.str();
    
    m_font->renderText(*m_spriteBatcherWithColor, waitingText, SCREEN_WIDTH / 2, 0.5f, 0.5f, 0.5f, interfaceColor, true);
    
    m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceTexture);
}

void OpenGLESRenderer::renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    static Color interfaceColor = { 1, 1, 1, 1 };
    interfaceColor.alpha -= 0.025f;
    if(interfaceColor.alpha < 0.2f)
    {
        interfaceColor.alpha = 1;
    }
    
    m_spriteBatcherWithColor->beginBatch();
    
    std::stringstream ss;
    ss << "Tap anywhere to play again!";
    std::string waitingText = ss.str();
    
    m_font->renderText(*m_spriteBatcherWithColor, waitingText, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.5f, 0.5f, interfaceColor, true);
    
    m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceTexture);
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
        if((**itr).getPowerUpType() != POWER_UP_TYPE_NONE)
        {
            renderGameObject((**itr), Assets::getPowerUpBarItemTextureRegion((**itr), interfaceOverlay.getPowerUpBarItemsStateTime()));
        }
    }
    
    if(interfaceOverlay.getActiveButton().getPowerUpType() == POWER_UP_TYPE_PUSH)
    {
        renderGameObject(interfaceOverlay.getActiveButton(), Assets::getActiveButtonTextureRegion(interfaceOverlay.getActiveButton(), interfaceOverlay.getButtonsStateTime()));
    }
    
    renderGameObject(interfaceOverlay.getBombButton(), Assets::getBombButtonTextureRegion(interfaceOverlay.getBombButton(), interfaceOverlay.getButtonsStateTime()));
    
    m_spriteBatcher->endBatchWithTexture(m_interfaceTexture);
    
    std::stringstream ss;
    ss << interfaceOverlay.getNumMinutesLeft() << ":" << interfaceOverlay.getNumSecondsLeftFirstColumn() << interfaceOverlay.getNumSecondsLeftSecondColumn();
    std::string timeRemaining = ss.str();
    static Color interfaceColor = { 1, 1, 1, 1 };
    
    static const float timerX = 0.3554104477903f;
    static const float timerY = 13.0589552253125f;
    static const float timerWidth = 0.40298507462688f;
    static const float timerHeight = 0.425373134375f;
    
    m_spriteBatcherWithColor->beginBatch();
    m_font->renderText(*m_spriteBatcherWithColor, timeRemaining, timerX, timerY, timerWidth, timerHeight, interfaceColor);
    
    for (std::vector<std::unique_ptr<PowerUpBarItem>>::iterator itr = interfaceOverlay.getPowerUpBarItems().begin(); itr != interfaceOverlay.getPowerUpBarItems().end(); itr++)
    {
        if((**itr).getPowerUpType() != POWER_UP_TYPE_NONE)
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
                m_rectangleRenderer->renderRectangle(leftX, bottomY, leftX + miniMapGridWidth, bottomY + miniMapGridHeight, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
            }
        }
    }
}

void OpenGLESRenderer::renderSpectatorInterface(InterfaceOverlay &interfaceOverlay)
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_X, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_Y, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_WIDTH, INTERFACE_OVERLAY_SPECTATOR_BACKGROUND_HEIGHT, 0, Assets::getSpectatorInterfaceOverlayTextureRegion());
    renderGameObject(interfaceOverlay.getSpectatorControls(), Assets::getSpectatorControlsTextureRegion(interfaceOverlay.getSpectatorControls()));
    m_spriteBatcher->endBatchWithTexture(m_interfaceTexture);
    
    static Color interfaceColor = { 1, 1, 1, 1 };
    
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
    
    m_spriteBatcherWithColor->endBatchWithTexture(m_interfaceTexture);
    
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
                m_rectangleRenderer->renderRectangle(leftX, bottomY, leftX + miniMapGridWidth, bottomY + miniMapGridHeight, interfaceOverlay.getColorForMiniMapGridType(miniMapGridType));
            }
        }
    }
}

void OpenGLESRenderer::renderGameOverBlackCover(float alpha)
{
    static Color transitionCoverColor = { 0, 0, 0, 0 };
    transitionCoverColor.alpha = alpha;
    
    m_rectangleRenderer->renderRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, transitionCoverColor);
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
    glDeleteTextures(1, &m_interfaceTexture2);
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