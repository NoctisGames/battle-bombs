//
//  OfflineGameViewController.mm
//  bomber-party
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
    
    self.bgm = [[Music alloc] initWithMusicNamed:@"bg_game" fromBundle:[NSBundle mainBundle]];
    [self.bgm setLooping:true];
    [self.bgm setVolume:1.0f];
    
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
    
    on_chat_received([@"{\"playerIndex0\":\"Player_Offline\",\"numPlayers\":1,\"breakableBlockPowerUpFlags\":\"4,1,1,0,0,4,0,0,3,4,0,4,0,3,0,3,4,4,4,0,0,2,4,4,0,1,3,4,2,0,1,2,4,0,3,4,0,3,1,0,3,4,0,2,0,0,4,3,3,1,1,4,3,3,4,4,3,2,4,0,0,1,1,0,4,3,0,3,0,0,0,0,4,4,0,2,1,0,1,1,0,4,0,4,1,2,2,2,2,2,4,4,0,4,1,1,0,0,1,0,3,3,0,4,0,0,0,3,0,0,0,2,3,2,3,0,0,0,4,4,1,0,4,2,0,4,3,1,0,3,3,0,1,0,0,1,4,4\",\"breakableBlockYValues\":\"0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,8,8,8,8,8,8,8,8,8,9,9,9,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16\",\"breakableBlockXValues\":\"2,3,4,6,8,9,10,15,16,2,8,10,14,16,0,1,3,4,5,6,9,12,14,15,16,17,0,2,4,6,8,10,12,16,1,4,5,6,7,8,9,10,12,13,14,15,17,0,4,6,8,10,12,16,1,5,6,8,10,13,14,16,17,6,10,12,16,2,5,7,8,10,12,14,15,16,2,8,16,2,5,6,9,12,14,16,4,10,16,0,1,2,4,5,6,7,8,11,12,13,14,15,2,6,8,10,12,16,0,1,2,5,6,7,8,9,10,12,14,15,16,18,2,6,8,10,12,14,16,2,3,5,6,7,9,14,15,16\",\"eventType\":1337,\"playerIndex0X\":3.747559070587158,\"numBreakableBlocks\":138,\"playerIndex0Direction\":0,\"playerIndex0Y\":3.418919086456299}" UTF8String]);
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
    short musicId = get_current_music_id();
    switch (musicId)
    {
        case MUSIC_STOP:
            [self.bgm stop];
            break;
        case MUSIC_PLAY:
            [self.bgm play];
            break;
        default:
            break;
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
    
    [self dismissViewControllerAnimated:NO completion:nil];
}

@end
