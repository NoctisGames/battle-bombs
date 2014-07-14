//
//  GameEvent.h
//  bomber-party
//
//  Created by Stephen Gowen on 3/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

// Event Type
#define BEGIN_SPECTATE 1336
#define BEGIN_GAME 1337
#define CLIENT_UPDATE 1338

// Map Type
#define MAP_SPACE 0
#define MAP_GRASSLANDS 1
#define MAP_MOUNTAINS 2
#define MAP_BASE 3

#define PLAYER_EVENT_BASE 1000000
#define PLAYER_EVENT_DIRECTION_BASE 100000
#define PLAYER_EVENT_GRID_X_BASE 1000
#define PLAYER_EVENT_GRID_Y_BASE 10

#define PLAYER_MOVE_RIGHT 1
#define PLAYER_MOVE_UP 2
#define PLAYER_MOVE_LEFT 3
#define PLAYER_MOVE_DOWN 4
#define PLAYER_MOVE_STOP 5
#define PLAYER_PLANT_BOMB 6
#define PLAYER_PUSH_BOMB 7
#define PLAYER_DEATH 9

#endif /* GAME_EVENT_H */
