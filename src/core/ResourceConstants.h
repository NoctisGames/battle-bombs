//
//  ResourceConstants.h
//  battlebombs
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef RESOURCE_CONSTANTS_H
#define RESOURCE_CONSTANTS_H

//// Music Definitions ////

#define MUSIC_STOP 1
#define MUSIC_PLAY_MAP_SPACE 2
#define MUSIC_PLAY_MAP_GRASSLANDS 3
#define MUSIC_PLAY_MAP_MOUNTAINS 4
#define MUSIC_PLAY_MAP_BASE 5

//// Sound Definitions ////

#define SOUND_COUNT_DOWN_3 1
#define SOUND_COUNT_DOWN_2 2
#define SOUND_COUNT_DOWN_1 3
#define SOUND_BATTLE 4
#define SOUND_PLANT_BOMB 5
#define SOUND_EXPLOSION 6
#define SOUND_PU_BOMB 7
#define SOUND_PU_FIRE 8
#define SOUND_PU_SPEED 9
#define SOUND_PU_FORCE_FIELD 10
#define SOUND_PU_PUSH 11
#define SOUND_PU_SHIELD 12
#define SOUND_FORCE_FIELD_DOWN 13
#define SOUND_DEATH 14
#define SOUND_HURRY 15
#define SOUND_GAME_SET 16
#define SOUND_DRAW 17

//// Texture Definitions ////

#define GAME_TEXTURE_WIDTH 1024
#define GAME_TEXTURE_HEIGHT 1024

//// Texture Region Definitions ////

/// Maps

#define WORLD_BACKGROUND_TEXTURE_REGION_X 4
#define WORLD_BACKGROUND_TEXTURE_REGION_Y 4
#define WORLD_BACKGROUND_TEXTURE_REGION_WIDTH 536
#define WORLD_BACKGROUND_TEXTURE_REGION_HEIGHT 640

#define SPACE_TILE_TEXTURE_REGION_X 624
#define SPACE_TILE_TEXTURE_REGION_Y 8
#define SPACE_TILE_TEXTURE_REGION_WIDTH 32
#define SPACE_TILE_TEXTURE_REGION_HEIGHT 48

#define MOUNTAINS_DOOR_TEXTURE_REGION_X 696
#define MOUNTAINS_DOOR_TEXTURE_REGION_Y 488
#define MOUNTAINS_DOOR_TEXTURE_REGION_WIDTH 96
#define MOUNTAINS_DOOR_TEXTURE_REGION_HEIGHT 96

#define BORDER_TOP_TEXTURE_REGION_X 4
#define BORDER_TOP_TEXTURE_REGION_Y 656
#define BORDER_TOP_TEXTURE_REGION_WIDTH 536
#define BORDER_TOP_TEXTURE_REGION_HEIGHT 65

#define BORDER_LEFT_TEXTURE_REGION_X 548
#define BORDER_LEFT_TEXTURE_REGION_Y 36
#define BORDER_LEFT_TEXTURE_REGION_WIDTH 33
#define BORDER_LEFT_TEXTURE_REGION_HEIGHT 448

#define BORDER_RIGHT_TEXTURE_REGION_X 587
#define BORDER_RIGHT_TEXTURE_REGION_Y 36
#define BORDER_RIGHT_TEXTURE_REGION_WIDTH 25
#define BORDER_RIGHT_TEXTURE_REGION_HEIGHT 448

#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_X 548
#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_Y 604
#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_WIDTH 128
#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_HEIGHT 96

#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_X 684
#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_Y 604
#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_WIDTH 120
#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_HEIGHT 96

#define BORDER_BOTTOM_TEXTURE_REGION_X 4
#define BORDER_BOTTOM_TEXTURE_REGION_Y 728
#define BORDER_BOTTOM_TEXTURE_REGION_WIDTH 536
#define BORDER_BOTTOM_TEXTURE_REGION_HEIGHT 32

#define INSIDE_BLOCK_TEXTURE_REGION_X 496
#define INSIDE_BLOCK_TEXTURE_REGION_Y 912
#define INSIDE_BLOCK_TEXTURE_REGION_WIDTH 64
#define INSIDE_BLOCK_TEXTURE_REGION_HEIGHT 64

#define BREAKABLE_BLOCK_TEXTURE_REGION_X 608
#define BREAKABLE_BLOCK_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_X 604
#define BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_X 800
#define BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_X 896
#define BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_X 608
#define BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_X 704
#define BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_X 800
#define BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_X 896
#define BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_X 608
#define BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_Y 920
#define BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_X 704
#define BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_Y 920
#define BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_X 800
#define BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_Y 920
#define BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH 96
#define BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT 96

// Sudden Death Mountains

#define FALLING_ICE_FRAME_1_TEXTURE_REGION_X 624
#define FALLING_ICE_FRAME_2_TEXTURE_REGION_X 688
#define FALLING_ICE_FRAME_3_TEXTURE_REGION_X 752
#define FALLING_ICE_FRAME_4_TEXTURE_REGION_X 816
#define FALLING_ICE_FRAME_5_TEXTURE_REGION_X 880
#define FALLING_ICE_FRAMES_TEXTURE_REGION_Y 8
#define FALLING_ICE_FRAME_TEXTURE_REGION_WIDTH 64
#define FALLING_ICE_FRAME_TEXTURE_REGION_HEIGHT 64

