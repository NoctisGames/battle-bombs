//
//  TGProgressDialog.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "TGProgressDialog.h"
#import <QuartzCore/QuartzCore.h>

@interface TGProgressDialog ()
{
    NSString *messageLabelText;
}

@end

@implementation TGProgressDialog

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
