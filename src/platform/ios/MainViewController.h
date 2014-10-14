//
//  MainViewController.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/26/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

@interface MainViewController : UIViewController <UIAlertViewDelegate>
{
    // Empty
}

- (IBAction)startQuickMatch:(id)sender;

- (IBAction)startQuickOfflineMatch:(id)sender;

- (void)showConnectionErrorMessage;

@end
