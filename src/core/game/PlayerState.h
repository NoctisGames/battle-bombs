//
//  PlayerState.h
//  battlebombs
//
//  Created by Stephen Gowen on 4/7/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

typedef enum
{
    ALIVE,
    DYING,
    DEAD,
    ABOUT_TO_FALL, // for Space map
    FALLING, // for Space map
    FREEZING, // for Mountains map
    FROZEN // for Mountains map
} Player_State;

#endif /* PLAYER_STATE_H */