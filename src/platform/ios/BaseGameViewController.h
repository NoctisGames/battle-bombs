//
//  BaseGameViewController.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import <GLKit/GLKit.h>

@class GADBannerView;

@interface BaseGameViewController : GLKViewController
{
    
}

@property (strong, nonatomic) IBOutlet GADBannerView *bannerView;

@property (strong, nonatomic) NSString *username;

#pragma mark <Protected>

- (void)handleGameState:(int)gameState;

- (void)pushEvents;

- (void)handleSound;

- (void)handleMusic;

- (void)onResume;

- (void)onPause;

- (bool)isOffline;

@end
