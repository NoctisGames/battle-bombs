//
//  GameViewController.mm
//  battle-bombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "GameViewController.h"
#import "Logger.h"
#import "Music.h"
#import "Sound.h"

#include "game.h"
#include "Assets.h"

@interface GameViewController ()
{
    // Empty
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) Music *bgm;
@property (strong, nonatomic) Sound *plantBombSound;
@property (strong, nonatomic) Sound *explosionSound;
@property (strong, nonatomic) Sound *deathSound;

@end

@implementation GameViewController

static NSString *KEEP_ALIVE = @"KEEP_ALIVE";
static NSString *EVENT_TYPE = @"eventType";
static int CLIENT_UPDATE = 1338;
static NSString *EVENTS = @"events";
static NSString *PLAYER_INDEX = @"playerIndex";
static NSString *X = @"X";
static NSString *Y = @"Y";
static NSString *DIRECTION = @"Direction";

static Logger *logger = nil;

+ (void)initialize
{
    logger = [[Logger alloc] initWithClass:[GameViewController class]];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    if (!self.context)
    {
        [logger error:@"Failed to create ES context"];
    }
    
    GLKView *view = (GLKView *)self.view;
    
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.userInteractionEnabled = YES;
    
    [view setMultipleTouchEnabled:YES];
    
    [self setupGL];
    
    self.plantBombSound = [[Sound alloc] initWithSoundNamed:@"plant_bomb.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:3];
    self.explosionSound = [[Sound alloc] initWithSoundNamed:@"explosion.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.deathSound = [[Sound alloc] initWithSoundNamed:@"death.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onPause)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onResume)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
    [[WarpClient getInstance] addChatRequestListener:self];
    [[WarpClient getInstance] addConnectionRequestListener:self];
    [[WarpClient getInstance] addLobbyRequestListener:self];
    [[WarpClient getInstance] addNotificationListener:self];
    [[WarpClient getInstance] addRoomRequestListener:self];
    [[WarpClient getInstance] addZoneRequestListener:self];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [self onPause];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    on_touch_down(pos.x, pos.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    on_touch_dragged(pos.x, pos.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    on_touch_up(pos.x, pos.y);
}

#pragma mark <GLKViewControllerDelegate>

- (void)update
{
    int gameState = get_state();
    switch (gameState)
    {
        case 0:
            update(self.timeSinceLastUpdate);
            [self pushEvents];
            break;
        case 1:
            init([self.username UTF8String]);
            break;
        case 2:
            [self dismissViewControllerAnimated:true completion:nil];
            break;
        default:
            break;
    }
}

#pragma mark <GLKViewDelegate>

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    present();
    [self handleSound];
    [self handleMusic];
}

#pragma mark <ChatRequestListener>

- (void)onSendChatDone:(Byte)result
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSendPrivateChatDone:(Byte)result
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

#pragma mark <ConnectionRequestListener>

- (void)onConnectDone:(ConnectEvent*) event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onDisconnectDone:(ConnectEvent*) event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onInitUDPDone:(Byte)result
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

#pragma mark <LobbyRequestListener>

- (void)onJoinLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onLeaveLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSubscribeLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUnSubscribeLobbyDone:(LobbyEvent*)lobbyEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetLiveLobbyInfoDone:(LiveRoomInfoEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

#pragma mark <NotifyListener>

- (void)onRoomCreated:(RoomData *)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onRoomDestroyed:(RoomData *)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserLeftRoom:(RoomData *)roomData username:(NSString *)username
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
    
    if([username isEqualToString:self.username])
    {
        [self onPause];
    }
}

- (void)onUserJoinedRoom:(RoomData *)roomData username:(NSString *)username
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserLeftLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserJoinedLobby:(LobbyData *)lobbyData username:(NSString *)username
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
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
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUpdatePeersReceived:(UpdateEvent *)updateEvent
{
    NSLog(@"%s",__FUNCTION__);
}

-(void)onUserChangeRoomProperty:(RoomData *)event username:(NSString *)username properties:(NSDictionary *)properties lockedProperties:(NSDictionary *)lockedProperties
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onMoveCompleted:(MoveEvent *) moveEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserPaused:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUserResumed:(NSString *)userName withLocation:(NSString *)locId isLobby:(BOOL)isLobby
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGameStarted:(NSString *)sender roomId:(NSString *)roomId  nextTurn:(NSString *)nextTurn
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGameStopped:(NSString *)sender roomId:(NSString *)roomId
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

#pragma mark <RoomRequestListener>

- (void)onSubscribeRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUnSubscribeRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onJoinRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onLeaveRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetLiveRoomInfoDone:(LiveRoomInfoEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSetCustomRoomDataDone:(LiveRoomInfoEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUpdatePropertyDone:(LiveRoomInfoEvent *)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onLockPropertiesDone:(Byte)result
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onUnlockPropertiesDone:(Byte)result
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

#pragma mark <ZoneRequestListener>

- (void)onCreateRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onDeleteRoomDone:(RoomEvent*)roomEvent
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetAllRoomsDone:(AllRoomsEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetOnlineUsersDone:(AllUsersEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetLiveUserInfoDone:(LiveUserInfoEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onSetCustomUserDataDone:(LiveUserInfoEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

- (void)onGetMatchedRoomsDone:(MatchedRoomsEvent*)event
{
    // TODO
    NSLog(@"%s",__FUNCTION__);
}

#pragma mark <Private>

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    self.preferredFramesPerSecond = 60;
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    CGSize screenSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
    
    CGSize newSize = CGSizeMake(screenSize.width, screenSize.height);
    newSize.width = roundf(newSize.width);
	newSize.height = roundf(newSize.height);
    
    if([Logger isDebugEnabled])
    {
        [logger debug:[NSString stringWithFormat:@"dimension %f x %f", newSize.width, newSize.height]];
    }
    
    init([self.username UTF8String]);
    on_surface_created(newSize.width, newSize.height);
    on_surface_changed(newSize.width, newSize.height, [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    on_resume();
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
        
        [[WarpClient getInstance] sendChat:eventsMessage];
    }
    else if(is_time_to_send_keep_alive())
    {
        reset_time_since_last_client_event();
        
        [[WarpClient getInstance] sendChat:KEEP_ALIVE];
    }
}

- (void)handleSound
{
    short soundId;
    while ((soundId = get_current_sound_id()) > 0)
    {
        switch (soundId)
        {
            case SOUND_PLANT_BOMB:
                [self.plantBombSound play];
                break;
            case SOUND_EXPLOSION:
                [self.explosionSound play];
                break;
            case SOUND_DEATH:
                [self.deathSound play];
                break;
            default:
                continue;
        }
    }
}

- (void)handleMusic
{
    bool loadedNewTrack = false;
    short musicId = get_current_music_id();
    switch (musicId)
    {
        case MUSIC_STOP:
            [self.bgm stop];
            break;
        case MUSIC_PLAY_MAP_SPACE:
            self.bgm = [[Music alloc] initWithMusicNamed:@"map_space" fromBundle:[NSBundle mainBundle]];
            loadedNewTrack = true;
            break;
        case MUSIC_PLAY_MAP_GRASSLANDS:
            self.bgm = [[Music alloc] initWithMusicNamed:@"map_grasslands" fromBundle:[NSBundle mainBundle]];
            loadedNewTrack = true;
            break;
        case MUSIC_PLAY_MAP_MOUNTAINS:
            self.bgm = [[Music alloc] initWithMusicNamed:@"map_mountains" fromBundle:[NSBundle mainBundle]];
            loadedNewTrack = true;
            break;
        case MUSIC_PLAY_MAP_BASE:
            self.bgm = [[Music alloc] initWithMusicNamed:@"map_base" fromBundle:[NSBundle mainBundle]];
            loadedNewTrack = true;
            break;
        default:
            break;
    }
    
    if(loadedNewTrack)
    {
        [self.bgm setLooping:true];
        [self.bgm setVolume:1.0f];
        [self.bgm play];
    }
}

- (void)onResume
{
    on_resume();
}

- (void)onPause
{
    [self.bgm stop];
    
    on_pause();
    
    WarpClient *warpClient = [WarpClient getInstance];
    
    [warpClient removeChatRequestListener:self];
    [warpClient removeConnectionRequestListener:self];
    [warpClient removeLobbyRequestListener:self];
    [warpClient removeNotificationListener:self];
    [warpClient removeRoomRequestListener:self];
    [warpClient removeZoneRequestListener:self];
    
    [[WarpClient getInstance] leaveRoom:self.joinedRoomId];
    [[WarpClient getInstance] disconnect];
    
    if (![self.presentedViewController isBeingDismissed])
    {
        [self dismissViewControllerAnimated:NO completion:nil];
    }
}

@end