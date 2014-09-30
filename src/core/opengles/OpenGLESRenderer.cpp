//
//  OpenGLESRenderer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "OpenGLESRenderer.h"
#include "OpenGLESSpriteBatcher.h"
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
#include "OpenGLESRectangleBatcher.h"
#include "MiniMapGridType.h"
#include "Rectangle.h"
#include "PlayerForceFieldState.h"
#include "SpectatorControls.h"
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "CountDownNumberGameObject.h"
#include "DisplayBattleGameObject.h"
#include "DisplayGameOverGameObject.h"
#include <sstream>

extern "C"
{
#include "asset_utils.h"
#include "platform_gl.h"
}

OpenGLESRenderer::OpenGLESRenderer(int width, int height) : Renderer()
{
    m_spriteBatcher = std::unique_ptr<OpenGLESSpriteBatcher>(new OpenGLESSpriteBatcher());
    m_rectangleBatcher = std::unique_ptr<OpenGLESRectangleBatcher>(new OpenGLESRectangleBatcher(true));
    
    m_gameTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("map_space.png")));
    m_interfaceTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("interface.png")));
    m_interfaceTexture2 = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("interface_2.png")));
    
    m_charBlackTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("char_black.png")));
    m_charBlueTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_blue.png")));
    m_charGreenTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_green.png")));
    m_charOrangeTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_orange.png")));
    m_charPinkTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_pink.png")));
    m_charRedTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_red.png")));
    m_charWhiteTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_white.png")));
    m_charYellowTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("bot_yellow.png")));
}

void OpenGLESRenderer::loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    glDeleteTextures(1, &m_gameTexture->texture);
    
    switch (mapType)
    {
        case MAP_SPACE:
            m_gameTexture->texture = load_png_asset_into_texture("map_space.png");
            break;
        case MAP_GRASSLANDS:
            m_gameTexture->texture = load_png_asset_into_texture("map_grasslands.png");
            break;
        case MAP_MOUNTAINS:
            m_gameTexture->texture = load_png_asset_into_texture("map_mountains.png");
            break;
        case MAP_BASE:
        default:
            m_gameTexture->texture = load_png_asset_into_texture("map_base.png");
            break;
    }
    
    if((players.at(0)->isBot() && player_sprites_loaded[0]) || (!players.at(0)->isBot() && !player_sprites_loaded[0]))
    {
        glDeleteTextures(1, &m_charBlackTexture->texture);
        m_charBlackTexture->texture = load_png_asset_into_texture(players.at(0)->isBot() ? "bot_black.png" : "char_black.png");
    }
    
    if((players.at(1)->isBot() && player_sprites_loaded[1]) || (!players.at(1)->isBot() && !player_sprites_loaded[1]))
    {
        glDeleteTextures(1, &m_charBlueTexture->texture);
        m_charBlueTexture->texture = load_png_asset_into_texture(players.at(1)->isBot() ? "bot_blue.png" : "char_blue.png");
    }
    
    if((players.at(2)->isBot() && player_sprites_loaded[2]) || (!players.at(2)->isBot() && !player_sprites_loaded[2]))
    {
        glDeleteTextures(1, &m_charGreenTexture->texture);
        m_charGreenTexture->texture = load_png_asset_into_texture(players.at(2)->isBot() ? "bot_green.png" : "char_green.png");
    }
    
    if((players.at(3)->isBot() && player_sprites_loaded[3]) || (!players.at(3)->isBot() && !player_sprites_loaded[3]))
    {
        glDeleteTextures(1, &m_charOrangeTexture->texture);
        m_charOrangeTexture->texture = load_png_asset_into_texture(players.at(3)->isBot() ? "bot_orange.png" : "char_orange.png");
    }
    
    if((players.at(4)->isBot() && player_sprites_loaded[4]) || (!players.at(4)->isBot() && !player_sprites_loaded[4]))
    {
        glDeleteTextures(1, &m_charPinkTexture->texture);
        m_charPinkTexture->texture = load_png_asset_into_texture(players.at(4)->isBot() ? "bot_pink.png" : "char_pink.png");
    }
    
    if((players.at(5)->isBot() && player_sprites_loaded[5]) || (!players.at(5)->isBot() && !player_sprites_loaded[5]))
    {
        glDeleteTextures(1, &m_charRedTexture->texture);
        m_charRedTexture->texture = load_png_asset_into_texture(players.at(5)->isBot() ? "bot_red.png" : "char_red.png");
    }
    
    if((players.at(6)->isBot() && player_sprites_loaded[6]) || (!players.at(6)->isBot() && !player_sprites_loaded[6]))
    {
        glDeleteTextures(1, &m_charWhiteTexture->texture);
        m_charWhiteTexture->texture = load_png_asset_into_texture(players.at(6)->isBot() ? "bot_white.png" : "char_white.png");
    }
    
    if((players.at(7)->isBot() && player_sprites_loaded[7]) || (!players.at(7)->isBot() && !player_sprites_loaded[7]))
    {
        glDeleteTextures(1, &m_charYellowTexture->texture);
        m_charYellowTexture->texture = load_png_asset_into_texture(players.at(7)->isBot() ? "bot_yellow.png" : "char_yellow.png");
    }
    
    updatePlayerSpritesLoadedArray(players);
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

void OpenGLESRenderer::renderWorldForeground(std::vector<std::unique_ptr<MapBorder>> &mapBordersFar, std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer::renderWorldForeground(mapBordersFar, insideBlocks, breakableBlocks, powerUps);
}

void OpenGLESRenderer::renderWaitingForServerInterface(WaitingForServerInterface &waitingForServerInterface)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer::renderWaitingForServerInterface(waitingForServerInterface);
}

void OpenGLESRenderer::renderWaitingForLocalSettingsInterface(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer::renderWaitingForLocalSettingsInterface(waitingForLocalSettingsInterface);
}

void OpenGLESRenderer::endFrame()
{
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::cleanUp()
{
    glDeleteTextures(1, &m_gameTexture->texture);
    glDeleteTextures(1, &m_interfaceTexture->texture);
    glDeleteTextures(1, &m_interfaceTexture2->texture);
    glDeleteTextures(1, &m_charBlackTexture->texture);
    glDeleteTextures(1, &m_charBlueTexture->texture);
    glDeleteTextures(1, &m_charGreenTexture->texture);
    glDeleteTextures(1, &m_charOrangeTexture->texture);
    glDeleteTextures(1, &m_charPinkTexture->texture);
    glDeleteTextures(1, &m_charRedTexture->texture);
    glDeleteTextures(1, &m_charWhiteTexture->texture);
    glDeleteTextures(1, &m_charYellowTexture->texture);
}