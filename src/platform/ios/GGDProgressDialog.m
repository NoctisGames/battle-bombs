//
//  GGDProgressDialog.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "GGDProgressDialog.h"
#import <QuartzCore/QuartzCore.h>

@interface GGDProgressDialog ()
{
    NSString *messageLabelText;
}

@end

@implementation GGDProgressDialog

- (id)initWithMessage:(NSString *)m
{
    self = [super init];
    
    nibName = [self determineNibName];
    
    messageLabelText = m;
    
    return self;
}

- (void)show
{
    [super initView];
    
    [self.progressActivityIndicatorView startAnimating];
    
    self.containerView.layer.cornerRadius = 10.0;
    self.progressMessageLabel.text = messageLabelText;
    
    [self transformViewIfNecessary];
    
    [self addToWindow];
}

- (void)remove
{
    [self.progressActivityIndicatorView stopAnimating];
    
    [self removeFromWindow];
}

- (NSString *)determineNibName
{
    return @"Progress_Dialog_Landscape";
}

@end