#define FALLING_ICE_SHADOW_FRAME_1_TEXTURE_REGION_X 624
#define FALLING_ICE_SHADOW_FRAME_2_TEXTURE_REGION_X 688
#define FALLING_ICE_SHADOW_FRAME_3_TEXTURE_REGION_X 752
#define FALLING_ICE_SHADOW_FRAME_4_TEXTURE_REGION_X 816
#define FALLING_ICE_SHADOW_FRAME_5_TEXTURE_REGION_X 880
#define FALLING_ICE_SHADOW_FRAMES_TEXTURE_REGION_Y 72
#define FALLING_ICE_SHADOW_FRAME_TEXTURE_REGION_WIDTH 64
#define FALLING_ICE_SHADOW_FRAME_TEXTURE_REGION_HEIGHT 64

#define ICE_PATCH_FRAME_1_TEXTURE_REGION_X 624
#define ICE_PATCH_FRAME_2_TEXTURE_REGION_X 688
#define ICE_PATCH_FRAME_3_TEXTURE_REGION_X 752
#define ICE_PATCH_FRAME_4_TEXTURE_REGION_X 816
#define ICE_PATCH_FRAME_5_TEXTURE_REGION_X 880
#define ICE_PATCH_FRAME_6_TEXTURE_REGION_X 944
#define ICE_PATCH_FRAMES_TEXTURE_REGION_Y 136
#define ICE_PATCH_FRAME_TEXTURE_REGION_WIDTH 64
#define ICE_PATCH_FRAME_TEXTURE_REGION_HEIGHT 64

#define FREEZING_INSIDE_BLOCK_FRAME_1_TEXTURE_REGION_X 624
#define FREEZING_INSIDE_BLOCK_FRAME_2_TEXTURE_REGION_X 688
#define FREEZING_INSIDE_BLOCK_FRAME_3_TEXTURE_REGION_X 752
#define FREEZING_INSIDE_BLOCK_FRAME_4_TEXTURE_REGION_X 816
#define FREEZING_INSIDE_BLOCK_FRAME_5_TEXTURE_REGION_X 880
#define FREEZING_INSIDE_BLOCK_FRAME_6_TEXTURE_REGION_X 944
#define FREEZING_INSIDE_BLOCK_FRAMES_TEXTURE_REGION_Y 200
#define FREEZING_INSIDE_BLOCK_FRAME_TEXTURE_REGION_WIDTH 64
#define FREEZING_INSIDE_BLOCK_FRAME_TEXTURE_REGION_HEIGHT 64

#define FREEZING_BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_X 624
#define FREEZING_BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_Y 264
#define FREEZING_BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_X 720
#define FREEZING_BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_Y 264
#define FREEZING_BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_X 816
#define FREEZING_BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_Y 264
#define FREEZING_BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_X 912
#define FREEZING_BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_Y 264
#define FREEZING_BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_X 624
#define FREEZING_BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_Y 360
#define FREEZING_BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_X 720
#define FREEZING_BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_Y 360
#define FREEZING_BREAKABLE_BLOCK_FRAME_TEXTURE_REGION_WIDTH 96
#define FREEZING_BREAKABLE_BLOCK_FRAME_TEXTURE_REGION_HEIGHT 96

/// Game

// Bombs

#define BOMB_FRAME_1_TEXTURE_REGION_X 888
#define BOMB_FRAME_1_TEXTURE_REGION_Y 128
#define BOMB_FRAME_2_TEXTURE_REGION_X 888
#define BOMB_FRAME_2_TEXTURE_REGION_Y 192
#define BOMB_FRAME_3_TEXTURE_REGION_X 888
#define BOMB_FRAME_3_TEXTURE_REGION_Y 256
#define BOMB_FRAME_4_TEXTURE_REGION_X 888
#define BOMB_FRAME_4_TEXTURE_REGION_Y 320
#define BOMB_FRAME_5_TEXTURE_REGION_X 888
#define BOMB_FRAME_5_TEXTURE_REGION_Y 384
#define BOMB_FRAME_6_TEXTURE_REGION_X 952
#define BOMB_FRAME_6_TEXTURE_REGION_Y 128
#define BOMB_FRAME_7_TEXTURE_REGION_X 952
#define BOMB_FRAME_7_TEXTURE_REGION_Y 192
#define BOMB_EXPLOSION_FRAME_1_TEXTURE_REGION_X 952
#define BOMB_EXPLOSION_FRAME_1_TEXTURE_REGION_Y 256
#define BOMB_EXPLOSION_FRAME_2_TEXTURE_REGION_X 952
#define BOMB_EXPLOSION_FRAME_2_TEXTURE_REGION_Y 320
#define BOMB_TEXTURE_REGION_WIDTH 64
#define BOMB_TEXTURE_REGION_HEIGHT 64

// Explosions

#define CORE_PART_1_TEXTURE_REGION_X 688
#define CORE_PART_1_TEXTURE_REGION_Y 8
#define CORE_PART_2_TEXTURE_REGION_X 768
#define CORE_PART_2_TEXTURE_REGION_Y 8
#define CORE_PART_3_TEXTURE_REGION_X 848
#define CORE_PART_3_TEXTURE_REGION_Y 8

#define CORE_PART_4_POW_1_TEXTURE_REGION_X 928
#define CORE_PART_4_POW_1_TEXTURE_REGION_Y 8
#define CORE_PART_4_POW_2_TEXTURE_REGION_X 688
#define CORE_PART_4_POW_2_TEXTURE_REGION_Y 176
#define CORE_PART_5_POW_2_TEXTURE_REGION_X 768
#define CORE_PART_5_POW_2_TEXTURE_REGION_Y 176
#define CORE_PART_6_POW_2_TEXTURE_REGION_X 848
#define CORE_PART_6_POW_2_TEXTURE_REGION_Y 176

