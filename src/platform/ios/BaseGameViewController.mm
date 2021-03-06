//
//  BaseGameViewController.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "BaseGameViewController.h"
#import "Logger.h"
#import "Music.h"
#import "Sound.h"
#import "UIView+Toast.h"

// C++
#include "game.h"
#include "ResourceConstants.h"
#include "ScreenState.h"

// AdMob
#import "GADBannerView.h"
#import "GADRequest.h"

@interface BaseGameViewController ()
{
    // Empty
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) Music *bgm;
@property (strong, nonatomic) Sound *countDown3Sound;
@property (strong, nonatomic) Sound *countDown2Sound;
@property (strong, nonatomic) Sound *countDown1Sound;
@property (strong, nonatomic) Sound *baseTileTriggerSound;
@property (strong, nonatomic) Sound *miniExplosionSound;
@property (strong, nonatomic) Sound *battleSound;
@property (strong, nonatomic) Sound *plantBombSound;
@property (strong, nonatomic) Sound *explosionSound;
@property (strong, nonatomic) Sound *powerUpBombSound;
@property (strong, nonatomic) Sound *powerUpFireSound;
@property (strong, nonatomic) Sound *powerUpSpeedSound;
@property (strong, nonatomic) Sound *powerUpForceFieldSound;
@property (strong, nonatomic) Sound *powerUpPushSound;
@property (strong, nonatomic) Sound *powerUpShieldSound;
@property (strong, nonatomic) Sound *powerUpMegaFireSound;
@property (strong, nonatomic) Sound *powerUpRemoteBombSound;
@property (strong, nonatomic) Sound *powerUpLandmineSound;
@property (strong, nonatomic) Sound *powerUpCurseSound;
@property (strong, nonatomic) Sound *forceFieldDownSound;
@property (strong, nonatomic) Sound *deathSound;
@property (strong, nonatomic) Sound *hurrySound;
@property (strong, nonatomic) Sound *gameSetSound;
@property (strong, nonatomic) Sound *drawSound;
@property (strong, nonatomic) Sound *raiseShieldSound;
@property (strong, nonatomic) Sound *dislodgingSpaceTileSound;
@property (strong, nonatomic) Sound *fallingSpaceTileSound;
@property (strong, nonatomic) Sound *fallingObjectSound;
@property (strong, nonatomic) Sound *crashingFireBallSound;
@property (strong, nonatomic) Sound *crashingIceBallSound;

@end

@implementation BaseGameViewController

static Logger *logger = nil;
static bool isRunningiOS8 = false;

