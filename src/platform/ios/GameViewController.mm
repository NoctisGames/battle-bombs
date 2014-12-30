//
//  GameViewController.mm
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#define APPWARP_APP_KEY         @"e4207af0-de78-496d-8"
#define APPWARP_SECRET_KEY      @"139f6094-4e04-4041-9"
#define APPWARP_HOST_ADDRESS    @"191.234.54.124"
#define APPWARP_AUTH_DATA       @"GowenGameDev_1.3.0"

#import "GameViewController.h"
#import "MainViewController.h"

// C++
#include "game.h"
#include "GameEvent.h"
#include "ScreenState.h"

@interface GameViewController ()
{
    // Empty
}

@property (strong, nonatomic) NSString *joinedRoomId;

@end

@implementation GameViewController

static NSString * const KEEP_ALIVE = [@"KEEP_ALIVE_" stringByAppendingFormat:@"%i", PLATFORM_IOS];
static NSString * const EVENT_TYPE = @"eventType";
static NSString * const EVENTS = @"events";
static NSString * const PLAYER_INDEX = @"playerIndex";
static NSString * const X = @"X";
static NSString * const Y = @"Y";
static NSString * const DIRECTION = @"Direction";

// PRE_GAME_SERVER_UPDATE
static NSString * const PHASE = @"phase";

+ (void)initialize
{
    [WarpClient initWarp:APPWARP_APP_KEY server:APPWARP_HOST_ADDRESS];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    WarpClient *warpClient = [WarpClient getInstance];
    
    [warpClient addConnectionRequestListener:self];
    [warpClient addNotificationListener:self];
    
    NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, CONNECTING];
    
    on_chat_received([preGameUpdate UTF8String]);
    
    [warpClient connectWithUserName:self.username authData:APPWARP_AUTH_DATA];
}

#pragma mark <ConnectionRequestListener>

- (void)onConnectDone:(ConnectEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
    NSLog(@"event.result: %hhu", event.result);
    
    if(event.result == 0)
    {
        NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, FINDING_ROOM_TO_JOIN];
        
        on_chat_received([preGameUpdate UTF8String]);
        
        [[WarpClient getInstance] joinRoomInRangeBetweenMinUsers:0 andMaxUsers:7 maxPrefered:YES];
    }
    else
    {
        NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, CONNECTION_ERROR];
        
        on_chat_received([preGameUpdate UTF8String]);
    }
}

- (void)onDisconnectDone:(ConnectEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onInitUDPDone:(Byte)result
{
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <NotifyListener>

- (void)onRoomCreated:(RoomData *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onRoomDestroyed:(RoomData *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserLeftRoom:(RoomData *)roomData username:(NSString *)username
{
    NSLog(@"%s", __FUNCTION__);
    
    if(roomData)
    {
        if([username isEqualToString:self.username])
        {
            [self onPause];
        }
    }
}

- (void)onUserJoinedRoom:(RoomData *)roomData username:(NSString *)username
{
    NSLog(@"%s", __FUNCTION__);
    
    if(roomData)
    {
        if([username isEqualToString:self.username])
        {
            self.joinedRoomId = roomData.roomId;
            
            NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, ROOM_JOINED_WAITING_FOR_SERVER];
            
            on_chat_received([preGameUpdate UTF8String]);
        }
    }
}

- (void)onUserLeftLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserJoinedLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onChatReceived:(ChatEvent *)chatEvent
{
    NSLog(@"%s", __FUNCTION__);
    
    NSLog(@"%@", chatEvent.message);
    
    on_chat_received([chatEvent.message UTF8String]);
}

- (void)onPrivateChatReceived:(NSString *)message fromUser:(NSString *)senderName
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUpdatePeersReceived:(UpdateEvent *)updateEvent
{
    NSLog(@"%s", __FUNCTION__);
}

-(void)onUserChangeRoomProperty:(RoomData *)event username:(NSString *)username properties:(NSDictionary *)properties lockedProperties:(NSDictionary *)lockedProperties
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onMoveCompleted:(MoveEvent *) moveEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserPaused:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUserResumed:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGameStarted:(NSString *)sender roomId:(NSString *)roomId  nextTurn:(NSString *)nextTurn
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGameStopped:(NSString *)sender roomId:(NSString *)roomId
{
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <Protected>

- (void)handleScreenState:(int)screenState
{
    if(screenState == SCREEN_STATE_EXIT)
    {
        [self dismissViewControllerAnimated:YES completion:nil];
    }
    else if(screenState == SCREEN_STATE_CONNECTION_ERROR)
    {
        MainViewController *mainViewController = (MainViewController *)self.presentingViewController;
        [self dismissViewControllerAnimated:YES completion:^
        {
            [mainViewController showConnectionErrorMessage];
        }];
    }
}

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
    
    [warpClient removeConnectionRequestListener:self];
    [warpClient removeNotificationListener:self];
    
    [warpClient leaveRoom:self.joinedRoomId];
    [warpClient disconnect];
    
    [super onPause];
}

@end