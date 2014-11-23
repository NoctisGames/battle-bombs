//
//  PlayerState.h
//  battlebombs
//
//  Created by Stephen Gowen on 6/26/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef PLAYER_ACTION_STATE_H
#define PLAYER_ACTION_STATE_H

typedef enum
{
    IDLE,
    WINNING,
    PLACING_BOMB,
    PUSHING_BOMB,
    RAISING_SHIELD,
    SHIELD_RAISED
} Player_Action_State;

#endif /* PLAYER_ACTION_STATE_H */