//
//  BaseGameViewController.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

/// System Versioning Preprocessor Macros
#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

#import <GLKit/GLKit.h>

@class GADBannerView;

@interface BaseGameViewController : GLKViewController
{
    
}

@property (strong, nonatomic) IBOutlet GADBannerView *bannerView;

@property (strong, nonatomic) NSString *username;

#pragma mark <Protected>

- (void)handleScreenState:(int)screenState;

- (void)pushEvents;

- (void)handleSound;

- (void)handleMusic;

- (void)onResume;

- (void)onPause;

- (bool)isOffline;

@end
