//
//  OfflineGameViewController.mm
//  battle-bombs
//
//  Created by Stephen Gowen on 5/19/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "OfflineGameViewController.h"
#import "Logger.h"
#import "Music.h"
#import "Sound.h"

#include "game.h"
#include "ResourceConstants.h"

@interface OfflineGameViewController ()
{
    // Empty
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) Music *bgm;
@property (strong, nonatomic) Sound *plantBombSound;
@property (strong, nonatomic) Sound *explosionSound;
@property (strong, nonatomic) Sound *deathSound;

@end

@implementation OfflineGameViewController

static Logger *logger = nil;

+ (void)initialize
{
    logger = [[Logger alloc] initWithClass:[OfflineGameViewController class]];
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
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    on_chat_received([@"{\"breakableBlockPowerUpFlags\": \"3,1,0,1,0,4,4,1,0,4,1,0,3,2,1,1,0,3,0,2,0,4,1,0,0,0,0,3,2,4,3,3,1,0,2,4,3,4,4,4,0,1,4,2,3,3,4,2,0,4,0,1,2,1,4,3,2,1,4,4,0,4,0,4,3,1,0,0,4,4,0,0,4,3,3,0,0,0,0,1,2,0,3,2,0,1,0,4,4,4,3,0,4,4,0,0,4,4,0,2,2,0,2,0,4\", \"breakableBlockXValues\": \"8,4,5,7,8,9,10,5,7,11,0,1,3,4,5,7,10,11,12,14,2,6,8,10,14,0,2,5,6,7,9,10,11,12,14,2,4,6,8,12,2,3,7,10,12,6,8,10,12,2,4,6,8,12,2,6,7,8,9,10,12,2,4,6,10,0,1,2,3,4,5,8,9,10,11,12,14,0,4,6,8,10,12,14,1,2,4,5,7,8,9,10,12,13,2,4,6,7,10,4,5,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 105, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"Player_Offline\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Bot 1\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Bot 3\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Bot 4\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Bot 5\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Bot 6\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Bot 7\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}" UTF8String]);
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
            break;
        case 1:
            init([@"Player_Offline" UTF8String]);
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
    
    init([@"Player_Offline" UTF8String]);
    on_surface_created(newSize.width, newSize.height);
    on_surface_changed(newSize.width, newSize.height, [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    on_resume();
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
    
    if (![self.presentedViewController isBeingDismissed])
    {
        [self dismissViewControllerAnimated:NO completion:nil];
    }
}

@end
