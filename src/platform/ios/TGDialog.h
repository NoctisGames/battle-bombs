//
//  TGDialog.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

@interface TGDialog : UIView
{
@protected
    NSString *nibName;
    UIView *view;
}

- (void)show;

- (void)remove;

- (NSString *) determineNibName;

- (void)initView;

- (void)addToWindow;

- (void)removeFromWindow;

- (void)transformViewIfNecessary;

@end