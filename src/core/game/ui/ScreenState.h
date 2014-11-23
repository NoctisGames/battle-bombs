//
//  ScreenState.h
//  battlebombs
//
//  Created by Stephen Gowen on 11/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef battlebombs_ScreenState_h
#define battlebombs_ScreenState_h

/// Screen States are used to indicate to the outer layer (Java, Obj-C, C#)
/// that some interesting shit has happened on the C++ layer

#define SCREEN_STATE_NORMAL 0
#define SCREEN_STATE_ENTERED_SPECTATOR_MODE 1
#define SCREEN_STATE_CONNECTION_ERROR 2
#define SCREEN_STATE_EXIT 3
#define SCREEN_STATE_OFFLINE_MODE_NEXT_MAP 4

#endif /* battlebombs_ScreenState_h */