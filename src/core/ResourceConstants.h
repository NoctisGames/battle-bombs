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
#define MUSIC_PLAY_MAP_SPACE 2
#define MUSIC_PLAY_MAP_GRASSLANDS 3
#define MUSIC_PLAY_MAP_MOUNTAINS 4
#define MUSIC_PLAY_MAP_BASE 5

// Sound Definitions
#define SOUND_PLANT_BOMB 1
#define SOUND_EXPLOSION 2
#define SOUND_DEATH 3

// Texture Definitions
#define GAME_TEXTURE_WIDTH 1024
#define GAME_TEXTURE_HEIGHT 1024

// Texture Region Definitions

// Game
#define WORLD_BACKGROUND_TEXTURE_REGION_X 0
#define WORLD_BACKGROUND_TEXTURE_REGION_Y 0
#define WORLD_BACKGROUND_TEXTURE_REGION_WIDTH 536
#define WORLD_BACKGROUND_TEXTURE_REGION_HEIGHT 608

#define MOUNTAINS_DOOR_TEXTURE_REGION_X 544
#define MOUNTAINS_DOOR_TEXTURE_REGION_Y 520
#define MOUNTAINS_DOOR_TEXTURE_REGION_WIDTH 96
#define MOUNTAINS_DOOR_TEXTURE_REGION_HEIGHT 64

#define BORDER_TOP_TEXTURE_REGION_X 0
#define BORDER_TOP_TEXTURE_REGION_Y 616
#define BORDER_TOP_TEXTURE_REGION_WIDTH 536
#define BORDER_TOP_TEXTURE_REGION_HEIGHT 33

#define BORDER_LEFT_TEXTURE_REGION_X 544
#define BORDER_LEFT_TEXTURE_REGION_Y 32
#define BORDER_LEFT_TEXTURE_REGION_WIDTH 33
#define BORDER_LEFT_TEXTURE_REGION_HEIGHT 448

#define BORDER_RIGHT_TEXTURE_REGION_X 583
#define BORDER_RIGHT_TEXTURE_REGION_Y 32
#define BORDER_RIGHT_TEXTURE_REGION_WIDTH 25
#define BORDER_RIGHT_TEXTURE_REGION_HEIGHT 448

#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_X 544
#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_Y 600
#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_WIDTH 128
#define BORDER_BOTTOM_LEFT_TEXTURE_REGION_HEIGHT 96

#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_X 680
#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_Y 600
#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_WIDTH 120
#define BORDER_BOTTOM_RIGHT_TEXTURE_REGION_HEIGHT 96

#define BORDER_BOTTOM_TEXTURE_REGION_X 0
#define BORDER_BOTTOM_TEXTURE_REGION_Y 656
#define BORDER_BOTTOM_TEXTURE_REGION_WIDTH 536
#define BORDER_BOTTOM_TEXTURE_REGION_HEIGHT 32

#define INSIDE_BLOCK_TEXTURE_REGION_X 400
#define INSIDE_BLOCK_TEXTURE_REGION_Y 928
#define INSIDE_BLOCK_TEXTURE_REGION_WIDTH 32
#define INSIDE_BLOCK_TEXTURE_REGION_HEIGHT 32

#define BREAKABLE_BLOCK_TEXTURE_REGION_X 8
#define BREAKABLE_BLOCK_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_X 104
#define BREAKABLE_BLOCK_FRAME_1_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_X 200
#define BREAKABLE_BLOCK_FRAME_2_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_X 296
#define BREAKABLE_BLOCK_FRAME_3_TEXTURE_REGION_Y 728
#define BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_X 8
#define BREAKABLE_BLOCK_FRAME_4_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_X 104
#define BREAKABLE_BLOCK_FRAME_5_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_X 200
#define BREAKABLE_BLOCK_FRAME_6_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_X 296
#define BREAKABLE_BLOCK_FRAME_7_TEXTURE_REGION_Y 824
#define BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_X 8
#define BREAKABLE_BLOCK_FRAME_8_TEXTURE_REGION_Y 920
#define BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_X 104
#define BREAKABLE_BLOCK_FRAME_9_TEXTURE_REGION_Y 920
#define BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_X 200
#define BREAKABLE_BLOCK_FRAME_10_TEXTURE_REGION_Y 920
#define BREAKABLE_BLOCK_TEXTURE_REGION_WIDTH 96
#define BREAKABLE_BLOCK_TEXTURE_REGION_HEIGHT 96

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

