//
//  ResourceConstants.h
//  bomber-party
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef RESOURCE_CONSTANTS_H
#define RESOURCE_CONSTANTS_H

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

// Music Definitions
#define MUSIC_STOP 1
#define MUSIC_PLAY 2

// Sound Definitions
#define SOUND_PLANT_BOMB 1
#define SOUND_EXPLOSION 2
#define SOUND_DEATH 3

// Texture Definitions
#define GAME_TEXTURE_WIDTH 1024
#define GAME_TEXTURE_HEIGHT 1024

// Texture Region Definitions
#define WORLD_BACKGROUND_TEXTURE_REGION_X 0
#define WORLD_BACKGROUND_TEXTURE_REGION_Y 0
#define WORLD_BACKGROUND_TEXTURE_REGION_WIDTH 536
#define WORLD_BACKGROUND_TEXTURE_REGION_HEIGHT 608

#define INSIDE_BLOCK_TEXTURE_REGION_X 8
#define INSIDE_BLOCK_TEXTURE_REGION_Y 800
#define INSIDE_BLOCK_TEXTURE_REGION_WIDTH 32
#define INSIDE_BLOCK_TEXTURE_REGION_HEIGHT 32

#define BREAKABLE_BLOCK_TEXTURE_REGION_X 0
#define BREAKABLE_BLOCK_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_X 96
#define BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_X 192
#define BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_X 288
#define BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_X 384
#define BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_X 480
#define BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_X 576
#define BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_X 672
#define BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_X 768
#define BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_X 864
#define BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_Y 696
#define BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_X 864
#define BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_Y 792
#define BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH 96
#define BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT 96

#define POWER_UP_BOMB_TEXTURE_REGION_Y 824
#define POWER_UP_FIRE_TEXTURE_REGION_Y 864
#define POWER_UP_FORCE_FIELD_TEXTURE_REGION_Y 904
#define POWER_UP_SPEED_TEXTURE_REGION_Y 944
#define POWER_UP_PUSH_TEXTURE_REGION_Y 984
#define POWER_UP_FRAME_1_TEXTURE_REGION_X 112
#define POWER_UP_FRAME_2_TEXTURE_REGION_X 144
#define POWER_UP_FRAME_3_TEXTURE_REGION_X 176
#define POWER_UP_FRAME_4_TEXTURE_REGION_X 208
#define POWER_UP_FRAME_5_TEXTURE_REGION_X 240
#define POWER_UP_FRAME_6_TEXTURE_REGION_X 208
#define POWER_UP_FRAME_7_TEXTURE_REGION_X 176
#define POWER_UP_FRAME_8_TEXTURE_REGION_X 144
#define POWER_UP_TEXTURE_REGION_WIDTH 32
#define POWER_UP_TEXTURE_REGION_HEIGHT 32

#define INTERFACE_OVERLAY_LEFT_BAR_TEXTURE_REGION_X 512
#define INTERFACE_OVERLAY_LEFT_BAR_TEXTURE_REGION_Y 808
#define INTERFACE_OVERLAY_LEFT_BAR_TEXTURE_REGION_WIDTH 48
#define INTERFACE_OVERLAY_LEFT_BAR_TEXTURE_REGION_HEIGHT 214

#define DIRECTIONAL_CONTROL_TEXTURE_REGION_X 568
#define DIRECTIONAL_CONTROL_TEXTURE_REGION_Y 928
#define DIRECTIONAL_CONTROL_TEXTURE_REGION_WIDTH 135
#define DIRECTIONAL_CONTROL_TEXTURE_REGION_HEIGHT 93

#define INTERFACE_OVERLAY_BOTTOM_BAR_TEXTURE_REGION_X 584
#define INTERFACE_OVERLAY_BOTTOM_BAR_TEXTURE_REGION_Y 864
#define INTERFACE_OVERLAY_BOTTOM_BAR_TEXTURE_REGION_WIDTH 192
#define INTERFACE_OVERLAY_BOTTOM_BAR_TEXTURE_REGION_HEIGHT 48

