//
//  TGDialog.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "TGDialog.h"
#import "TGDeviceUtil.h"

#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0f * M_PI)

@implementation TGDialog

- (void)show
{
    // Subclasses must override
}

- (void)remove
{
    // Subclasses must override
}

- (NSString *) determineNibName
{
    // Subclasses must override
    return nil;
}

- (void)initView
{
	if([[NSBundle mainBundle] pathForResource:nibName ofType:@"nib"] != nil)
	{
		NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:nibName owner:self options:nil];
		view = [topLevelObjects objectAtIndex:0];
	}
	else
	{
		[NSException raise:@"CNMissingNib" format:@"%@ was not found in SDK bundle or main bundle", nibName];
	}
}

- (void)addToWindow
{
    UIWindow *window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];
	view.frame = window.bounds;
    [window addSubview:view];
    [window setNeedsLayout];
}

- (void)removeFromWindow
{
    [view removeFromSuperview];
    UIWindow *window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];
    [window setNeedsLayout];
}

- (void)transformViewIfNecessary
{
    if(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0"))
    {
        return;
    }
    
    if ([nibName rangeOfString:@"Landscape"].location != NSNotFound)
    {
        CGAffineTransform landscapeTransform = CGAffineTransformMakeRotation(DEGREES_TO_RADIANS(90));
        
        if([TGDeviceUtil isIPhoneFiveScreen])
        {
            landscapeTransform = CGAffineTransformTranslate (landscapeTransform, 124, 124);
        }
        else
        {
            landscapeTransform = CGAffineTransformTranslate (landscapeTransform, 80, 80);
        }
        
        [view setTransform:landscapeTransform];
    }
}

@end