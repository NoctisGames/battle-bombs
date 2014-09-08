//
//  GameState.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum
{
    WAITING, // Temporary state because on the first play of offline mode (it doesn't make sense to make the user tap the screen)
    WAITING_FOR_CONNECTION, // Only set when the GameScreen is first created
    CONNECTION_ERROR_WAITING_FOR_INPUT, // Only set when the GameScreen is first created
    WAITING_FOR_SERVER,
    WAITING_FOR_LOCAL_SETTINGS,
    COUNTING_DOWN,
    SPECTATING,
    RUNNING,
    GAME_ENDING
} Game_State;

#endif /* GAME_STATE_H */