#define CORE_END_TEXTURE_REGION_X 968
#define CORE_END_TEXTURE_REGION_Y 8

#define HEAD_PART_1_TEXTURE_REGION_X 688
#define HEAD_PART_1_TEXTURE_REGION_Y 48
#define HEAD_PART_2_TEXTURE_REGION_X 768
#define HEAD_PART_2_TEXTURE_REGION_Y 48
#define HEAD_PART_3_TEXTURE_REGION_X 848
#define HEAD_PART_3_TEXTURE_REGION_Y 48

#define HEAD_PART_4_POW_1_TEXTURE_REGION_X 928
#define HEAD_PART_4_POW_1_TEXTURE_REGION_Y 48
#define HEAD_PART_4_POW_2_TEXTURE_REGION_X 688
#define HEAD_PART_4_POW_2_TEXTURE_REGION_Y 216
#define HEAD_PART_5_POW_2_TEXTURE_REGION_X 768
#define HEAD_PART_5_POW_2_TEXTURE_REGION_Y 216
#define HEAD_PART_6_POW_2_TEXTURE_REGION_X 848
#define HEAD_PART_6_POW_2_TEXTURE_REGION_Y 216

#define EDGE_FROM_CORE_PART_1_TEXTURE_REGION_X 688
#define EDGE_FROM_CORE_PART_1_TEXTURE_REGION_Y 88
#define EDGE_FROM_CORE_PART_2_TEXTURE_REGION_X 768
#define EDGE_FROM_CORE_PART_2_TEXTURE_REGION_Y 88
#define EDGE_FROM_CORE_PART_3_TEXTURE_REGION_X 848
#define EDGE_FROM_CORE_PART_3_TEXTURE_REGION_Y 88

#define EDGE_FROM_CORE_PART_4_POW_1_TEXTURE_REGION_X 928
#define EDGE_FROM_CORE_PART_4_POW_1_TEXTURE_REGION_Y 88
#define EDGE_FROM_CORE_PART_4_POW_2_TEXTURE_REGION_X 688
#define EDGE_FROM_CORE_PART_4_POW_2_TEXTURE_REGION_Y 336
#define EDGE_FROM_CORE_PART_5_POW_2_TEXTURE_REGION_X 768
#define EDGE_FROM_CORE_PART_5_POW_2_TEXTURE_REGION_Y 336

#define EDGE_FROM_FAT_NECK_TEXTURE_REGION_X 688
#define EDGE_FROM_FAT_NECK_TEXTURE_REGION_Y 376
#define EDGE_FROM_THIN_NECK_TEXTURE_REGION_X 768
#define EDGE_FROM_THIN_NECK_TEXTURE_REGION_Y 376

#define FAT_NECK_TEXTURE_REGION_X 688
#define FAT_NECK_TEXTURE_REGION_Y 256
#define THIN_NECK_TEXTURE_REGION_X 768
#define THIN_NECK_TEXTURE_REGION_Y 256

#define BODY_TEXTURE_REGION_X 688
#define BODY_TEXTURE_REGION_Y 296

#define NECK_AND_BODY_END_TEXTURE_REGION_X 848
#define NECK_AND_BODY_END_TEXTURE_REGION_Y 256

#define FIRE_TEXTURE_REGION_WIDTH 32
#define FIRE_TEXTURE_REGION_HEIGHT 32

// Power Ups

#define POWER_UP_SHIELD_TEXTURE_REGION_Y 464
#define POWER_UP_BOMB_TEXTURE_REGION_Y 504
#define POWER_UP_FIRE_TEXTURE_REGION_Y 544
#define POWER_UP_FORCE_FIELD_TEXTURE_REGION_Y 584
#define POWER_UP_SPEED_TEXTURE_REGION_Y 624
#define POWER_UP_PUSH_TEXTURE_REGION_Y 664
#define POWER_UP_FRAME_1_TEXTURE_REGION_X 856
#define POWER_UP_FRAME_2_TEXTURE_REGION_X 888
#define POWER_UP_FRAME_3_TEXTURE_REGION_X 920
#define POWER_UP_FRAME_4_TEXTURE_REGION_X 952
#define POWER_UP_FRAME_5_TEXTURE_REGION_X 984
#define POWER_UP_FRAME_6_TEXTURE_REGION_X 952
#define POWER_UP_FRAME_7_TEXTURE_REGION_X 920
#define POWER_UP_FRAME_8_TEXTURE_REGION_X 888
#define POWER_UP_TEXTURE_REGION_WIDTH 32
#define POWER_UP_TEXTURE_REGION_HEIGHT 32

/// Player/Bot

#define PLAYER_TEXTURE_REGION_WIDTH 64
#define PLAYER_TEXTURE_REGION_HEIGHT 64
#define PLAYER_TEXTURE_REGION_INSET 8

#define PLAYER_IDLE_WINNING_TEXTURE_REGION_Y 64 * 0 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_IDLE_FRAME_1_TEXTURE_REGION_X 64 * 0 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_IDLE_FRAME_2_TEXTURE_REGION_X 64 * 1 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_IDLE_FRAME_3_TEXTURE_REGION_X 64 * 2 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_IDLE_FRAME_4_TEXTURE_REGION_X 64 * 3 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_WINNING_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WINNING_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WINNING_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WINNING_FRAME_4_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WINNING_FRAME_5_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WINNING_FRAME_6_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET

