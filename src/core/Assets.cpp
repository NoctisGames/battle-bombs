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
#include "MapBorder.h"
#include "InsideBlock.h"
#include "PlayerState.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "BreakableBlock.h"
#include "PowerUp.h"
#include "Fire.h"
#include "DPadControl.h"
#include "PowerUpBarItem.h"
#include "ActiveButton.h"
#include "BombButton.h"
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

TextureRegion Assets::getMapBorderTextureRegion(MapBorder &mapBorder)
{
    static TextureRegion TR_MOUNTAINS_DOOR = TextureRegion(MOUNTAINS_DOOR_TEXTURE_REGION_X, MOUNTAINS_DOOR_TEXTURE_REGION_Y, MOUNTAINS_DOOR_TEXTURE_REGION_WIDTH, MOUNTAINS_DOOR_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BORDER_TOP = TextureRegion(BORDER_TOP_TEXTURE_REGION_X, BORDER_TOP_TEXTURE_REGION_Y, BORDER_TOP_TEXTURE_REGION_WIDTH, BORDER_TOP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BORDER_LEFT = TextureRegion(BORDER_LEFT_TEXTURE_REGION_X, BORDER_LEFT_TEXTURE_REGION_Y, BORDER_LEFT_TEXTURE_REGION_WIDTH, BORDER_LEFT_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BORDER_RIGHT = TextureRegion(BORDER_RIGHT_TEXTURE_REGION_X, BORDER_RIGHT_TEXTURE_REGION_Y, BORDER_RIGHT_TEXTURE_REGION_WIDTH, BORDER_RIGHT_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BORDER_BOTTOM_LEFT = TextureRegion(BORDER_BOTTOM_LEFT_TEXTURE_REGION_X, BORDER_BOTTOM_LEFT_TEXTURE_REGION_Y, BORDER_BOTTOM_LEFT_TEXTURE_REGION_WIDTH, BORDER_BOTTOM_LEFT_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BORDER_BOTTOM_RIGHT = TextureRegion(BORDER_BOTTOM_RIGHT_TEXTURE_REGION_X, BORDER_BOTTOM_RIGHT_TEXTURE_REGION_Y, BORDER_BOTTOM_RIGHT_TEXTURE_REGION_WIDTH, BORDER_BOTTOM_RIGHT_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BORDER_BOTTOM = TextureRegion(BORDER_BOTTOM_TEXTURE_REGION_X, BORDER_BOTTOM_TEXTURE_REGION_Y, BORDER_BOTTOM_TEXTURE_REGION_WIDTH, BORDER_BOTTOM_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    
    switch (mapBorder.getType())
    {
        case MOUNTAINS_DOOR:
            return TR_MOUNTAINS_DOOR;
        case BORDER_TOP:
            return TR_BORDER_TOP;
        case BORDER_LEFT:
            return TR_BORDER_LEFT;
        case BORDER_RIGHT:
            return TR_BORDER_RIGHT;
        case BORDER_BOTTOM_LEFT:
            return TR_BORDER_BOTTOM_LEFT;
        case BORDER_BOTTOM_RIGHT:
            return TR_BORDER_BOTTOM_RIGHT;
        case BORDER_BOTTOM:
        default:
            return TR_BORDER_BOTTOM;
    }
}

TextureRegion Assets::getInsideBlockTextureRegion()
{
    static TextureRegion insideBlockTextureRegion = TextureRegion(INSIDE_BLOCK_TEXTURE_REGION_X, INSIDE_BLOCK_TEXTURE_REGION_Y, INSIDE_BLOCK_TEXTURE_REGION_WIDTH, INSIDE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);

    return insideBlockTextureRegion;
}

TextureRegion Assets::getNineTextureRegion()
{
    static TextureRegion insideBlockTextureRegion = TextureRegion(POWER_UP_ICON_FRAME_5_TEXTURE_REGION_X, POWER_UP_ICON_FORCE_FIELD_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    
    return insideBlockTextureRegion;
}

TextureRegion Assets::getOneTextureRegion()
{
    static TextureRegion insideBlockTextureRegion = TextureRegion(POWER_UP_ICON_FRAME_1_TEXTURE_REGION_X, POWER_UP_ICON_FORCE_FIELD_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    
    return insideBlockTextureRegion;
}

TextureRegion Assets::getBreakableBlockTextureRegion(BreakableBlock &breakableBlock)
{
    if(breakableBlock.getBreakableBlockState() == NORMAL)
    {
        static TextureRegion textureRegion = TextureRegion(BREAKABLE_BLOCK_TEXTURE_REGION_X, BREAKABLE_BLOCK_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
        
        return textureRegion;
    }
    else if(breakableBlock.getBreakableBlockState() == EXPLODING)
    {
        static std::vector<TextureRegion> breakableBlockTextureRegions;
        if (breakableBlockTextureRegions.size() == 0)
        {
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            breakableBlockTextureRegions.push_back(TextureRegion(BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        }
        
        static float cycleTime = 1.00f;
        static std::vector<float> frames;
        if (frames.size() == 0)
        {
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
            frames.push_back(0.1f);
        }
        
        return breakableBlockTextureRegions.at(getKeyFrameNumber(breakableBlock.getStateTime(), cycleTime, frames));
    }
    else
    {
        static TextureRegion textureRegion = TextureRegion(BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_X, BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_Y, BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH, BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
        
        return textureRegion;
    }
}

TextureRegion Assets::getInterfaceOverlayTextureRegion()
{
    static TextureRegion textureRegion = TextureRegion(INTERFACE_OVERLAY_TEXTURE_REGION_X, INTERFACE_OVERLAY_TEXTURE_REGION_Y, INTERFACE_OVERLAY_TEXTURE_REGION_WIDTH, INTERFACE_OVERLAY_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    return textureRegion;
}

TextureRegion Assets::getDPadControlTextureRegion(DPadControl &dpadControl)
{
    static TextureRegion TR_D_PAD_RIGHT_TEXTURE_REGION = TextureRegion(D_PAD_RIGHT_TEXTURE_REGION_X, D_PAD_RIGHT_TEXTURE_REGION_Y, D_PAD_RIGHT_TEXTURE_REGION_WIDTH, D_PAD_RIGHT_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_D_PAD_UP_TEXTURE_REGION = TextureRegion(D_PAD_UP_TEXTURE_REGION_X, D_PAD_UP_TEXTURE_REGION_Y, D_PAD_UP_TEXTURE_REGION_WIDTH, D_PAD_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_D_PAD_LEFT_TEXTURE_REGION = TextureRegion(D_PAD_LEFT_TEXTURE_REGION_X, D_PAD_LEFT_TEXTURE_REGION_Y, D_PAD_LEFT_TEXTURE_REGION_WIDTH, D_PAD_LEFT_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_D_PAD_DOWN_TEXTURE_REGION = TextureRegion(D_PAD_DOWN_TEXTURE_REGION_X, D_PAD_DOWN_TEXTURE_REGION_Y, D_PAD_DOWN_TEXTURE_REGION_WIDTH, D_PAD_DOWN_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_D_PAD_NO_DIRECTION_TEXTURE_REGION = TextureRegion(D_PAD_NO_DIRECTION_TEXTURE_REGION_X, D_PAD_NO_DIRECTION_TEXTURE_REGION_Y, D_PAD_NO_DIRECTION_TEXTURE_REGION_WIDTH, D_PAD_NO_DIRECTION_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    
    switch(dpadControl.getState())
    {
        case RIGHT_ARROW:
            return TR_D_PAD_RIGHT_TEXTURE_REGION;
        case UP_ARROW:
            return TR_D_PAD_UP_TEXTURE_REGION;
        case LEFT_ARROW:
            return TR_D_PAD_LEFT_TEXTURE_REGION;
        case DOWN_ARROW:
            return TR_D_PAD_DOWN_TEXTURE_REGION;
        case NO_DIRECTION:
        default:
            return TR_D_PAD_NO_DIRECTION_TEXTURE_REGION;
    }
}

TextureRegion Assets::getPlayerTextureRegion(PlayerDynamicGameObject &player)
{
    if (player.getPlayerState() == Player_State::ALIVE)
    {
        if(player.getPlayerActionState() == IDLE)
        {
            if (player.isMoving())
            {
                static std::vector<TextureRegion> playerRightTextureRegions;
                if (playerRightTextureRegions.size() == 0)
                {
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_1_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_2_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_3_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_4_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_5_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_6_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_7_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerRightTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_8_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                }
                
                static std::vector<TextureRegion> playerUpTextureRegions;
                if (playerUpTextureRegions.size() == 0)
                {
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_1_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_2_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_3_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_4_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_5_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_6_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_7_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerUpTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_8_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                }
                
                static std::vector<TextureRegion> playerLeftTextureRegions;
                if (playerLeftTextureRegions.size() == 0)
                {
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_1_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_2_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_3_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_4_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_5_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_6_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_7_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerLeftTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_8_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                }
                
                static std::vector<TextureRegion> playerDownTextureRegions;
                if (playerDownTextureRegions.size() == 0)
                {
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_1_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_2_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_3_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_4_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_5_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_6_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_7_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerDownTextureRegions.push_back(TextureRegion(PLAYER_WALK_FRAME_8_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                }
                
                static float cycleTime = 0.8f;
                static std::vector<float> playerFrames;
                if (playerFrames.size() == 0)
                {
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
                    playerFrames.push_back(0.1f);
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
            else if(player.getStateTime() > 3)
            {
                static std::vector<TextureRegion> playerIdleTextureRegions;
                if (playerIdleTextureRegions.size() == 0)
                {
                    playerIdleTextureRegions.push_back(TextureRegion(PLAYER_IDLE_FRAME_1_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerIdleTextureRegions.push_back(TextureRegion(PLAYER_IDLE_FRAME_2_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerIdleTextureRegions.push_back(TextureRegion(PLAYER_IDLE_FRAME_3_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                    playerIdleTextureRegions.push_back(TextureRegion(PLAYER_IDLE_FRAME_4_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                }
                
                static float cycleTime = 0.8f;
                static std::vector<float> playerFrames;
                if (playerFrames.size() == 0)
                {
                    playerFrames.push_back(0.2f);
                    playerFrames.push_back(0.2f);
                    playerFrames.push_back(0.2f);
                    playerFrames.push_back(0.2f);
                }
                
                return playerIdleTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            }
            else
            {
                static TextureRegion playerRightIdleTextureRegion = TextureRegion(PLAYER_WALK_IDLE_TEXTURE_REGION_X, PLAYER_WALK_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
                static TextureRegion playerUpIdleTextureRegion = TextureRegion(PLAYER_WALK_IDLE_TEXTURE_REGION_X, PLAYER_WALK_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
                static TextureRegion playerLeftIdleTextureRegion = TextureRegion(PLAYER_WALK_IDLE_TEXTURE_REGION_X, PLAYER_WALK_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
                static TextureRegion playerDownIdleTextureRegion = TextureRegion(PLAYER_WALK_IDLE_TEXTURE_REGION_X, PLAYER_WALK_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
                
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
        else if(player.getPlayerActionState() == PLACING_BOMB)
        {
            static std::vector<TextureRegion> playerPlacingBombRightTextureRegions;
            if (playerPlacingBombRightTextureRegions.size() == 0)
            {
                playerPlacingBombRightTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombRightTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombRightTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerPlacingBombUpTextureRegions;
            if (playerPlacingBombUpTextureRegions.size() == 0)
            {
                playerPlacingBombUpTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_UP_FRAME_1_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_UP_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombUpTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_UP_FRAME_2_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_UP_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombUpTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_UP_FRAME_3_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_UP_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerPlacingBombLeftTextureRegions;
            if (playerPlacingBombLeftTextureRegions.size() == 0)
            {
                playerPlacingBombLeftTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_LEFT_FRAME_1_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_LEFT_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombLeftTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_LEFT_FRAME_2_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_LEFT_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombLeftTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_LEFT_FRAME_3_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_LEFT_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerPlacingBombDownTextureRegions;
            if (playerPlacingBombDownTextureRegions.size() == 0)
            {
                playerPlacingBombDownTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_DOWN_FRAME_1_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_DOWN_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombDownTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_DOWN_FRAME_2_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_DOWN_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPlacingBombDownTextureRegions.push_back(TextureRegion(PLAYER_PLACE_BOMB_DOWN_FRAME_3_TEXTURE_REGION_X, PLAYER_PLACE_BOMB_DOWN_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static float cycleTime = 0.15f;
            static std::vector<float> playerFrames;
            if (playerFrames.size() == 0)
            {
                playerFrames.push_back(0.05f);
                playerFrames.push_back(0.05f);
                playerFrames.push_back(0.05f);
            }
            
            switch (player.getDirection())
            {
                case DIRECTION_RIGHT:
                    return playerPlacingBombRightTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_UP:
                    return playerPlacingBombUpTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_LEFT:
                    return playerPlacingBombLeftTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_DOWN:
                default:
                    return playerPlacingBombDownTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            }
        }
        else if(player.getPlayerActionState() == PUSHING_BOMB)
        {
            static std::vector<TextureRegion> playerPushingBombRightTextureRegions;
            if (playerPushingBombRightTextureRegions.size() == 0)
            {
                playerPushingBombRightTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombRightTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombRightTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerPushingBombUpTextureRegions;
            if (playerPushingBombUpTextureRegions.size() == 0)
            {
                playerPushingBombUpTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_UP_FRAME_1_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_UP_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombUpTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_UP_FRAME_2_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_UP_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombUpTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_UP_FRAME_3_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_UP_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerPushingBombLeftTextureRegions;
            if (playerPushingBombLeftTextureRegions.size() == 0)
            {
                playerPushingBombLeftTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_LEFT_FRAME_1_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_LEFT_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombLeftTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_LEFT_FRAME_2_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_LEFT_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombLeftTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_LEFT_FRAME_3_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_LEFT_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static std::vector<TextureRegion> playerPushingBombDownTextureRegions;
            if (playerPushingBombDownTextureRegions.size() == 0)
            {
                playerPushingBombDownTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_DOWN_FRAME_1_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_DOWN_FRAME_1_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombDownTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_DOWN_FRAME_2_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_DOWN_FRAME_2_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerPushingBombDownTextureRegions.push_back(TextureRegion(PLAYER_PUSH_BOMB_DOWN_FRAME_3_TEXTURE_REGION_X, PLAYER_PUSH_BOMB_DOWN_FRAME_3_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static float cycleTime = 0.15f;
            static std::vector<float> playerFrames;
            if (playerFrames.size() == 0)
            {
                playerFrames.push_back(0.05f);
                playerFrames.push_back(0.05f);
                playerFrames.push_back(0.05f);
            }
            
            switch (player.getDirection())
            {
                case DIRECTION_RIGHT:
                    return playerPushingBombRightTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_UP:
                    return playerPushingBombUpTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_LEFT:
                    return playerPushingBombLeftTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
                case DIRECTION_DOWN:
                default:
                    return playerPushingBombDownTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            }
        }
        else
        {
            static std::vector<TextureRegion> playerWinningTextureRegions;
            if (playerWinningTextureRegions.size() == 0)
            {
                playerWinningTextureRegions.push_back(TextureRegion(PLAYER_WINNING_FRAME_1_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerWinningTextureRegions.push_back(TextureRegion(PLAYER_WINNING_FRAME_2_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerWinningTextureRegions.push_back(TextureRegion(PLAYER_WINNING_FRAME_3_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerWinningTextureRegions.push_back(TextureRegion(PLAYER_WINNING_FRAME_4_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerWinningTextureRegions.push_back(TextureRegion(PLAYER_WINNING_FRAME_5_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
                playerWinningTextureRegions.push_back(TextureRegion(PLAYER_WINNING_FRAME_6_TEXTURE_REGION_X, PLAYER_IDLE_WINNING_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            }
            
            static float cycleTime = 0.6f;
            static std::vector<float> playerFrames;
            if (playerFrames.size() == 0)
            {
                playerFrames.push_back(0.1f);
                playerFrames.push_back(0.1f);
                playerFrames.push_back(0.1f);
                playerFrames.push_back(0.1f);
                playerFrames.push_back(0.1f);
                playerFrames.push_back(0.1f);
            }
            
            static float cycleTimeForBot = 1.2f;
            static std::vector<float> botFrames;
            if (botFrames.size() == 0)
            {
                botFrames.push_back(0.2f);
                botFrames.push_back(0.2f);
                botFrames.push_back(0.2f);
                botFrames.push_back(0.2f);
                botFrames.push_back(0.2f);
                botFrames.push_back(0.2f);
            }
            
            return player.isBot() ? playerWinningTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTimeForBot, botFrames)) : playerWinningTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
        }
    }
    else
    {
        static std::vector<TextureRegion> playerDeathRightTextureRegions;
        if (playerDeathRightTextureRegions.size() == 0)
        {
            playerDeathRightTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X, PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathRightTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X, PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathRightTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X, PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathRightTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X, PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathRightTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X, PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathRightTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X, PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        }
        
        static std::vector<TextureRegion> playerDeathUpTextureRegions;
        if (playerDeathUpTextureRegions.size() == 0)
        {
            playerDeathUpTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X, PLAYER_DEATH_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathUpTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X, PLAYER_DEATH_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathUpTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X, PLAYER_DEATH_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathUpTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X, PLAYER_DEATH_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathUpTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X, PLAYER_DEATH_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathUpTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X, PLAYER_DEATH_UP_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        }
        
        static std::vector<TextureRegion> playerDeathLeftTextureRegions;
        if (playerDeathLeftTextureRegions.size() == 0)
        {
            playerDeathLeftTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X, PLAYER_DEATH_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathLeftTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X, PLAYER_DEATH_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathLeftTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X, PLAYER_DEATH_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathLeftTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X, PLAYER_DEATH_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathLeftTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X, PLAYER_DEATH_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathLeftTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X, PLAYER_DEATH_LEFT_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        }
        
        static std::vector<TextureRegion> playerDeathDownTextureRegions;
        if (playerDeathDownTextureRegions.size() == 0)
        {
            playerDeathDownTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X, PLAYER_DEATH_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathDownTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X, PLAYER_DEATH_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathDownTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X, PLAYER_DEATH_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathDownTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X, PLAYER_DEATH_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathDownTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X, PLAYER_DEATH_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
            playerDeathDownTextureRegions.push_back(TextureRegion(PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X, PLAYER_DEATH_DOWN_TEXTURE_REGION_Y, PLAYER_TEXTURE_REGION_WIDTH, PLAYER_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        }

        static float cycleTime = 0.6f;
        static std::vector<float> playerFrames;
        if (playerFrames.size() == 0)
        {
            playerFrames.push_back(0.1f);
            playerFrames.push_back(0.1f);
            playerFrames.push_back(0.1f);
            playerFrames.push_back(0.1f);
            playerFrames.push_back(0.1f);
            playerFrames.push_back(0.1f);
        }
        
        switch (player.getDirection())
        {
            case DIRECTION_RIGHT:
                return playerDeathRightTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            case DIRECTION_UP:
                return playerDeathUpTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            case DIRECTION_LEFT:
                return playerDeathLeftTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
            case DIRECTION_DOWN:
            default:
                return playerDeathDownTextureRegions.at(getKeyFrameNumber(player.getStateTime(), cycleTime, playerFrames));
        }
    }
}

TextureRegion Assets::getBombTextureRegion(BombGameObject &bomb)
{
    static std::vector<TextureRegion> bombTextureRegions;
    if (bombTextureRegions.size() == 0)
    {
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_1_TEXTURE_REGION_X, BOMB_FRAME_1_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_2_TEXTURE_REGION_X, BOMB_FRAME_2_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_3_TEXTURE_REGION_X, BOMB_FRAME_3_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_4_TEXTURE_REGION_X, BOMB_FRAME_4_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_5_TEXTURE_REGION_X, BOMB_FRAME_5_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_6_TEXTURE_REGION_X, BOMB_FRAME_6_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombTextureRegions.push_back(TextureRegion(BOMB_FRAME_7_TEXTURE_REGION_X, BOMB_FRAME_7_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> bombExplodingTextureRegions;
    if (bombExplodingTextureRegions.size() == 0)
    {
        bombExplodingTextureRegions.push_back(TextureRegion(BOMB_EXPLOSION_FRAME_1_TEXTURE_REGION_X, BOMB_EXPLOSION_FRAME_1_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        bombExplodingTextureRegions.push_back(TextureRegion(BOMB_EXPLOSION_FRAME_2_TEXTURE_REGION_X, BOMB_EXPLOSION_FRAME_2_TEXTURE_REGION_Y, BOMB_TEXTURE_REGION_WIDTH, BOMB_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }

    static float cycleTime = 3.00f;
    static std::vector<float> bombFrames;
    if (bombFrames.size() == 0)
    {
        bombFrames.push_back(0.42857142857143f);
        bombFrames.push_back(0.42857142857143f);
        bombFrames.push_back(0.42857142857143f);
        bombFrames.push_back(0.42857142857143f);
        bombFrames.push_back(0.42857142857143f);
        bombFrames.push_back(0.42857142857143f);
        bombFrames.push_back(0.42857142857143f);
    }
    
    static float bombExplodingCycleTime = 0.2f;
    static std::vector<float> bombExplodingFrames;
    if (bombExplodingFrames.size() == 0)
    {
        bombExplodingFrames.push_back(0.1f);
        bombExplodingFrames.push_back(0.1f);
    }
    
    if(bomb.isExploding())
    {
        return bombExplodingTextureRegions.at(getKeyFrameNumber(bomb.getStateTime(), bombExplodingCycleTime, bombExplodingFrames));
    }
    else
    {
        return bombTextureRegions.at(getKeyFrameNumber(bomb.getStateTime(), cycleTime, bombFrames));
    }
}

TextureRegion Assets::getFireTextureRegion(Fire &fire)
{
    static TextureRegion TR_CORE_PART_1 = TextureRegion(CORE_PART_1_TEXTURE_REGION_X, CORE_PART_1_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_PART_2 = TextureRegion(CORE_PART_2_TEXTURE_REGION_X, CORE_PART_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_PART_3 = TextureRegion(CORE_PART_3_TEXTURE_REGION_X, CORE_PART_3_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_PART_4_POW_1 = TextureRegion(CORE_PART_4_POW_1_TEXTURE_REGION_X, CORE_PART_4_POW_1_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_PART_4_POW_2 = TextureRegion(CORE_PART_4_POW_2_TEXTURE_REGION_X, CORE_PART_4_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_PART_5_POW_2 = TextureRegion(CORE_PART_5_POW_2_TEXTURE_REGION_X, CORE_PART_5_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_PART_6_POW_2 = TextureRegion(CORE_PART_6_POW_2_TEXTURE_REGION_X, CORE_PART_6_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_CORE_END = TextureRegion(CORE_END_TEXTURE_REGION_X, CORE_END_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_1 = TextureRegion(HEAD_PART_1_TEXTURE_REGION_X, HEAD_PART_1_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_2 = TextureRegion(HEAD_PART_2_TEXTURE_REGION_X, HEAD_PART_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_3 = TextureRegion(HEAD_PART_3_TEXTURE_REGION_X, HEAD_PART_3_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_4_POW_1 = TextureRegion(HEAD_PART_4_POW_1_TEXTURE_REGION_X, HEAD_PART_4_POW_1_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_4_POW_2 = TextureRegion(HEAD_PART_4_POW_2_TEXTURE_REGION_X, HEAD_PART_4_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_5_POW_2 = TextureRegion(HEAD_PART_5_POW_2_TEXTURE_REGION_X, HEAD_PART_5_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_HEAD_PART_6_POW_2 = TextureRegion(HEAD_PART_6_POW_2_TEXTURE_REGION_X, HEAD_PART_6_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_CORE_PART_1 = TextureRegion(EDGE_FROM_CORE_PART_1_TEXTURE_REGION_X, EDGE_FROM_CORE_PART_1_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_CORE_PART_2 = TextureRegion(EDGE_FROM_CORE_PART_2_TEXTURE_REGION_X, EDGE_FROM_CORE_PART_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_CORE_PART_3 = TextureRegion(EDGE_FROM_CORE_PART_3_TEXTURE_REGION_X, EDGE_FROM_CORE_PART_3_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_CORE_PART_4_POW_1 = TextureRegion(EDGE_FROM_CORE_PART_4_POW_1_TEXTURE_REGION_X, EDGE_FROM_CORE_PART_4_POW_1_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_CORE_PART_4_POW_2 = TextureRegion(EDGE_FROM_CORE_PART_4_POW_2_TEXTURE_REGION_X, EDGE_FROM_CORE_PART_4_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_CORE_PART_5_POW_2 = TextureRegion(EDGE_FROM_CORE_PART_5_POW_2_TEXTURE_REGION_X, EDGE_FROM_CORE_PART_5_POW_2_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_FAT_NECK = TextureRegion(EDGE_FROM_FAT_NECK_TEXTURE_REGION_X, EDGE_FROM_FAT_NECK_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_EDGE_FROM_THIN_NECK = TextureRegion(EDGE_FROM_THIN_NECK_TEXTURE_REGION_X, EDGE_FROM_THIN_NECK_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_FAT_NECK = TextureRegion(FAT_NECK_TEXTURE_REGION_X, FAT_NECK_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_THIN_NECK = TextureRegion(THIN_NECK_TEXTURE_REGION_X, THIN_NECK_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BODY = TextureRegion(BODY_TEXTURE_REGION_X, BODY_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_NECK_AND_BODY_END = TextureRegion(NECK_AND_BODY_END_TEXTURE_REGION_X, NECK_AND_BODY_END_TEXTURE_REGION_Y, FIRE_TEXTURE_REGION_WIDTH, FIRE_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    
    switch (fire.getFireType())
    {
        case CORE_PART_1:
            return TR_CORE_PART_1;
        case CORE_PART_2:
            return TR_CORE_PART_2;
        case CORE_PART_3:
            return TR_CORE_PART_3;
        case CORE_PART_4_POW_1:
            return TR_CORE_PART_4_POW_1;
        case CORE_PART_4_POW_2:
            return TR_CORE_PART_4_POW_2;
        case CORE_PART_5_POW_2:
            return TR_CORE_PART_5_POW_2;
        case CORE_PART_6_POW_2:
            return TR_CORE_PART_6_POW_2;
        case CORE_END:
            return TR_CORE_END;
        case HEAD_PART_1:
            return TR_HEAD_PART_1;
        case HEAD_PART_2:
            return TR_HEAD_PART_2;
        case HEAD_PART_3:
            return TR_HEAD_PART_3;
        case HEAD_PART_4_POW_1:
            return TR_HEAD_PART_4_POW_1;
        case HEAD_PART_4_POW_2:
            return TR_HEAD_PART_4_POW_2;
        case HEAD_PART_5_POW_2:
            return TR_HEAD_PART_5_POW_2;
        case HEAD_PART_6_POW_2:
            return TR_HEAD_PART_6_POW_2;
        case EDGE_FROM_CORE_PART_1:
            return TR_EDGE_FROM_CORE_PART_1;
        case EDGE_FROM_CORE_PART_2:
            return TR_EDGE_FROM_CORE_PART_2;
        case EDGE_FROM_CORE_PART_3:
            return TR_EDGE_FROM_CORE_PART_3;
        case EDGE_FROM_CORE_PART_4_POW_1:
            return TR_EDGE_FROM_CORE_PART_4_POW_1;
        case EDGE_FROM_CORE_PART_4_POW_2:
            return TR_EDGE_FROM_CORE_PART_4_POW_2;
        case EDGE_FROM_CORE_PART_5_POW_2:
            return TR_EDGE_FROM_CORE_PART_5_POW_2;
        case EDGE_FROM_FAT_NECK:
            return TR_EDGE_FROM_FAT_NECK;
        case EDGE_FROM_THIN_NECK:
            return TR_EDGE_FROM_THIN_NECK;
        case FAT_NECK:
            return TR_FAT_NECK;
        case THIN_NECK:
            return TR_THIN_NECK;
        case BODY:
            return TR_BODY;
        case NECK_AND_BODY_END:
        default:
            return TR_NECK_AND_BODY_END;
    }
}

TextureRegion Assets::getPowerUpBarItemTextureRegion(PowerUpBarItem &powerUpBarItem, float powerUpBarItemsStateTime)
{
    static std::vector<TextureRegion> powerUpBombTextureRegions;
    if (powerUpBombTextureRegions.size() == 0)
    {
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_1_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_2_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_3_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_4_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_5_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_6_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_7_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_8_TEXTURE_REGION_X, POWER_UP_ICON_BOMB_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> powerUpFireTextureRegions;
    if (powerUpFireTextureRegions.size() == 0)
    {
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_1_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_2_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_3_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_4_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_5_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_6_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_7_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_8_TEXTURE_REGION_X, POWER_UP_ICON_FIRE_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> powerUpSpeedTextureRegions;
    if (powerUpSpeedTextureRegions.size() == 0)
    {
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_1_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_2_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_3_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_4_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_5_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_6_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_7_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_ICON_FRAME_8_TEXTURE_REGION_X, POWER_UP_ICON_SPEED_TEXTURE_REGION_Y, POWER_UP_ICON_TEXTURE_REGION_WIDTH, POWER_UP_ICON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static float cycleTime = 1.60f;
    static std::vector<float> frames;
    if (frames.size() == 0)
    {
        frames.push_back(0.2f);
        frames.push_back(0.2f);
        frames.push_back(0.2f);
        frames.push_back(0.2f);
        frames.push_back(0.2f);
        frames.push_back(0.2f);
        frames.push_back(0.2f);
        frames.push_back(0.2f);
    }
    
    switch (powerUpBarItem.getPowerUpType())
    {
        case FIRE:
            return powerUpFireTextureRegions.at(getKeyFrameNumber(powerUpBarItemsStateTime, cycleTime, frames));
        case SPEED:
            return powerUpSpeedTextureRegions.at(getKeyFrameNumber(powerUpBarItemsStateTime, cycleTime, frames));
        case BOMB:
        case NONE:
        default:
            return powerUpBombTextureRegions.at(getKeyFrameNumber(powerUpBarItemsStateTime, cycleTime, frames));
    }
}

TextureRegion Assets::getActiveButtonTextureRegion(ActiveButton &activeButton, float buttonsStateTime)
{
    if(activeButton.getPowerUpType() == PUSH)
    {
        static TextureRegion TR_BUTTON_PUSH_ENABLED_TEXTURE_REGION = TextureRegion(BUTTON_PUSH_ENABLED_TEXTURE_REGION_X, BUTTON_PUSH_ENABLED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
        static TextureRegion TR_BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION = TextureRegion(BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION_X, BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
        static TextureRegion TR_BUTTON_PUSH_PRESSED_TEXTURE_REGION = TextureRegion(BUTTON_PUSH_PRESSED_TEXTURE_REGION_X, BUTTON_PUSH_PRESSED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
        static TextureRegion TR_BUTTON_PUSH_DISABLED_TEXTURE_REGION = TextureRegion(BUTTON_PUSH_DISABLED_TEXTURE_REGION_X, BUTTON_PUSH_DISABLED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
        
        if (activeButton.getButtonState() == ENABLED)
        {
            if(activeButton.isPressed())
            {
                return TR_BUTTON_PUSH_PRESSED_TEXTURE_REGION;
            }
            else
            {
                static std::vector<TextureRegion> bombButtonEnabledTextureRegions;
                if (bombButtonEnabledTextureRegions.size() == 0)
                {
                    bombButtonEnabledTextureRegions.push_back(TR_BUTTON_PUSH_ENABLED_TEXTURE_REGION);
                    bombButtonEnabledTextureRegions.push_back(TR_BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION);
                }
                
                static float cycleTime = 1.00f;
                static std::vector<float> frames;
                if (frames.size() == 0)
                {
                    frames.push_back(0.7f);
                    frames.push_back(0.3f);
                }
                
                return bombButtonEnabledTextureRegions.at(getKeyFrameNumber(buttonsStateTime, cycleTime, frames));
            }
        }
        else
        {
            return TR_BUTTON_PUSH_DISABLED_TEXTURE_REGION;
        }
    }
}

TextureRegion Assets::getBombButtonTextureRegion(BombButton &bombButton, float buttonsStateTime)
{
    static TextureRegion TR_BUTTON_BOMB_ENABLED_TEXTURE_REGION = TextureRegion(BUTTON_BOMB_ENABLED_TEXTURE_REGION_X, BUTTON_BOMB_ENABLED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION = TextureRegion(BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION_X, BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BUTTON_BOMB_PRESSED_TEXTURE_REGION = TextureRegion(BUTTON_BOMB_PRESSED_TEXTURE_REGION_X, BUTTON_BOMB_PRESSED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    static TextureRegion TR_BUTTON_BOMB_DISABLED_TEXTURE_REGION = TextureRegion(BUTTON_BOMB_DISABLED_TEXTURE_REGION_X, BUTTON_BOMB_DISABLED_TEXTURE_REGION_Y, BUTTON_TEXTURE_REGION_WIDTH, BUTTON_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT);
    
    if (bombButton.getButtonState() == ENABLED)
    {
        if(bombButton.isPressed())
        {
            return TR_BUTTON_BOMB_PRESSED_TEXTURE_REGION;
        }
        else
        {
            static std::vector<TextureRegion> bombButtonEnabledTextureRegions;
            if (bombButtonEnabledTextureRegions.size() == 0)
            {
                bombButtonEnabledTextureRegions.push_back(TR_BUTTON_BOMB_ENABLED_TEXTURE_REGION);
                bombButtonEnabledTextureRegions.push_back(TR_BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION);
            }
            
            static float cycleTime = 1.00f;
            static std::vector<float> frames;
            if (frames.size() == 0)
            {
                frames.push_back(0.7f);
                frames.push_back(0.3f);
            }
            
            return bombButtonEnabledTextureRegions.at(getKeyFrameNumber(buttonsStateTime, cycleTime, frames));
        }
    }
    else
    {
        return TR_BUTTON_BOMB_DISABLED_TEXTURE_REGION;
    }
}

TextureRegion Assets::getPowerUpTextureRegion(PowerUp &powerUp)
{
    static std::vector<TextureRegion> powerUpBombTextureRegions;
    if (powerUpBombTextureRegions.size() == 0)
    {
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_1_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_2_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_3_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_4_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_5_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_6_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_7_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpBombTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_8_TEXTURE_REGION_X, POWER_UP_BOMB_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> powerUpFireTextureRegions;
    if (powerUpFireTextureRegions.size() == 0)
    {
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_1_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_2_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_3_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_4_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_5_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_6_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_7_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpFireTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_8_TEXTURE_REGION_X, POWER_UP_FIRE_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> powerUpSpeedTextureRegions;
    if (powerUpSpeedTextureRegions.size() == 0)
    {
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_1_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_2_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_3_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_4_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_5_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_6_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_7_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpSpeedTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_8_TEXTURE_REGION_X, POWER_UP_SPEED_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static std::vector<TextureRegion> powerUpPushTextureRegions;
    if (powerUpPushTextureRegions.size() == 0)
    {
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_1_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_2_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_3_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_4_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_5_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_6_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_7_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
        powerUpPushTextureRegions.push_back(TextureRegion(POWER_UP_FRAME_8_TEXTURE_REGION_X, POWER_UP_PUSH_TEXTURE_REGION_Y, POWER_UP_TEXTURE_REGION_WIDTH, POWER_UP_TEXTURE_REGION_HEIGHT, GAME_TEXTURE_WIDTH, GAME_TEXTURE_HEIGHT));
    }
    
    static float cycleTime = 0.80f;
    static std::vector<float> frames;
    if (frames.size() == 0)
    {
        frames.push_back(0.1f);
        frames.push_back(0.1f);
        frames.push_back(0.1f);
        frames.push_back(0.1f);
        frames.push_back(0.1f);
        frames.push_back(0.1f);
        frames.push_back(0.1f);
        frames.push_back(0.1f);
    }

    switch (powerUp.getType())
    {
        case FIRE:
            return powerUpFireTextureRegions.at(getKeyFrameNumber(powerUp.getStateTime(), cycleTime, frames));
        case SPEED:
            return powerUpSpeedTextureRegions.at(getKeyFrameNumber(powerUp.getStateTime(), cycleTime, frames));
        case PUSH:
            return powerUpPushTextureRegions.at(getKeyFrameNumber(powerUp.getStateTime(), cycleTime, frames));
        case BOMB:
        case NONE:
        default:
            return powerUpBombTextureRegions.at(getKeyFrameNumber(powerUp.getStateTime(), cycleTime, frames));
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
    if (m_sSoundIds.size() > 0)
    {
        m_sSoundIds.erase(m_sSoundIds.begin());
    }
}

int Assets::getKeyFrameNumber(float stateTime, float cycleTime, std::vector<float> &frameDurations)
{
    if (stateTime > cycleTime && cycleTime > 0)
    {
        while (stateTime > cycleTime)
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