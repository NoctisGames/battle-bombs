//
//  AppDelegate.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/29/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate ()

//Uncomment these if you want to create a full screen game
//@property NSWindow * fullScreenWindow;
//@property ViewController * viewController;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
    //To create a full screen window, uncomment the following code and delete the window in MainMenu.xib
    /*NSRect mainDisplayRect = [NSScreen mainScreen].frame;
    
    self.fullScreenWindow = [[NSWindow alloc] initWithContentRect:mainDisplayRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES];
    self.fullScreenWindow.level = NSMainMenuWindowLevel + 1;
    self.fullScreenWindow.hidesOnDeactivate = YES;
    self.fullScreenWindow.collectionBehavior = NSWindowCollectionBehaviorFullScreenPrimary;
    
    self.viewController = [ViewController new];
    
    
    NSRect windowSize = self.fullScreenWindow.frame;
    CGRect viewRect = CGRectMake(0, 0, windowSize.size.width, windowSize.size.height);
    self.viewController.view = [[PTGLView alloc] initWithFrame:viewRect];
    [self.fullScreenWindow.contentView addSubview:self.viewController.view];
    [self.fullScreenWindow.contentView setAutoresizesSubviews:YES];
    [self.viewController.view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    
    [self.fullScreenWindow makeKeyAndOrderFront:nil];
    
    [self.viewController awakeFromNib];*/
}

@end