#define PLAYER_WALK_RIGHT_TEXTURE_REGION_Y 64 * 1 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_UP_TEXTURE_REGION_Y 64 * 2 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_LEFT_TEXTURE_REGION_Y 64 * 3 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_DOWN_TEXTURE_REGION_Y 64 * 4 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_WALK_IDLE_TEXTURE_REGION_X 64 * 0 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_1_TEXTURE_REGION_X 64 * 1 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_2_TEXTURE_REGION_X 64 * 2 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_3_TEXTURE_REGION_X 64 * 3 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_4_TEXTURE_REGION_X 64 * 4 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_5_TEXTURE_REGION_X 64 * 5 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_6_TEXTURE_REGION_X 64 * 6 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_7_TEXTURE_REGION_X 64 * 7 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_WALK_FRAME_8_TEXTURE_REGION_X 64 * 8 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y 64 * 1 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_UP_TEXTURE_REGION_Y 64 * 2 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_LEFT_TEXTURE_REGION_Y 64 * 3 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_DOWN_TEXTURE_REGION_Y 64 * 4 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PLACE_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_Y 64 * 8 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_Y 64 * 8 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_Y 64 * 8 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PLACE_BOMB_UP_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_UP_FRAME_1_TEXTURE_REGION_Y 64 * 7 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_UP_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_UP_FRAME_2_TEXTURE_REGION_Y 64 * 7 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_UP_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_UP_FRAME_3_TEXTURE_REGION_Y 64 * 7 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PLACE_BOMB_LEFT_FRAME_1_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_LEFT_FRAME_1_TEXTURE_REGION_Y 64 * 8 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_LEFT_FRAME_2_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_LEFT_FRAME_2_TEXTURE_REGION_Y 64 * 8 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_LEFT_FRAME_3_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_LEFT_FRAME_3_TEXTURE_REGION_Y 64 * 8 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PLACE_BOMB_DOWN_FRAME_1_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_DOWN_FRAME_1_TEXTURE_REGION_Y 64 * 7 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_DOWN_FRAME_2_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_DOWN_FRAME_2_TEXTURE_REGION_Y 64 * 7 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_DOWN_FRAME_3_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PLACE_BOMB_DOWN_FRAME_3_TEXTURE_REGION_Y 64 * 7 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PUSH_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_Y 64 * 6 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_Y 64 * 6 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_Y 64 * 6 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PUSH_BOMB_UP_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_UP_FRAME_1_TEXTURE_REGION_Y 64 * 5 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_UP_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_UP_FRAME_2_TEXTURE_REGION_Y 64 * 5 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_UP_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_UP_FRAME_3_TEXTURE_REGION_Y 64 * 5 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PUSH_BOMB_LEFT_FRAME_1_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_LEFT_FRAME_1_TEXTURE_REGION_Y 64 * 6 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_LEFT_FRAME_2_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_LEFT_FRAME_2_TEXTURE_REGION_Y 64 * 6 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_LEFT_FRAME_3_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_LEFT_FRAME_3_TEXTURE_REGION_Y 64 * 6 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_PUSH_BOMB_DOWN_FRAME_1_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_DOWN_FRAME_1_TEXTURE_REGION_Y 64 * 5 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_DOWN_FRAME_2_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_DOWN_FRAME_2_TEXTURE_REGION_Y 64 * 5 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_DOWN_FRAME_3_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_PUSH_BOMB_DOWN_FRAME_3_TEXTURE_REGION_Y 64 * 5 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_RAISE_SHIELD_RIGHT_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_RIGHT_FRAME_1_TEXTURE_REGION_Y 64 * 10 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_RIGHT_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_RIGHT_FRAME_2_TEXTURE_REGION_Y 64 * 10 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_RIGHT_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_RIGHT_FRAME_3_TEXTURE_REGION_Y 64 * 10 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_RAISE_SHIELD_UP_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_UP_FRAME_1_TEXTURE_REGION_Y 64 * 9 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_UP_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_UP_FRAME_2_TEXTURE_REGION_Y 64 * 9 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_UP_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_UP_FRAME_3_TEXTURE_REGION_Y 64 * 9 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_RAISE_SHIELD_LEFT_FRAME_1_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_LEFT_FRAME_1_TEXTURE_REGION_Y 64 * 10 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_LEFT_FRAME_2_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_LEFT_FRAME_2_TEXTURE_REGION_Y 64 * 10 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_LEFT_FRAME_3_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_LEFT_FRAME_3_TEXTURE_REGION_Y 64 * 10 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_RAISE_SHIELD_DOWN_FRAME_1_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_DOWN_FRAME_1_TEXTURE_REGION_Y 64 * 9 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_DOWN_FRAME_2_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_DOWN_FRAME_2_TEXTURE_REGION_Y 64 * 9 + PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_DOWN_FRAME_3_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_RAISE_SHIELD_DOWN_FRAME_3_TEXTURE_REGION_Y 64 * 9 + PLAYER_TEXTURE_REGION_INSET

#define PLAYER_FREEZING_FRAME_1_TEXTURE_REGION_X 64 * 10 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_FREEZING_FRAME_2_TEXTURE_REGION_X 64 * 11 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_FREEZING_FRAME_3_TEXTURE_REGION_X 64 * 12 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_FREEZING_FRAME_4_TEXTURE_REGION_X 64 * 13 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_FREEZING_FRAME_5_TEXTURE_REGION_X 64 * 14 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_FREEZING_FRAME_6_TEXTURE_REGION_X 64 * 15 - PLAYER_TEXTURE_REGION_INSET
#define PLAYER_FREEZING_FRAMES_TEXTURE_REGION_Y 64 * 11 + PLAYER_TEXTURE_REGION_INSET