#define INTERFACE_OVERLAY_TEXTURE_REGION_X 744
#define INTERFACE_OVERLAY_TEXTURE_REGION_Y 952
#define INTERFACE_OVERLAY_TEXTURE_REGION_WIDTH 136
#define INTERFACE_OVERLAY_TEXTURE_REGION_HEIGHT 72

#define PLAYER_TEXTURE_REGION_WIDTH 64
#define PLAYER_TEXTURE_REGION_HEIGHT 64

#define PLAYER_WALK_RIGHT_TEXTURE_REGION_Y 64 * 1
#define PLAYER_WALK_UP_TEXTURE_REGION_Y 64 * 2
#define PLAYER_WALK_LEFT_TEXTURE_REGION_Y 64 * 3
#define PLAYER_WALK_DOWN_TEXTURE_REGION_Y 64 * 4

#define PLAYER_WALK_IDLE_TEXTURE_REGION_X 64 * 0
#define PLAYER_WALK_FRAME_1_TEXTURE_REGION_X 64 * 1
#define PLAYER_WALK_FRAME_2_TEXTURE_REGION_X 64 * 2
#define PLAYER_WALK_FRAME_3_TEXTURE_REGION_X 64 * 3
#define PLAYER_WALK_FRAME_4_TEXTURE_REGION_X 64 * 4
#define PLAYER_WALK_FRAME_5_TEXTURE_REGION_X 64 * 5
#define PLAYER_WALK_FRAME_6_TEXTURE_REGION_X 64 * 6
#define PLAYER_WALK_FRAME_7_TEXTURE_REGION_X 64 * 7
#define PLAYER_WALK_FRAME_8_TEXTURE_REGION_X 64 * 8

#define PLAYER_DEATH_RIGHT_TEXTURE_REGION_Y 64 * 1
#define PLAYER_DEATH_UP_TEXTURE_REGION_Y 64 * 2
#define PLAYER_DEATH_LEFT_TEXTURE_REGION_Y 64 * 3
#define PLAYER_DEATH_DOWN_TEXTURE_REGION_Y 64 * 4

#define PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X 640
#define PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X 704
#define PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X 768
#define PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X 832
#define PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X 896
#define PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X 960

#define BOMB_FRAME_TEXTURE_REGION_X 616
#define BOMB_FRAME_1_TEXTURE_REGION_Y 8
#define BOMB_FRAME_2_TEXTURE_REGION_Y 72
#define BOMB_FRAME_3_TEXTURE_REGION_Y 136
#define BOMB_FRAME_4_TEXTURE_REGION_Y 200
#define BOMB_FRAME_5_TEXTURE_REGION_Y 264
#define BOMB_FRAME_6_TEXTURE_REGION_Y 328
#define BOMB_FRAME_7_TEXTURE_REGION_Y 392
#define BOMB_EXPLOSION_FRAME_1_TEXTURE_REGION_Y 456
#define BOMB_EXPLOSION_FRAME_2_TEXTURE_REGION_Y 520
#define BOMB_TEXTURE_REGION_WIDTH 64
#define BOMB_TEXTURE_REGION_HEIGHT 64

// Explosion Fire Stuff

#define CORE_PART_1_TEXTURE_REGION_X 688
#define CORE_PART_1_TEXTURE_REGION_Y 8
#define CORE_PART_2_TEXTURE_REGION_X 728
#define CORE_PART_2_TEXTURE_REGION_Y 8
#define CORE_PART_3_TEXTURE_REGION_X 768
#define CORE_PART_3_TEXTURE_REGION_Y 8

