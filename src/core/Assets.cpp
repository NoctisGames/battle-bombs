//
//  Assets.cpp
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Assets.h"
#include "TextureRegion.h"
#include "PlayerDynamicGameObject.h"
#include "Vector2D.h"
#include "InsideBlock.h"
#include "PlayerState.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include <list>

Assets * Assets::getInstance()
{
	static Assets *assets = new Assets();
	return assets;
}

TextureRegion Assets::getWorldBackgroundTextureRegion()
{
    static TextureRegion textureRegion = TextureRegion(WORLD_BACKGROUND_TEXTURE_REGION_X, WORLD_BACKGROUND_TEXTURE_REGION_Y, WORLD_BACKGROUND_TEXTURE_REGION_WIDTH, WORLD_BACKGROUND_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    return textureRegion;
}

TextureRegion Assets::getInsideBlockTextureRegion()
{
    static TextureRegion insideBlockTextureRegion = TextureRegion(INSIDE_BLOCK_TEXTURE_REGION_X, INSIDE_BLOCK_TEXTURE_REGION_Y, INSIDE_BLOCK_TEXTURE_REGION_WIDTH, INSIDE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
	
	return insideBlockTextureRegion;
}

TextureRegion Assets::getBreakableBlockTextureRegion()
{
	static TextureRegion breakableBlockTextureRegion = TextureRegion(BREAKABLE_BLOCK_TEXTURE_REGION_X, BREAKABLE_BLOCK_TEXTURE_REGION_Y, INSIDE_BLOCK_TEXTURE_REGION_WIDTH, INSIDE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);

	return breakableBlockTextureRegion;
}

TextureRegion Assets::getInterfaceOverlayTextureRegion()
{
    static TextureRegion textureRegion = TextureRegion(INTERFACE_OVERLAY_TEXTURE_REGION_X, INTERFACE_OVERLAY_TEXTURE_REGION_Y, INTERFACE_OVERLAY_TEXTURE_REGION_WIDTH, INTERFACE_OVERLAY_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    return textureRegion;
}

TextureRegion Assets::getDPadControlTextureRegion()
{
    static TextureRegion textureRegion = TextureRegion(DIRECTIONAL_CONTROL_TEXTURE_REGION_X, DIRECTIONAL_CONTROL_TEXTURE_REGION_Y, DIRECTIONAL_CONTROL_TEXTURE_REGION_WIDTH, DIRECTIONAL_CONTROL_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    return textureRegion;
}
TextureRegion Assets::getActiveButtonTextureRegion()
{
	static TextureRegion textureRegion = TextureRegion(ACTIVE_BUTTON_TEXTURE_REGION_X, ACTIVE_BUTTON_TEXTURE_REGION_Y, ACTIVE_BUTTON_TEXTURE_REGION_WIDTH, ACTIVE_BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
	return textureRegion;
}

TextureRegion Assets::getPlayerTextureRegion(PlayerDynamicGameObject &player)
{
    if(player.getPlayerState() == Player_State::ALIVE)
    {
        if(player.isMoving())
        {
            static std::vector<TextureRegion> playerRightTextureRegions;
            if(playerRightTextureRegions.size() == 0)
            {
                playerRightTextureRegions.push_back(TextureRegion(PLAYER_RIGHT_STEP_LEFT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerRightTextureRegions.push_back(TextureRegion(PLAYER_RIGHT_STEP_RIGHT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerUpTextureRegions;
            if(playerUpTextureRegions.size() == 0)
            {
                playerUpTextureRegions.push_back(TextureRegion(PLAYER_UP_STEP_LEFT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerUpTextureRegions.push_back(TextureRegion(PLAYER_UP_STEP_RIGHT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerLeftTextureRegions;
            if(playerLeftTextureRegions.size() == 0)
            {
                playerLeftTextureRegions.push_back(TextureRegion(PLAYER_LEFT_STEP_LEFT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerLeftTextureRegions.push_back(TextureRegion(PLAYER_LEFT_STEP_RIGHT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerDownTextureRegions;
            if(playerDownTextureRegions.size() == 0)
            {
                playerDownTextureRegions.push_back(TextureRegion(PLAYER_DOWN_STEP_LEFT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerDownTextureRegions.push_back(TextureRegion(PLAYER_DOWN_STEP_RIGHT_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static float cycleTime = 0.6f;
            static std::vector<float> playerFrames;
            if(playerFrames.size() == 0)
            {
                playerFrames.push_back(0.3f);
                playerFrames.push_back(0.3f);
            }
            
            switch (player.getDirection())
            {
                case DIRECTION_RIGHT:
                    return playerRightTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_UP:
                    return playerUpTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_LEFT:
                    return playerLeftTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_DOWN:
                default:
                    return playerDownTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            }
        }
        else
        {
            static TextureRegion playerRightIdleTextureRegion = TextureRegion(PLAYER_RIGHT_IDLE_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
            static TextureRegion playerUpIdleTextureRegion = TextureRegion(PLAYER_UP_IDLE_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
            static TextureRegion playerLeftIdleTextureRegion = TextureRegion(PLAYER_LEFT_IDLE_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
            static TextureRegion playerDownIdleTextureRegion = TextureRegion(PLAYER_DOWN_IDLE_TEXTURE_REGION_X, PLAYER_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
            
            switch (player.getDirection())
            {
                case DIRECTION_RIGHT:
                    return playerRightIdleTextureRegion;
                case DIRECTION_UP:
                    return playerUpIdleTextureRegion;
                case DIRECTION_LEFT:
                    return playerLeftIdleTextureRegion;
                case DIRECTION_DOWN:
                default:
                    return playerDownIdleTextureRegion;
            }
        }
    }
    else
    {
        static std::vector<TextureRegion> playerDeathTextureRegions;
        if(playerDeathTextureRegions.size() == 0)
        {
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_7_TEXTURE_REGION_X, PLAYER_DEATH_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        }
        
        static float cycleTime = 0.98f;
        static std::vector<float> playerFrames;
        if(playerFrames.size() == 0)
        {
            playerFrames.push_back(0.14f);
            playerFrames.push_back(0.14f);
            playerFrames.push_back(0.14f);
            playerFrames.push_back(0.14f);
            playerFrames.push_back(0.14f);
            playerFrames.push_back(0.14f);
            playerFrames.push_back(0.14f);
        }
        
        return playerDeathTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
    }
}

TextureRegion Assets::getBombTextureRegion(BombGameObject &bomb)
{
    static std::vector<TextureRegion> bombTextureRegions;
    if(bombTextureRegions.size() == 0)
    {
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_1_TEXTURE_REGION_X, BOMB_FRAME_1_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_2_TEXTURE_REGION_X, BOMB_FRAME_2_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_3_TEXTURE_REGION_X, BOMB_FRAME_3_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static float cycleTime = 3.00f;
    static std::vector<float> bombFrames;
    if(bombFrames.size() == 0)
    {
        bombFrames.push_back(1.00f);
        bombFrames.push_back(1.00f);
        bombFrames.push_back(1.00f);
    }
    
    return bombTextureRegions.at(getKeyFrameNumber(bomb.getStateTime(), cycleTime, bombFrames));
}

TextureRegion Assets::getExplosionTextureRegion(Explosion &explosion)
{
    static std::vector<TextureRegion> explosionCoreTextureRegions;
    if(explosionCoreTextureRegions.size() == 0)
    {
        explosionCoreTextureRegions.push_back(TextureRegion(EXPLOSION_CORE_FRAME_1_TEXTURE_REGION_X, EXPLOSION_CORE_FRAME_1_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionCoreTextureRegions.push_back(TextureRegion(EXPLOSION_CORE_FRAME_2_TEXTURE_REGION_X, EXPLOSION_CORE_FRAME_2_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionCoreTextureRegions.push_back(TextureRegion(EXPLOSION_CORE_FRAME_3_TEXTURE_REGION_X, EXPLOSION_CORE_FRAME_3_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionCoreTextureRegions.push_back(TextureRegion(EXPLOSION_CORE_FRAME_4_TEXTURE_REGION_X, EXPLOSION_CORE_FRAME_4_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> explosionBodyTextureRegions;
    if(explosionBodyTextureRegions.size() == 0)
    {
        explosionBodyTextureRegions.push_back(TextureRegion(EXPLOSION_BODY_FRAME_1_TEXTURE_REGION_X, EXPLOSION_BODY_FRAME_1_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionBodyTextureRegions.push_back(TextureRegion(EXPLOSION_BODY_FRAME_2_TEXTURE_REGION_X, EXPLOSION_BODY_FRAME_2_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionBodyTextureRegions.push_back(TextureRegion(EXPLOSION_BODY_FRAME_3_TEXTURE_REGION_X, EXPLOSION_BODY_FRAME_3_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionBodyTextureRegions.push_back(TextureRegion(EXPLOSION_BODY_FRAME_4_TEXTURE_REGION_X, EXPLOSION_BODY_FRAME_4_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> explosionTailTextureRegions;
    if(explosionTailTextureRegions.size() == 0)
    {
        explosionTailTextureRegions.push_back(TextureRegion(EXPLOSION_TAIL_FRAME_1_TEXTURE_REGION_X, EXPLOSION_TAIL_FRAME_1_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionTailTextureRegions.push_back(TextureRegion(EXPLOSION_TAIL_FRAME_2_TEXTURE_REGION_X, EXPLOSION_TAIL_FRAME_2_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionTailTextureRegions.push_back(TextureRegion(EXPLOSION_TAIL_FRAME_3_TEXTURE_REGION_X, EXPLOSION_TAIL_FRAME_3_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        explosionTailTextureRegions.push_back(TextureRegion(EXPLOSION_TAIL_FRAME_4_TEXTURE_REGION_X, EXPLOSION_TAIL_FRAME_4_TEXTURE_REGION_Y, EXPLOSION_TEXTURE_REGION_WIDTH, EXPLOSION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static float cycleTime = 0.60f;
    static std::vector<float> explosionFrames;
    if(explosionFrames.size() == 0)
    {
        explosionFrames.push_back(0.15f);
        explosionFrames.push_back(0.15f);
        explosionFrames.push_back(0.15f);
        explosionFrames.push_back(0.15f);
    }
    
    switch (explosion.getExplosionType())
    {
        case CORE:
            return explosionCoreTextureRegions.at(getKeyFrameNumber(explosion.getStateTime(), cycleTime, explosionFrames));
        case BODY:
            return explosionBodyTextureRegions.at(getKeyFrameNumber(explosion.getStateTime(), cycleTime, explosionFrames));
        case TAIL:
        default:
            return explosionTailTextureRegions.at(getKeyFrameNumber(explosion.getStateTime(), cycleTime, explosionFrames));
    }
}

TextureRegion Assets::getPowerUpTextureRegion(PowerUp &powerUp)
{
	static TextureRegion bombTextureRegion = TextureRegion(BOMB_POWER_UP_TEXTURE_REGION_X, BOMB_POWER_UP_TEXTURE_REGION_Y, BOMB_POWER_UP_TEXTURE_REGION_WIDTH, BOMB_POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
	static TextureRegion fireTextureRegion = TextureRegion(FIRE_POWER_UP_TEXTURE_REGION_X, FIRE_POWER_UP_TEXTURE_REGION_Y, FIRE_POWER_UP_TEXTURE_REGION_WIDTH, FIRE_POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
	static TextureRegion speedTextureRegion = TextureRegion(SPEED_POWER_UP_TEXTURE_REGION_X, SPEED_POWER_UP_TEXTURE_REGION_Y, SPEED_POWER_UP_TEXTURE_REGION_WIDTH, SPEED_POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
	static TextureRegion kickTextureRegion = TextureRegion(KICK_POWER_UP_TEXTURE_REGION_X, KICK_POWER_UP_TEXTURE_REGION_Y, KICK_POWER_UP_TEXTURE_REGION_WIDTH, KICK_POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);

	switch(powerUp.getType())
	{
		case NONE:
			return bombTextureRegion;
		case BOMBS:
			return bombTextureRegion;
		case FIRE:
			return fireTextureRegion;
		case SPEED:
			return speedTextureRegion;
		case KICK:
			return kickTextureRegion;
		default:
			return bombTextureRegion;
	}

}

void Assets::setMusicId(short musicId)
{
    m_sMusicId = musicId;
}

short Assets::getMusicId()
{
    return m_sMusicId;
}

short Assets::getFirstSoundId()
{
    return m_sSoundIds.size() > 0 ? m_sSoundIds.front() : 0;
}

void Assets::addSoundIdToPlayQueue(short soundId)
{
    if (m_sSoundIds.size() < MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        m_sSoundIds.push_back(soundId);
    }
}

void Assets::eraseFirstSoundId()
{
    if(m_sSoundIds.size() > 0)
    {
        m_sSoundIds.erase(m_sSoundIds.begin());
    }
}

int Assets::getKeyFrameNumber(float stateTime, float cycleTime, std::vector<float> &frameDurations)
{
    if (stateTime > cycleTime && cycleTime > 0)
    {
        while(stateTime > cycleTime)
        {
            stateTime -= cycleTime;
        }
    }
    
    for (unsigned int i = 0; i < frameDurations.size(); i++)
    {
        if (stateTime < frameDurations.at(i))
        {
            return i;
        }
        
        stateTime -= frameDurations.at(i);
    }
    
    return 0;
}

Assets::Assets()
{
    // Hide Constructor for Singleton
}