/// Interface

#define FONT_TEXTURE_REGION_X 8
#define FONT_TEXTURE_REGION_Y 248
#define FONT_GLYPHS_PER_ROW 16
#define FONT_GLYPH_WIDTH 38
#define FONT_GLYPH_HEIGHT 26

#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_X 480
#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_Y 216
#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_WIDTH 536
#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_HEIGHT 42

#define SPECTATOR_CONTROLS_LEFT_ARROW_HIGHLIGHTED_TEXTURE_REGION_X 902
#define SPECTATOR_CONTROLS_LEFT_ARROW_HIGHLIGHTED_TEXTURE_REGION_Y 120
#define SPECTATOR_CONTROLS_RIGHT_ARROW_HIGHLIGHTED_TEXTURE_REGION_X 902
#define SPECTATOR_CONTROLS_RIGHT_ARROW_HIGHLIGHTED_TEXTURE_REGION_Y 168
#define SPECTATOR_CONTROLS_NOTHING_HIGHLIGHTED_TEXTURE_REGION_X 902
#define SPECTATOR_CONTROLS_NOTHING_HIGHLIGHTED_TEXTURE_REGION_Y 216
#define SPECTATOR_CONTROLS_TEXTURE_REGION_WIDTH 114
#define SPECTATOR_CONTROLS_TEXTURE_REGION_HEIGHT 42

#define INTERFACE_OVERLAY_TEXTURE_REGION_X 480
#define INTERFACE_OVERLAY_TEXTURE_REGION_Y 696
#define INTERFACE_OVERLAY_TEXTURE_REGION_WIDTH 536
#define INTERFACE_OVERLAY_TEXTURE_REGION_HEIGHT 320

#define D_PAD_RIGHT_TEXTURE_REGION_X 8
#define D_PAD_RIGHT_TEXTURE_REGION_Y 712
#define D_PAD_RIGHT_TEXTURE_REGION_WIDTH 132
#define D_PAD_RIGHT_TEXTURE_REGION_HEIGHT 144

#define D_PAD_UP_TEXTURE_REGION_X 152
#define D_PAD_UP_TEXTURE_REGION_Y 712
#define D_PAD_UP_TEXTURE_REGION_WIDTH 132
#define D_PAD_UP_TEXTURE_REGION_HEIGHT 144

#define D_PAD_LEFT_TEXTURE_REGION_X 8
#define D_PAD_LEFT_TEXTURE_REGION_Y 872
#define D_PAD_LEFT_TEXTURE_REGION_WIDTH 132
#define D_PAD_LEFT_TEXTURE_REGION_HEIGHT 144

#define D_PAD_DOWN_TEXTURE_REGION_X 152
#define D_PAD_DOWN_TEXTURE_REGION_Y 872
#define D_PAD_DOWN_TEXTURE_REGION_WIDTH 132
#define D_PAD_DOWN_TEXTURE_REGION_HEIGHT 144

#define D_PAD_NO_DIRECTION_TEXTURE_REGION_X 480
#define D_PAD_NO_DIRECTION_TEXTURE_REGION_Y 872
#define D_PAD_NO_DIRECTION_TEXTURE_REGION_WIDTH 132
#define D_PAD_NO_DIRECTION_TEXTURE_REGION_HEIGHT 144

#define POWER_UP_ICON_BOMB_TEXTURE_REGION_Y 72
#define POWER_UP_ICON_FIRE_TEXTURE_REGION_Y 40
#define POWER_UP_ICON_FORCE_FIELD_TEXTURE_REGION_Y 104
#define POWER_UP_ICON_SPEED_TEXTURE_REGION_Y 8
#define POWER_UP_ICON_FRAME_1_TEXTURE_REGION_X 272
#define POWER_UP_ICON_FRAME_2_TEXTURE_REGION_X 304
#define POWER_UP_ICON_FRAME_3_TEXTURE_REGION_X 336
#define POWER_UP_ICON_FRAME_4_TEXTURE_REGION_X 368
#define POWER_UP_ICON_FRAME_5_TEXTURE_REGION_X 400
#define POWER_UP_ICON_FRAME_6_TEXTURE_REGION_X 368
#define POWER_UP_ICON_FRAME_7_TEXTURE_REGION_X 336
#define POWER_UP_ICON_FRAME_8_TEXTURE_REGION_X 304
#define POWER_UP_ICON_TEXTURE_REGION_WIDTH 32
#define POWER_UP_ICON_TEXTURE_REGION_HEIGHT 32

#define BUTTON_PUSH_DISABLED_TEXTURE_REGION_X 636
#define BUTTON_PUSH_DISABLED_TEXTURE_REGION_Y 360
#define BUTTON_PUSH_ENABLED_TEXTURE_REGION_X 700
#define BUTTON_PUSH_ENABLED_TEXTURE_REGION_Y 360
#define BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION_X 764
#define BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION_Y 360
#define BUTTON_PUSH_PRESSED_TEXTURE_REGION_X 828
#define BUTTON_PUSH_PRESSED_TEXTURE_REGION_Y 360

#define BUTTON_SHIELD_DISABLED_TEXTURE_REGION_X 636
#define BUTTON_SHIELD_DISABLED_TEXTURE_REGION_Y 424
#define BUTTON_SHIELD_ENABLED_TEXTURE_REGION_X 700
#define BUTTON_SHIELD_ENABLED_TEXTURE_REGION_Y 424
#define BUTTON_SHIELD_HIGHLIGHTED_TEXTURE_REGION_X 764
#define BUTTON_SHIELD_HIGHLIGHTED_TEXTURE_REGION_Y 424
#define BUTTON_SHIELD_PRESSED_TEXTURE_REGION_X 828
#define BUTTON_SHIELD_PRESSED_TEXTURE_REGION_Y 424