#define PLAYER_TEXTURE_REGION_WIDTH 64
#define PLAYER_TEXTURE_REGION_HEIGHT 64

#define PLAYER_IDLE_WINNING_TEXTURE_REGION_Y 64 * 0

#define PLAYER_IDLE_FRAME_1_TEXTURE_REGION_X 64 * 0
#define PLAYER_IDLE_FRAME_2_TEXTURE_REGION_X 64 * 1
#define PLAYER_IDLE_FRAME_3_TEXTURE_REGION_X 64 * 2
#define PLAYER_IDLE_FRAME_4_TEXTURE_REGION_X 64 * 3

#define PLAYER_WINNING_FRAME_1_TEXTURE_REGION_X 64 * 10
#define PLAYER_WINNING_FRAME_2_TEXTURE_REGION_X 64 * 11
#define PLAYER_WINNING_FRAME_3_TEXTURE_REGION_X 64 * 12
#define PLAYER_WINNING_FRAME_4_TEXTURE_REGION_X 64 * 13
#define PLAYER_WINNING_FRAME_5_TEXTURE_REGION_X 64 * 14
#define PLAYER_WINNING_FRAME_6_TEXTURE_REGION_X 64 * 15

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

#define PLAYER_DEATH_FRAME_1_TEXTURE_REGION_X 64 * 10
#define PLAYER_DEATH_FRAME_2_TEXTURE_REGION_X 64 * 11
#define PLAYER_DEATH_FRAME_3_TEXTURE_REGION_X 64 * 12
#define PLAYER_DEATH_FRAME_4_TEXTURE_REGION_X 64 * 13
#define PLAYER_DEATH_FRAME_5_TEXTURE_REGION_X 64 * 14
#define PLAYER_DEATH_FRAME_6_TEXTURE_REGION_X 64 * 15

#define PLAYER_PLACE_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_X 64 * 10
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_Y 64 * 8
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_X 64 * 11
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_Y 64 * 8
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_X 64 * 12
#define PLAYER_PLACE_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_Y 64 * 8

#define PLAYER_PLACE_BOMB_UP_FRAME_1_TEXTURE_REGION_X 64 * 10
#define PLAYER_PLACE_BOMB_UP_FRAME_1_TEXTURE_REGION_Y 64 * 7
#define PLAYER_PLACE_BOMB_UP_FRAME_2_TEXTURE_REGION_X 64 * 11
#define PLAYER_PLACE_BOMB_UP_FRAME_2_TEXTURE_REGION_Y 64 * 7
#define PLAYER_PLACE_BOMB_UP_FRAME_3_TEXTURE_REGION_X 64 * 12
#define PLAYER_PLACE_BOMB_UP_FRAME_3_TEXTURE_REGION_Y 64 * 7

#define PLAYER_PLACE_BOMB_LEFT_FRAME_1_TEXTURE_REGION_X 64 * 13
#define PLAYER_PLACE_BOMB_LEFT_FRAME_1_TEXTURE_REGION_Y 64 * 8
#define PLAYER_PLACE_BOMB_LEFT_FRAME_2_TEXTURE_REGION_X 64 * 14
#define PLAYER_PLACE_BOMB_LEFT_FRAME_2_TEXTURE_REGION_Y 64 * 8
#define PLAYER_PLACE_BOMB_LEFT_FRAME_3_TEXTURE_REGION_X 64 * 15
#define PLAYER_PLACE_BOMB_LEFT_FRAME_3_TEXTURE_REGION_Y 64 * 8

#define PLAYER_PLACE_BOMB_DOWN_FRAME_1_TEXTURE_REGION_X 64 * 13
#define PLAYER_PLACE_BOMB_DOWN_FRAME_1_TEXTURE_REGION_Y 64 * 7
#define PLAYER_PLACE_BOMB_DOWN_FRAME_2_TEXTURE_REGION_X 64 * 14
#define PLAYER_PLACE_BOMB_DOWN_FRAME_2_TEXTURE_REGION_Y 64 * 7
#define PLAYER_PLACE_BOMB_DOWN_FRAME_3_TEXTURE_REGION_X 64 * 15
#define PLAYER_PLACE_BOMB_DOWN_FRAME_3_TEXTURE_REGION_Y 64 * 7

