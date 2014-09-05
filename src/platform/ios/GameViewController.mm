//
//  GameViewController.mm
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "GameViewController.h"

// C++
#include "game.h"

@interface GameViewController ()
{
    // Empty
}

@end

@implementation GameViewController

static NSString * const KEEP_ALIVE = @"KEEP_ALIVE";
static NSString * const EVENT_TYPE = @"eventType";
static NSString * const EVENTS = @"events";
static NSString * const PLAYER_INDEX = @"playerIndex";
static NSString * const X = @"X";
static NSString * const Y = @"Y";
static NSString * const DIRECTION = @"Direction";

static int const CLIENT_UPDATE = 1338;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    WarpClient *warpClient = [WarpClient getInstance];
    
    [warpClient addChatRequestListener:self];
    [warpClient addConnectionRequestListener:self];
    [warpClient addLobbyRequestListener:self];
    [warpClient addNotificationListener:self];
    [warpClient addRoomRequestListener:self];
    [warpClient addZoneRequestListener:self];
}

#pragma mark <ChatRequestListener>

- (void)onSendChatDone:(Byte)result
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSendPrivateChatDone:(Byte)result
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <ConnectionRequestListener>

- (void)onConnectDone:(ConnectEvent*) event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onDisconnectDone:(ConnectEvent*) event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onInitUDPDone:(Byte)result
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <LobbyRequestListener>

- (void)onJoinLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onLeaveLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSubscribeLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUnSubscribeLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetLiveLobbyInfoDone:(LiveRoomInfoEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <NotifyListener>

- (void)onRoomCreated:(RoomData *)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onRoomDestroyed:(RoomData *)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserLeftRoom:(RoomData *)roomData username:(NSString *)username
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
    
    if([username isEqualToString:self.username])
    {
        [self onPause];
    }
}

- (void)onUserJoinedRoom:(RoomData *)roomData username:(NSString *)username
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserLeftLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserJoinedLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onChatReceived:(ChatEvent *)chatEvent
{
    NSLog(@"%s", __FUNCTION__);
    
    NSLog(@"%@", chatEvent.message);
    
    on_chat_received([chatEvent.message UTF8String]);
}

-(void)onPrivateChatReceived:(NSString *)message fromUser:(NSString *)senderName
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUpdatePeersReceived:(UpdateEvent *)updateEvent
{
    NSLog(@"%s", __FUNCTION__);
}

-(void)onUserChangeRoomProperty:(RoomData *)event username:(NSString *)username properties:(NSDictionary *)properties lockedProperties:(NSDictionary *)lockedProperties
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onMoveCompleted:(MoveEvent *) moveEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserPaused:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserResumed:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGameStarted:(NSString *)sender roomId:(NSString *)roomId  nextTurn:(NSString *)nextTurn
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGameStopped:(NSString *)sender roomId:(NSString *)roomId
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <RoomRequestListener>

- (void)onSubscribeRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUnSubscribeRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onJoinRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onLeaveRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetLiveRoomInfoDone:(LiveRoomInfoEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSetCustomRoomDataDone:(LiveRoomInfoEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUpdatePropertyDone:(LiveRoomInfoEvent *)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onLockPropertiesDone:(Byte)result
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUnlockPropertiesDone:(Byte)result
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <ZoneRequestListener>

- (void)onCreateRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onDeleteRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetAllRoomsDone:(AllRoomsEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetOnlineUsersDone:(AllUsersEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetLiveUserInfoDone:(LiveUserInfoEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSetCustomUserDataDone:(LiveUserInfoEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetMatchedRoomsDone:(MatchedRoomsEvent*)event
{
    // TODO
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <Protected>

- (void)pushEvents
{
    int eventId = get_oldest_event_id();
    if(eventId > 0)
    {
        NSString *eventsMessage = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":\"%i,", EVENT_TYPE, CLIENT_UPDATE, EVENTS, eventId];
        while ((eventId = get_oldest_event_id()) > 0)
        {
            eventsMessage = [eventsMessage stringByAppendingFormat:@"%i,", eventId];
        }
        eventsMessage = [eventsMessage stringByAppendingFormat:@"%i\"", 0]; // Terminate with 0
        
        eventsMessage = [eventsMessage stringByAppendingFormat:@",\"%@%i%@\":%f", PLAYER_INDEX, get_player_index(), X, get_player_x()];
        eventsMessage = [eventsMessage stringByAppendingFormat:@",\"%@%i%@\":%f", PLAYER_INDEX, get_player_index(), Y, get_player_y()];
        eventsMessage = [eventsMessage stringByAppendingFormat:@",\"%@%i%@\":%i}", PLAYER_INDEX, get_player_index(), DIRECTION, get_player_direction()];
        
        reset_time_since_last_client_event();
        
        WarpClient *warpClient = [WarpClient getInstance];
        [warpClient sendChat:eventsMessage];
    }
    else if(is_time_to_send_keep_alive())
    {
        reset_time_since_last_client_event();
        
        WarpClient *warpClient = [WarpClient getInstance];
        [warpClient sendChat:KEEP_ALIVE];
    }
}

- (void)onPause
{
    WarpClient *warpClient = [WarpClient getInstance];
    
    [warpClient removeChatRequestListener:self];
    [warpClient removeConnectionRequestListener:self];
    [warpClient removeLobbyRequestListener:self];
    [warpClient removeNotificationListener:self];
    [warpClient removeRoomRequestListener:self];
    [warpClient removeZoneRequestListener:self];
    
    [warpClient leaveRoom:self.joinedRoomId];
    [warpClient disconnect];
    
    [super onPause];
}

@end