#define BUTTON_BOMB_DISABLED_TEXTURE_REGION_X 636
#define BUTTON_BOMB_DISABLED_TEXTURE_REGION_Y 296
#define BUTTON_BOMB_ENABLED_TEXTURE_REGION_X 700
#define BUTTON_BOMB_ENABLED_TEXTURE_REGION_Y 296
#define BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION_X 764
#define BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION_Y 296
#define BUTTON_BOMB_PRESSED_TEXTURE_REGION_X 828
#define BUTTON_BOMB_PRESSED_TEXTURE_REGION_Y 296

#define BUTTON_TEXTURE_REGION_WIDTH 64
#define BUTTON_TEXTURE_REGION_HEIGHT 64

#define FORCE_FIELD_ACTIVE_FRAME_1_TEXTURE_REGION_X 436
#define FORCE_FIELD_ACTIVE_FRAME_1_TEXTURE_REGION_Y 4
#define FORCE_FIELD_ACTIVE_FRAME_2_TEXTURE_REGION_X 500
#define FORCE_FIELD_ACTIVE_FRAME_2_TEXTURE_REGION_Y 4
#define FORCE_FIELD_ACTIVE_FRAME_3_TEXTURE_REGION_X 564
#define FORCE_FIELD_ACTIVE_FRAME_3_TEXTURE_REGION_Y 4
#define FORCE_FIELD_ACTIVE_FRAME_4_TEXTURE_REGION_X 628
#define FORCE_FIELD_ACTIVE_FRAME_4_TEXTURE_REGION_Y 4
#define FORCE_FIELD_ACTIVE_FRAME_5_TEXTURE_REGION_X 692
#define FORCE_FIELD_ACTIVE_FRAME_5_TEXTURE_REGION_Y 4
#define FORCE_FIELD_ACTIVE_FRAME_6_TEXTURE_REGION_X 756
#define FORCE_FIELD_ACTIVE_FRAME_6_TEXTURE_REGION_Y 4
#define FORCE_FIELD_ACTIVE_FRAME_7_TEXTURE_REGION_X 820
#define FORCE_FIELD_ACTIVE_FRAME_7_TEXTURE_REGION_Y 4

#define FORCE_FIELD_STARTING_FRAME_1_TEXTURE_REGION_X 436
#define FORCE_FIELD_STARTING_FRAME_1_TEXTURE_REGION_Y 68
#define FORCE_FIELD_STARTING_FRAME_2_TEXTURE_REGION_X 500
#define FORCE_FIELD_STARTING_FRAME_2_TEXTURE_REGION_Y 68
#define FORCE_FIELD_STARTING_FRAME_3_TEXTURE_REGION_X 564
#define FORCE_FIELD_STARTING_FRAME_3_TEXTURE_REGION_Y 68
#define FORCE_FIELD_STARTING_FRAME_4_TEXTURE_REGION_X 628
#define FORCE_FIELD_STARTING_FRAME_4_TEXTURE_REGION_Y 68
#define FORCE_FIELD_STARTING_FRAME_5_TEXTURE_REGION_X 692
#define FORCE_FIELD_STARTING_FRAME_5_TEXTURE_REGION_Y 68

#define FORCE_FIELD_BREAKING_FRAME_1_TEXTURE_REGION_X 436
#define FORCE_FIELD_BREAKING_FRAME_1_TEXTURE_REGION_Y 132
#define FORCE_FIELD_BREAKING_FRAME_2_TEXTURE_REGION_X 500
#define FORCE_FIELD_BREAKING_FRAME_2_TEXTURE_REGION_Y 132
#define FORCE_FIELD_BREAKING_FRAME_3_TEXTURE_REGION_X 564
#define FORCE_FIELD_BREAKING_FRAME_3_TEXTURE_REGION_Y 132
#define FORCE_FIELD_BREAKING_FRAME_4_TEXTURE_REGION_X 628
#define FORCE_FIELD_BREAKING_FRAME_4_TEXTURE_REGION_Y 132
#define FORCE_FIELD_BREAKING_FRAME_5_TEXTURE_REGION_X 692
#define FORCE_FIELD_BREAKING_FRAME_5_TEXTURE_REGION_Y 132
#define FORCE_FIELD_BREAKING_FRAME_6_TEXTURE_REGION_X 756
#define FORCE_FIELD_BREAKING_FRAME_6_TEXTURE_REGION_Y 132
#define FORCE_FIELD_BREAKING_FRAME_7_TEXTURE_REGION_X 820
#define FORCE_FIELD_BREAKING_FRAME_7_TEXTURE_REGION_Y 132

#define FORCE_FIELD_TEXTURE_REGION_WIDTH 64
#define FORCE_FIELD_TEXTURE_REGION_HEIGHT 64

