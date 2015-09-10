//
//  GGDProgressDialog.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "GGDDialog.h"

@interface GGDProgressDialog : GGDDialog
{
    
}

@property (strong, nonatomic) IBOutlet UIView *containerView;
@property (strong, nonatomic) IBOutlet UIActivityIndicatorView *progressActivityIndicatorView;
@property (strong, nonatomic) IBOutlet UILabel *progressMessageLabel;

- (id)initWithMessage:(NSString *)m;

@end