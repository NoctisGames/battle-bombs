//
//  GameViewController.mm
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define APPWARP_APP_KEY         @"1bc691b6-8c0f-4247-9"
#define APPWARP_SECRET_KEY      @"139f6094-4e04-4041-9"
#define APPWARP_HOST_ADDRESS    @"191.234.54.124"

// Response Codes
#define CLIENT_NEEDS_TO_UPDATE 1

#import "GameViewController.h"
#import "Logger.h"

// C++
#include "game.h"
#include "GameEvent.h"

@interface GameViewController ()
{
    // Empty
}

@property (strong, nonatomic) NSString *joinedRoomId;

@end

@implementation GameViewController

static NSString * const KEEP_ALIVE = [@"KEEP_ALIVE" stringByAppendingFormat:@"%i", PLATFORM_IOS];
static NSString * const EVENT_TYPE = @"eventType";
static NSString * const EVENTS = @"events";
static NSString * const PLAYER_INDEX = @"playerIndex";
static NSString * const X = @"X";
static NSString * const Y = @"Y";
static NSString * const DIRECTION = @"Direction";

// PRE_GAME_SERVER_UPDATE
static NSString * const PHASE = @"phase";

static Logger *logger = nil;

+ (void)initialize
{
    logger = [[Logger alloc] initWithClass:[GameViewController class]];
    
    [WarpClient initWarp:APPWARP_APP_KEY server:APPWARP_HOST_ADDRESS];
}

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
    
    NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, CONNECTING];
    
    on_chat_received([preGameUpdate UTF8String]);
    
    [warpClient connectWithUserName:self.username authData:@"T3chn3G4m35"];
}

#pragma mark <ChatRequestListener>

- (void)onSendChatDone:(Byte)result
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSendPrivateChatDone:(Byte)result
{
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <ConnectionRequestListener>

- (void)onConnectDone:(ConnectEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
    NSLog(@"event.result: %hhu", event.result);
    
    if(event.result == CLIENT_NEEDS_TO_UPDATE)
    {
        NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, UPDATE_REQUIRED];
        
        on_chat_received([preGameUpdate UTF8String]);
    }
    else
    {
        NSString *preGameUpdate = [NSString stringWithFormat:@"{\"%@\":%i,\"%@\":%i}", EVENT_TYPE, PRE_GAME, PHASE, FINDING_ROOM_TO_JOIN];
        
        on_chat_received([preGameUpdate UTF8String]);
        
        [[WarpClient getInstance] joinRoomInRangeBetweenMinUsers:0 andMaxUsers:7 maxPrefered:YES];
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

#pragma mark <LobbyRequestListener>

- (void)onJoinLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onLeaveLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSubscribeLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUnSubscribeLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetLiveLobbyInfoDone:(LiveRoomInfoEvent *)event
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

#pragma mark <RoomRequestListener>

- (void)onSubscribeRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUnSubscribeRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onJoinRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onLeaveRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetLiveRoomInfoDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSetCustomRoomDataDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUpdatePropertyDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onLockPropertiesDone:(Byte)result
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onUnlockPropertiesDone:(Byte)result
{
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <ZoneRequestListener>

- (void)onCreateRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onDeleteRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetAllRoomsDone:(AllRoomsEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetOnlineUsersDone:(AllUsersEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetLiveUserInfoDone:(LiveUserInfoEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onSetCustomUserDataDone:(LiveUserInfoEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

- (void)onGetMatchedRoomsDone:(MatchedRoomsEvent *)event
{
    NSLog(@"%s", __FUNCTION__);
}

#pragma mark <Protected>

- (void)handleGameState:(int)gameState
{
    if(gameState == 1)
    {
        [self dismissViewControllerAnimated:YES completion:nil];
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