#define INTERFACE_PLAYER_AVATAR_YELLOW_ALIVE_TEXTURE_REGION_X 8
#define INTERFACE_PLAYER_AVATAR_YELLOW_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_WHITE_ALIVE_TEXTURE_REGION_X 40
#define INTERFACE_PLAYER_AVATAR_WHITE_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_RED_ALIVE_TEXTURE_REGION_X 72
#define INTERFACE_PLAYER_AVATAR_RED_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_PINK_ALIVE_TEXTURE_REGION_X 104
#define INTERFACE_PLAYER_AVATAR_PINK_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_ORANGE_ALIVE_TEXTURE_REGION_X 136
#define INTERFACE_PLAYER_AVATAR_ORANGE_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_GREEN_ALIVE_TEXTURE_REGION_X 168
#define INTERFACE_PLAYER_AVATAR_GREEN_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_BLUE_ALIVE_TEXTURE_REGION_X 200
#define INTERFACE_PLAYER_AVATAR_BLUE_ALIVE_TEXTURE_REGION_Y 8
#define INTERFACE_PLAYER_AVATAR_BLACK_ALIVE_TEXTURE_REGION_X 232
#define INTERFACE_PLAYER_AVATAR_BLACK_ALIVE_TEXTURE_REGION_Y 8

#define INTERFACE_PLAYER_AVATAR_YELLOW_DEAD_TEXTURE_REGION_X 8
#define INTERFACE_PLAYER_AVATAR_YELLOW_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_WHITE_DEAD_TEXTURE_REGION_X 40
#define INTERFACE_PLAYER_AVATAR_WHITE_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_RED_DEAD_TEXTURE_REGION_X 72
#define INTERFACE_PLAYER_AVATAR_RED_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_PINK_DEAD_TEXTURE_REGION_X 104
#define INTERFACE_PLAYER_AVATAR_PINK_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_ORANGE_DEAD_TEXTURE_REGION_X 136
#define INTERFACE_PLAYER_AVATAR_ORANGE_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_GREEN_DEAD_TEXTURE_REGION_X 168
#define INTERFACE_PLAYER_AVATAR_GREEN_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_BLUE_DEAD_TEXTURE_REGION_X 200
#define INTERFACE_PLAYER_AVATAR_BLUE_DEAD_TEXTURE_REGION_Y 40
#define INTERFACE_PLAYER_AVATAR_BLACK_DEAD_TEXTURE_REGION_X 232
#define INTERFACE_PLAYER_AVATAR_BLACK_DEAD_TEXTURE_REGION_Y 40

#define INTERFACE_BOT_AVATAR_BLUE_ALIVE_TEXTURE_REGION_X 8
#define INTERFACE_BOT_AVATAR_BLUE_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_RED_ALIVE_TEXTURE_REGION_X 40
#define INTERFACE_BOT_AVATAR_RED_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_GREEN_ALIVE_TEXTURE_REGION_X 72
#define INTERFACE_BOT_AVATAR_GREEN_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_BLACK_ALIVE_TEXTURE_REGION_X 104
#define INTERFACE_BOT_AVATAR_BLACK_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_WHITE_ALIVE_TEXTURE_REGION_X 136
#define INTERFACE_BOT_AVATAR_WHITE_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_PINK_ALIVE_TEXTURE_REGION_X 168
#define INTERFACE_BOT_AVATAR_PINK_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_ORANGE_ALIVE_TEXTURE_REGION_X 200
#define INTERFACE_BOT_AVATAR_ORANGE_ALIVE_TEXTURE_REGION_Y 72
#define INTERFACE_BOT_AVATAR_YELLOW_ALIVE_TEXTURE_REGION_X 232
#define INTERFACE_BOT_AVATAR_YELLOW_ALIVE_TEXTURE_REGION_Y 72

#define INTERFACE_BOT_AVATAR_BLUE_DEAD_TEXTURE_REGION_X 8
#define INTERFACE_BOT_AVATAR_BLUE_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_RED_DEAD_TEXTURE_REGION_X 40
#define INTERFACE_BOT_AVATAR_RED_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_GREEN_DEAD_TEXTURE_REGION_X 72
#define INTERFACE_BOT_AVATAR_GREEN_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_BLACK_DEAD_TEXTURE_REGION_X 104
#define INTERFACE_BOT_AVATAR_BLACK_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_WHITE_DEAD_TEXTURE_REGION_X 136
#define INTERFACE_BOT_AVATAR_WHITE_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_PINK_DEAD_TEXTURE_REGION_X 168
#define INTERFACE_BOT_AVATAR_PINK_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_ORANGE_DEAD_TEXTURE_REGION_X 200
#define INTERFACE_BOT_AVATAR_ORANGE_DEAD_TEXTURE_REGION_Y 104
#define INTERFACE_BOT_AVATAR_YELLOW_DEAD_TEXTURE_REGION_X 232
#define INTERFACE_BOT_AVATAR_YELLOW_DEAD_TEXTURE_REGION_Y 104

#define INTERFACE_PLAYER_AVATAR_TEXTURE_REGION_WIDTH 32
#define INTERFACE_PLAYER_AVATAR_TEXTURE_REGION_HEIGHT 32

#define PLAYER_NAME_BUBBLE_TEXTURE_REGION_X 648
#define PLAYER_NAME_BUBBLE_TEXTURE_REGION_Y 512
#define PLAYER_NAME_BUBBLE_TEXTURE_REGION_WIDTH 64
#define PLAYER_NAME_BUBBLE_TEXTURE_REGION_HEIGHT 16

#define SPECTATING_WHO_ARROW_TEXTURE_REGION_X 648
#define SPECTATING_WHO_ARROW_TEXTURE_REGION_Y 536
#define SPECTATING_WHO_ARROW_TEXTURE_REGION_WIDTH 16
#define SPECTATING_WHO_ARROW_TEXTURE_REGION_HEIGHT 16

/// Interface 2

#define WAITING_PLAYERS_LIST_BACKGROUND_TEXTURE_REGION_X 280
#define WAITING_PLAYERS_LIST_BACKGROUND_TEXTURE_REGION_Y 8
#define WAITING_PLAYERS_LIST_BACKGROUND_TEXTURE_REGION_WIDTH 232
#define WAITING_PLAYERS_LIST_BACKGROUND_TEXTURE_REGION_HEIGHT 264