#define CORE_PART_4_POW_1_TEXTURE_REGION_X 808
#define CORE_PART_4_POW_1_TEXTURE_REGION_Y 8
#define CORE_PART_4_POW_2_TEXTURE_REGION_X 688
#define CORE_PART_4_POW_2_TEXTURE_REGION_Y 176
#define CORE_PART_5_POW_2_TEXTURE_REGION_X 728
#define CORE_PART_5_POW_2_TEXTURE_REGION_Y 176
#define CORE_PART_6_POW_2_TEXTURE_REGION_X 768
#define CORE_PART_6_POW_2_TEXTURE_REGION_Y 176

#define CORE_END_TEXTURE_REGION_X 848
#define CORE_END_TEXTURE_REGION_Y 8

#define HEAD_PART_1_TEXTURE_REGION_X 688
#define HEAD_PART_1_TEXTURE_REGION_Y 48
#define HEAD_PART_2_TEXTURE_REGION_X 728
#define HEAD_PART_2_TEXTURE_REGION_Y 48
#define HEAD_PART_3_TEXTURE_REGION_X 768
#define HEAD_PART_3_TEXTURE_REGION_Y 48

#define HEAD_PART_4_POW_1_TEXTURE_REGION_X 808
#define HEAD_PART_4_POW_1_TEXTURE_REGION_Y 48
#define HEAD_PART_4_POW_2_TEXTURE_REGION_X 688
#define HEAD_PART_4_POW_2_TEXTURE_REGION_Y 216
#define HEAD_PART_5_POW_2_TEXTURE_REGION_X 728
#define HEAD_PART_5_POW_2_TEXTURE_REGION_Y 216
#define HEAD_PART_6_POW_2_TEXTURE_REGION_X 768
#define HEAD_PART_6_POW_2_TEXTURE_REGION_Y 216

#define EDGE_FROM_CORE_PART_1_TEXTURE_REGION_X 688
#define EDGE_FROM_CORE_PART_1_TEXTURE_REGION_Y 88
#define EDGE_FROM_CORE_PART_2_TEXTURE_REGION_X 728
#define EDGE_FROM_CORE_PART_2_TEXTURE_REGION_Y 88
#define EDGE_FROM_CORE_PART_3_TEXTURE_REGION_X 768
#define EDGE_FROM_CORE_PART_3_TEXTURE_REGION_Y 88

#define EDGE_FROM_CORE_PART_4_POW_1_TEXTURE_REGION_X 808
#define EDGE_FROM_CORE_PART_4_POW_1_TEXTURE_REGION_Y 88
#define EDGE_FROM_CORE_PART_4_POW_2_TEXTURE_REGION_X 688
#define EDGE_FROM_CORE_PART_4_POW_2_TEXTURE_REGION_Y 336
#define EDGE_FROM_CORE_PART_5_POW_2_TEXTURE_REGION_X 728
#define EDGE_FROM_CORE_PART_5_POW_2_TEXTURE_REGION_Y 336

#define EDGE_FROM_FAT_NECK_TEXTURE_REGION_X 688
#define EDGE_FROM_FAT_NECK_TEXTURE_REGION_Y 376
#define EDGE_FROM_THIN_NECK_TEXTURE_REGION_X 728
#define EDGE_FROM_THIN_NECK_TEXTURE_REGION_Y 376

#define FAT_NECK_TEXTURE_REGION_X 688
#define FAT_NECK_TEXTURE_REGION_Y 256
#define THIN_NECK_TEXTURE_REGION_X 728
#define THIN_NECK_TEXTURE_REGION_Y 256

#define BODY_TEXTURE_REGION_X 688
#define BODY_TEXTURE_REGION_Y 296

#define NECK_AND_BODY_END_TEXTURE_REGION_X 768
#define NECK_AND_BODY_END_TEXTURE_REGION_Y 256

#define FIRE_TEXTURE_REGION_WIDTH 32
#define FIRE_TEXTURE_REGION_HEIGHT 32

#endif /* RESOURCE_CONSTANTS_H */
