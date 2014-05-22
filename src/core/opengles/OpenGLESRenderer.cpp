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
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"

extern "C"
{
#include "asset_utils.h"
}

OpenGLESRenderer::OpenGLESRenderer() : Renderer()
{
    m_spriteBatcher = std::unique_ptr<SpriteBatcher>(new SpriteBatcher(4000, false));
    
    m_gameTexture = load_png_asset_into_texture("game.png");
}

void OpenGLESRenderer::renderWorldBackground()
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(WORLD_BACKGROUND_X, WORLD_BACKGROUND_Y - m_fScrollY, WORLD_BACKGROUND_WIDTH, WORLD_BACKGROUND_HEIGHT, 0, Assets::getWorldBackgroundTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderWorldForeground(std::vector<std::unique_ptr<InsideBlock>> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock>> &breakableBlocks, std::vector<std::unique_ptr<PowerUp>> &powerUps)
{
    if(insideBlocks.size() > 0)
    {
        m_spriteBatcher->beginBatch();
        
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
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
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
        renderGameObjectWithRespectToPlayer((**itr), Assets::getExplosionTextureRegion((**itr)));
    }
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderInterface()
{
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(INTERFACE_BACKGROUND_X, INTERFACE_BACKGROUND_Y, INTERFACE_BACKGROUND_WIDTH, INTERFACE_BACKGROUND_HEIGHT, 0, Assets::getInterfaceOverlayTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

void OpenGLESRenderer::renderControls(DPadControl &dPadControl, ActiveButton &activeButton)
{
    m_spriteBatcher->beginBatch();
    renderGameObject(dPadControl, Assets::getDPadControlTextureRegion());
    renderGameObject(activeButton, Assets::getActiveButtonTextureRegion());
    m_spriteBatcher->endBatchWithTexture(m_gameTexture);
}

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