#define INTERFACE_2_PLAYER_AVATAR_BLUE_TEXTURE_REGION_X 8
#define INTERFACE_2_PLAYER_AVATAR_BLUE_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_RED_TEXTURE_REGION_X 40
#define INTERFACE_2_PLAYER_AVATAR_RED_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_GREEN_TEXTURE_REGION_X 72
#define INTERFACE_2_PLAYER_AVATAR_GREEN_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_BLACK_TEXTURE_REGION_X 104
#define INTERFACE_2_PLAYER_AVATAR_BLACK_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_WHITE_TEXTURE_REGION_X 136
#define INTERFACE_2_PLAYER_AVATAR_WHITE_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_PINK_TEXTURE_REGION_X 168
#define INTERFACE_2_PLAYER_AVATAR_PINK_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_ORANGE_TEXTURE_REGION_X 200
#define INTERFACE_2_PLAYER_AVATAR_ORANGE_TEXTURE_REGION_Y 136
#define INTERFACE_2_PLAYER_AVATAR_YELLOW_TEXTURE_REGION_X 232
#define INTERFACE_2_PLAYER_AVATAR_YELLOW_TEXTURE_REGION_Y 136

#define INTERFACE_2_BOT_AVATAR_BLUE_TEXTURE_REGION_X 8
#define INTERFACE_2_BOT_AVATAR_BLUE_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_RED_TEXTURE_REGION_X 40
#define INTERFACE_2_BOT_AVATAR_RED_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_GREEN_TEXTURE_REGION_X 72
#define INTERFACE_2_BOT_AVATAR_GREEN_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_BLACK_TEXTURE_REGION_X 104
#define INTERFACE_2_BOT_AVATAR_BLACK_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_WHITE_TEXTURE_REGION_X 136
#define INTERFACE_2_BOT_AVATAR_WHITE_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_PINK_TEXTURE_REGION_X 168
#define INTERFACE_2_BOT_AVATAR_PINK_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_ORANGE_TEXTURE_REGION_X 200
#define INTERFACE_2_BOT_AVATAR_ORANGE_TEXTURE_REGION_Y 168
#define INTERFACE_2_BOT_AVATAR_YELLOW_TEXTURE_REGION_X 232
#define INTERFACE_2_BOT_AVATAR_YELLOW_TEXTURE_REGION_Y 168

#define INTERFACE_2_PLAYER_AVATAR_TEXTURE_REGION_WIDTH 32
#define INTERFACE_2_PLAYER_AVATAR_TEXTURE_REGION_HEIGHT 32

#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_WP8_TEXTURE_REGION_X 8
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_WP8_TEXTURE_REGION_Y 200
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_WP8_TEXTURE_REGION_WIDTH 32
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_WP8_TEXTURE_REGION_HEIGHT 32

#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_ANDROID_TEXTURE_REGION_X 40
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_ANDROID_TEXTURE_REGION_Y 200
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_ANDROID_TEXTURE_REGION_WIDTH 32
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_ANDROID_TEXTURE_REGION_HEIGHT 32

#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_IOS_TEXTURE_REGION_X 72
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_IOS_TEXTURE_REGION_Y 200
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_IOS_TEXTURE_REGION_WIDTH 32
#define INTERFACE_2_PLAYER_PLATFORM_AVATAR_IOS_TEXTURE_REGION_HEIGHT 32

#define COUNTDOWN_3_TEXTURE_REGION_X 576
#define COUNTDOWN_3_TEXTURE_REGION_Y 8
#define COUNTDOWN_3_TEXTURE_REGION_WIDTH 104
#define COUNTDOWN_3_TEXTURE_REGION_HEIGHT 80

#define COUNTDOWN_2_TEXTURE_REGION_X 704
#define COUNTDOWN_2_TEXTURE_REGION_Y 8
#define COUNTDOWN_2_TEXTURE_REGION_WIDTH 104
#define COUNTDOWN_2_TEXTURE_REGION_HEIGHT 80

#define COUNTDOWN_1_TEXTURE_REGION_X 832
#define COUNTDOWN_1_TEXTURE_REGION_Y 8
#define COUNTDOWN_1_TEXTURE_REGION_WIDTH 104
#define COUNTDOWN_1_TEXTURE_REGION_HEIGHT 80

#define BATTLE_TEXTURE_REGION_X 568
#define BATTLE_TEXTURE_REGION_Y 104
#define BATTLE_TEXTURE_REGION_WIDTH 448
#define BATTLE_TEXTURE_REGION_HEIGHT 56

#define HURRY_UP_TEXTURE_REGION_X 8
#define HURRY_UP_TEXTURE_REGION_Y 816
#define HURRY_UP_TEXTURE_REGION_WIDTH 688
#define HURRY_UP_TEXTURE_REGION_HEIGHT 56

#define DRAW_TEXTURE_REGION_X 8
#define DRAW_TEXTURE_REGION_Y 688
#define DRAW_TEXTURE_REGION_WIDTH 360
#define DRAW_TEXTURE_REGION_HEIGHT 56

#define GAME_SET_TEXTURE_REGION_X 8
#define GAME_SET_TEXTURE_REGION_Y 752
#define GAME_SET_TEXTURE_REGION_WIDTH 472
#define GAME_SET_TEXTURE_REGION_HEIGHT 48

#endif /* RESOURCE_CONSTANTS_H */
