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
    
    m_gameTexture = load_png_asset_into_texture("game.png");
    m_charBlueTexture = load_png_asset_into_texture("char_blue.png");
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
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<PlayerDynamicGameObject>>::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((**itr).getPlayerState() != Player_State::DEAD)
        {
            renderGameObjectWithRespectToPlayer((**itr), Assets::getPlayerTextureRegion((**itr)));
        }
    }
    m_spriteBatcher->endBatchWithTexture(m_charBlueTexture);
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

void OpenGLESRenderer::renderInterface()
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(INTERFACE_OVERLAY_BACKGROUND_X, INTERFACE_OVERLAY_BACKGROUND_Y, INTERFACE_OVERLAY_BACKGROUND_WIDTH, INTERFACE_OVERLAY_BACKGROUND_HEIGHT, 0, Assets::getInterfaceOverlayTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::endFrame()
{
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::cleanUp()
{
    // TODO
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