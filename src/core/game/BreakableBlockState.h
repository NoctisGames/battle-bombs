//
//  PlayerState.h
//  battlebombs
//
//  Created by Stephen Gowen on 4/7/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef BREAKABLE_BLOCK_STATE_H
#define BREAKABLE_BLOCK_STATE_H

typedef enum
{
    BB_NORMAL,
    BB_FREEZING,
    BB_FROZEN,
    EXPLODING,
    DESTROYED
} Breakable_Block_State;

#endif /* BREAKABLE_BLOCK_STATE_H */