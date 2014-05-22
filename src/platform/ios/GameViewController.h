//
//  GameViewController.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import <GLKit/GLKit.h>
#import <AppWarp_iOS_SDK/AppWarp_iOS_SDK.h>

@interface GameViewController : GLKViewController <UITextFieldDelegate, ChatRequestListener, ConnectionRequestListener, LobbyRequestListener, NotifyListener, RoomRequestListener, ZoneRequestListener>
{
    // Nothing to do here
}

@property (strong, nonatomic) NSString *username;
@property (strong, nonatomic) NSString *joinedRoomId;

@end