#define PLAYER_PUSH_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_X 64 * 10
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_1_TEXTURE_REGION_Y 64 * 6
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_X 64 * 11
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_2_TEXTURE_REGION_Y 64 * 6
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_X 64 * 12
#define PLAYER_PUSH_BOMB_RIGHT_FRAME_3_TEXTURE_REGION_Y 64 * 6

#define PLAYER_PUSH_BOMB_UP_FRAME_1_TEXTURE_REGION_X 64 * 10
#define PLAYER_PUSH_BOMB_UP_FRAME_1_TEXTURE_REGION_Y 64 * 5
#define PLAYER_PUSH_BOMB_UP_FRAME_2_TEXTURE_REGION_X 64 * 11
#define PLAYER_PUSH_BOMB_UP_FRAME_2_TEXTURE_REGION_Y 64 * 5
#define PLAYER_PUSH_BOMB_UP_FRAME_3_TEXTURE_REGION_X 64 * 12
#define PLAYER_PUSH_BOMB_UP_FRAME_3_TEXTURE_REGION_Y 64 * 5

#define PLAYER_PUSH_BOMB_LEFT_FRAME_1_TEXTURE_REGION_X 64 * 13
#define PLAYER_PUSH_BOMB_LEFT_FRAME_1_TEXTURE_REGION_Y 64 * 6
#define PLAYER_PUSH_BOMB_LEFT_FRAME_2_TEXTURE_REGION_X 64 * 14
#define PLAYER_PUSH_BOMB_LEFT_FRAME_2_TEXTURE_REGION_Y 64 * 6
#define PLAYER_PUSH_BOMB_LEFT_FRAME_3_TEXTURE_REGION_X 64 * 15
#define PLAYER_PUSH_BOMB_LEFT_FRAME_3_TEXTURE_REGION_Y 64 * 6

#define PLAYER_PUSH_BOMB_DOWN_FRAME_1_TEXTURE_REGION_X 64 * 13
#define PLAYER_PUSH_BOMB_DOWN_FRAME_1_TEXTURE_REGION_Y 64 * 5
#define PLAYER_PUSH_BOMB_DOWN_FRAME_2_TEXTURE_REGION_X 64 * 14
#define PLAYER_PUSH_BOMB_DOWN_FRAME_2_TEXTURE_REGION_Y 64 * 5
#define PLAYER_PUSH_BOMB_DOWN_FRAME_3_TEXTURE_REGION_X 64 * 15
#define PLAYER_PUSH_BOMB_DOWN_FRAME_3_TEXTURE_REGION_Y 64 * 5

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

// Explosion Fire Stuff

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

// Interface

#define FONT_TEXTURE_REGION_X 8
#define FONT_TEXTURE_REGION_Y 248
#define FONT_GLYPHS_PER_ROW 16
#define FONT_GLYPH_WIDTH 38
#define FONT_GLYPH_HEIGHT 26

#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_X 480
#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_Y 168
#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_WIDTH 536
#define INTERFACE_OVERLAY_SPECTATOR_TEXTURE_REGION_HEIGHT 42

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

#define BUTTON_PUSH_DISABLED_TEXTURE_REGION_X 448
#define BUTTON_PUSH_DISABLED_TEXTURE_REGION_Y 72
#define BUTTON_PUSH_ENABLED_TEXTURE_REGION_X 512
#define BUTTON_PUSH_ENABLED_TEXTURE_REGION_Y 72
#define BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION_X 576
#define BUTTON_PUSH_HIGHLIGHTED_TEXTURE_REGION_Y 72
#define BUTTON_PUSH_PRESSED_TEXTURE_REGION_X 640
#define BUTTON_PUSH_PRESSED_TEXTURE_REGION_Y 72

#define BUTTON_BOMB_DISABLED_TEXTURE_REGION_X 448
#define BUTTON_BOMB_DISABLED_TEXTURE_REGION_Y 8
#define BUTTON_BOMB_ENABLED_TEXTURE_REGION_X 512
#define BUTTON_BOMB_ENABLED_TEXTURE_REGION_Y 8
#define BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION_X 576
#define BUTTON_BOMB_HIGHLIGHTED_TEXTURE_REGION_Y 8
#define BUTTON_BOMB_PRESSED_TEXTURE_REGION_X 640
#define BUTTON_BOMB_PRESSED_TEXTURE_REGION_Y 8

#define BUTTON_TEXTURE_REGION_WIDTH 64
#define BUTTON_TEXTURE_REGION_HEIGHT 64

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

#endif /* RESOURCE_CONSTANTS_H */
