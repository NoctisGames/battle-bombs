//
//  MainViewController.m
//  battlebombs
//
//  Created by Stephen Gowen on 2/26/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "MainViewController.h"
#import "GameViewController.h"
#import "Logger.h"

@interface MainViewController ()

@property (strong, nonatomic) NSString *joinedRoomId;

@end

@implementation MainViewController

static Logger *logger = nil;
static NSString *username;

+ (void)initialize
{
    logger = [[Logger alloc] initWithClass:[MainViewController class]];
    
    NSString *UUID = [[NSUUID UUID] UUIDString];
    UUID = [UUID substringToIndex:8];
    username = [@"Player_" stringByAppendingString:UUID];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [self initializeAppWarp];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([[segue identifier] isEqualToString:@"Main_To_Game"])
    {
        GameViewController *gameViewController = (GameViewController *) segue.destinationViewController;
        gameViewController.username = username;
        gameViewController.joinedRoomId = self.joinedRoomId;
        
        WarpClient *warpClient = [WarpClient getInstance];
        
        [warpClient removeChatRequestListener:self];
        [warpClient removeConnectionRequestListener:self];
        [warpClient removeLobbyRequestListener:self];
        [warpClient removeNotificationListener:self];
        [warpClient removeRoomRequestListener:self];
        [warpClient removeZoneRequestListener:self];
    }
    
    [super prepareForSegue:segue sender:sender];
}

- (IBAction)startQuickMatch:(id)sender
{
    [[WarpClient getInstance] connectWithUserName:username authData:@"iOS"];
}

- (IBAction)startQuickOfflineMatch:(id)sender
{
    [self performSegueWithIdentifier:@"Main_To_Offline_Game" sender:self];
}

#pragma <ChatRequestListener> methods

- (void)onSendChatDone:(Byte)result
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSendPrivateChatDone:(Byte)result
{
    NSLog(@"%s",__FUNCTION__);
}

#pragma <ConnectionRequestListener> methods

- (void)onConnectDone:(ConnectEvent *) event
{
    NSLog(@"%s",__FUNCTION__);
    
    [[WarpClient getInstance] joinRoomInRangeBetweenMinUsers:0 andMaxUsers:7 maxPrefered:YES];
}

- (void)onDisconnectDone:(ConnectEvent *) event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onInitUDPDone:(Byte)result
{
    NSLog(@"%s",__FUNCTION__);
}

#pragma <LobbyRequestListener> methods

- (void)onJoinLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onLeaveLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSubscribeLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUnSubscribeLobbyDone:(LobbyEvent *)lobbyEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetLiveLobbyInfoDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

#pragma <NotifyListener> methods

- (void)onRoomCreated:(RoomData *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onRoomDestroyed:(RoomData *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserLeftRoom:(RoomData *)roomData username:(NSString *)username
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserJoinedRoom:(RoomData *)roomData username:(NSString *)username
{
    NSLog(@"%s",__FUNCTION__);
    
    if(roomData)
    {
        self.joinedRoomId = roomData.roomId;
        
        [self performSegueWithIdentifier:@"Main_To_Game" sender:self];
    }
}

- (void)onUserLeftLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserJoinedLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onChatReceived:(ChatEvent *)chatEvent
{
    NSLog(@"%s",__FUNCTION__);
}

-(void)onPrivateChatReceived:(NSString *)message fromUser:(NSString *)senderName
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUpdatePeersReceived:(UpdateEvent *)updateEvent
{
    NSLog(@"%s",__FUNCTION__);
}

-(void)onUserChangeRoomProperty:(RoomData *)event username:(NSString *)username properties:(NSDictionary *)properties lockedProperties:(NSDictionary *)lockedProperties
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onMoveCompleted:(MoveEvent *) moveEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserPaused:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserResumed:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGameStarted:(NSString *)sender roomId:(NSString *)roomId  nextTurn:(NSString *)nextTurn
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGameStopped:(NSString *)sender roomId:(NSString *)roomId
{
    NSLog(@"%s",__FUNCTION__);
}

#pragma <RoomRequestListener> methods

- (void)onSubscribeRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUnSubscribeRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onJoinRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onLeaveRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetLiveRoomInfoDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSetCustomRoomDataDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUpdatePropertyDone:(LiveRoomInfoEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onLockPropertiesDone:(Byte)result
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUnlockPropertiesDone:(Byte)result
{
    NSLog(@"%s",__FUNCTION__);
}

#pragma <ZoneRequestListener> methods

- (void)onCreateRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onDeleteRoomDone:(RoomEvent *)roomEvent
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetAllRoomsDone:(AllRoomsEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetOnlineUsersDone:(AllUsersEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetLiveUserInfoDone:(LiveUserInfoEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSetCustomUserDataDone:(LiveUserInfoEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetMatchedRoomsDone:(MatchedRoomsEvent *)event
{
    NSLog(@"%s",__FUNCTION__);
}

#pragma private methods

- (void)initializeAppWarp
{
    [WarpClient initWarp:APPWARP_APP_KEY server:APPWARP_HOST_ADDRESS];
    
    WarpClient *warpClient = [WarpClient getInstance];
    
    [warpClient addChatRequestListener:self];
    [warpClient addConnectionRequestListener:self];
    [warpClient addLobbyRequestListener:self];
    [warpClient addNotificationListener:self];
    [warpClient addRoomRequestListener:self];
    [warpClient addZoneRequestListener:self];
}

@end