+ (void)initialize
{
    logger = [[Logger alloc] initWithClass:[BaseGameViewController class]];
    isRunningiOS8 = SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0");
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context)
    {
        [logger error:@"Failed to create ES context"];
    }
    
    GLKView *view = (GLKView *)self.view;
    
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.userInteractionEnabled = YES;
    [view setMultipleTouchEnabled:YES];
    
    self.preferredFramesPerSecond = 60;
    
    self.countDown3Sound = [[Sound alloc] initWithSoundNamed:@"countdown_3.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.countDown2Sound = [[Sound alloc] initWithSoundNamed:@"countdown_2.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.countDown1Sound = [[Sound alloc] initWithSoundNamed:@"countdown_1.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.baseTileTriggerSound = [[Sound alloc] initWithSoundNamed:@"base_tile_trigger.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:3];
    self.miniExplosionSound = [[Sound alloc] initWithSoundNamed:@"mini_explosion.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.battleSound = [[Sound alloc] initWithSoundNamed:@"battle.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.plantBombSound = [[Sound alloc] initWithSoundNamed:@"plant_bomb.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:3];
    self.explosionSound = [[Sound alloc] initWithSoundNamed:@"explosion.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.powerUpBombSound = [[Sound alloc] initWithSoundNamed:@"pu_bomb.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpFireSound = [[Sound alloc] initWithSoundNamed:@"pu_fire.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpSpeedSound = [[Sound alloc] initWithSoundNamed:@"pu_speed.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpForceFieldSound = [[Sound alloc] initWithSoundNamed:@"pu_force_field.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpPushSound = [[Sound alloc] initWithSoundNamed:@"pu_push.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpShieldSound = [[Sound alloc] initWithSoundNamed:@"pu_shield.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpMegaFireSound = [[Sound alloc] initWithSoundNamed:@"pu_mega_fire.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpRemoteBombSound = [[Sound alloc] initWithSoundNamed:@"pu_remote_bomb.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpLandmineSound = [[Sound alloc] initWithSoundNamed:@"pu_landmine.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.powerUpCurseSound = [[Sound alloc] initWithSoundNamed:@"pu_curse.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.forceFieldDownSound = [[Sound alloc] initWithSoundNamed:@"force_field_down.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.deathSound = [[Sound alloc] initWithSoundNamed:@"death.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:2];
    self.hurrySound = [[Sound alloc] initWithSoundNamed:@"hurry.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.gameSetSound = [[Sound alloc] initWithSoundNamed:@"game_set.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.drawSound = [[Sound alloc] initWithSoundNamed:@"draw.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.raiseShieldSound = [[Sound alloc] initWithSoundNamed:@"shield_raise.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
    self.dislodgingSpaceTileSound = [[Sound alloc] initWithSoundNamed:@"dislodging_space_tile.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.fallingSpaceTileSound = [[Sound alloc] initWithSoundNamed:@"falling_space_tile.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.fallingObjectSound = [[Sound alloc] initWithSoundNamed:@"falling_object.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.crashingFireBallSound = [[Sound alloc] initWithSoundNamed:@"crashing_fire_ball.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    self.crashingIceBallSound = [[Sound alloc] initWithSoundNamed:@"crashing_ice_ball.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:6];
    
    [EAGLContext setCurrentContext:self.context];
    
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
    
    init([self.username UTF8String], [self isOffline], isRunningiOS8);
    
    on_surface_created(newSize.width, newSize.height);
    
    on_surface_changed(newSize.width, newSize.height, [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    on_resume();
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onPause)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onResume)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
    self.bannerView.adUnitID = @"ca-app-pub-6190265373070512/7683862389";
    self.bannerView.rootViewController = self;
    
    GADRequest *request = [GADRequest request];
    [self.bannerView loadRequest:request];
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
    int screenState = get_state();
    switch (screenState)
    {
        case SCREEN_STATE_ENTERED_SPECTATOR_MODE:
            [self performSelectorOnMainThread:@selector(showToastWithMessage:) withObject:NSLocalizedString(@"you_are_in_spectator_mode", nil) waitUntilDone:NO];
            clear_state();
        case SCREEN_STATE_NORMAL:
            update(self.timeSinceLastUpdate);
            [self pushEvents];
        default:
            [self handleScreenState:screenState];
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

#pragma mark <Protected>

- (void)handleScreenState:(int)screenState
{
    // Override in subclass
}

- (void)pushEvents
{
    // Override in subclass
}

- (void)handleSound
{
    short soundId;
    while ((soundId = get_current_sound_id()) > 0)
    {
        switch (soundId)
        {
            case SOUND_COUNT_DOWN_3:
                [self.countDown3Sound play];
                break;
            case SOUND_COUNT_DOWN_2:
                [self.countDown2Sound play];
                break;
            case SOUND_COUNT_DOWN_1:
                [self.countDown1Sound play];
                break;
            case SOUND_BATTLE:
                [self.battleSound play];
                break;
            case SOUND_PLANT_BOMB:
                [self.plantBombSound play];
                break;
            case SOUND_EXPLOSION:
                [self.explosionSound play];
                break;
            case SOUND_PU_BOMB:
                [self.powerUpBombSound play];
                break;
            case SOUND_PU_FIRE:
                [self.powerUpFireSound play];
                break;
            case SOUND_PU_SPEED:
                [self.powerUpSpeedSound play];
                break;
            case SOUND_PU_FORCE_FIELD:
                [self.powerUpForceFieldSound play];
                break;
            case SOUND_PU_PUSH:
                [self.powerUpPushSound play];
                break;
            case SOUND_PU_SHIELD:
                [self.powerUpShieldSound play];
                break;
            case SOUND_PU_MEGA_FIRE:
                [self.powerUpMegaFireSound play];
                break;
            case SOUND_PU_REMOTE_BOMB:
                [self.powerUpRemoteBombSound play];
                break;
            case SOUND_PU_LAND_MINE:
                [self.powerUpLandmineSound play];
                break;
            case SOUND_PU_CURSE:
                [self.powerUpCurseSound play];
                break;
            case SOUND_FORCE_FIELD_DOWN:
                [self.forceFieldDownSound play];
                break;
            case SOUND_DEATH:
                [self.deathSound play];
                break;
            case SOUND_HURRY:
                [self.hurrySound play];
                break;
            case SOUND_GAME_SET:
                [self.gameSetSound play];
                break;
            case SOUND_DRAW:
                [self.drawSound play];
                break;
            case SOUND_RAISE_SHIELD:
                [self.raiseShieldSound play];
                break;
            case SOUND_DISLODGING_SPACE_TILE:
                [self.dislodgingSpaceTileSound play];
                break;
            case SOUND_FALLING_SPACE_TILE:
                [self.fallingSpaceTileSound play];
                break;
            case SOUND_FALLING_OBJECT:
                [self.fallingObjectSound play];
                break;
            case SOUND_CRASHING_FIRE_BALL:
                [self.crashingFireBallSound play];
                break;
            case SOUND_CRASHING_ICE_BALL:
                [self.crashingIceBallSound play];
                break;
            case SOUND_MINI_EXPLOSION:
                [self.miniExplosionSound play];
                break;
            case SOUND_BASE_TILE_TRIGGER:
                [self.baseTileTriggerSound play];
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

- (bool)isOffline
{
    return false;
}

#pragma mark <Private>

- (void)showToastWithMessage:(NSString *)message
{
    [self.view makeToast:message];
}

@end