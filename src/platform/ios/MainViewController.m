//
//  MainViewController.m
//  battlebombs
//
//  Created by Stephen Gowen on 2/26/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "MainViewController.h"
#import "BaseGameViewController.h"
#import "GGDApiManager.h"

@interface MainViewController ()
{
    // Empty
}

@property (strong, nonatomic) NSString *username;

@end

@implementation MainViewController

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    BaseGameViewController *gameViewController = (BaseGameViewController *) segue.destinationViewController;
    gameViewController.username = _username;
    
    [super prepareForSegue:segue sender:sender];
}

- (IBAction)startQuickMatch:(id)sender
{
    OnComplete onSuccess = ^(NSDictionary *jsonResponse)
    {
        BOOL isCurrentVersion = [[jsonResponse objectForKey:@"isCurrentVersion"] boolValue];
        if(isCurrentVersion)
        {
            BOOL isDownForMaintenance = [[jsonResponse objectForKey:@"isDownForMaintenance"] boolValue];
            if(isDownForMaintenance)
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"down_for_maintenance_title", nil)
                                                                message:NSLocalizedString(@"down_for_maintenance_message", nil)
                                                               delegate:self
                                                      cancelButtonTitle:NSLocalizedString(@"down_for_maintenance_cancel", nil)
                                                      otherButtonTitles:nil];
                [alert show];
            }
            else
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"enter_username_title", nil)
                                                                message:nil
                                                               delegate:self
                                                      cancelButtonTitle:NSLocalizedString(@"enter_username_cancel", nil)
                                                      otherButtonTitles:NSLocalizedString(@"enter_username_button", nil), nil];
                
                alert.alertViewStyle = UIAlertViewStylePlainTextInput;
                UITextField *alertTextField = [alert textFieldAtIndex:0];
                alertTextField.keyboardType = UIKeyboardTypeDefault;
                alertTextField.placeholder = NSLocalizedString(@"enter_username_hint", nil);
                
                NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
                _username = [userDefaults objectForKey:@"USERNAME"];
                [alertTextField setText:_username];
                
                [alert show];
            }
        }
        else
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"update_required_title", nil)
                                                            message:NSLocalizedString(@"update_required_message", nil)
                                                           delegate:self
                                                  cancelButtonTitle:NSLocalizedString(@"update_required_cancel", nil)
                                                  otherButtonTitles:nil];
            [alert show];
        }
    };
    
    OnComplete onFailure = ^(NSDictionary *jsonResponse)
    {
        [self showConnectionErrorMessage];
    };
    
    [[GGDApiManager getInstance] sendJsonRequestWithOnSuccess:onSuccess andOnFailure:onFailure];
}

- (IBAction)startQuickOfflineMatch:(id)sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"enter_username_title", nil)
                                                    message:nil
                                                   delegate:self
                                          cancelButtonTitle:NSLocalizedString(@"enter_username_cancel", nil)
                                          otherButtonTitles:NSLocalizedString(@"enter_username_play", nil), nil];
    
    alert.alertViewStyle = UIAlertViewStylePlainTextInput;
    UITextField *alertTextField = [alert textFieldAtIndex:0];
    alertTextField.keyboardType = UIKeyboardTypeDefault;
    alertTextField.placeholder = NSLocalizedString(@"enter_username_hint", nil);
    
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    _username = [userDefaults objectForKey:@"USERNAME"];
    [alertTextField setText:_username];
    
    [alert show];
}

#pragma mark <UIAlertViewDelegate>

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSString *title = [alertView buttonTitleAtIndex:buttonIndex];
    
    if([alertView.title isEqualToString:NSLocalizedString(@"enter_username_title", nil)])
    {
        if([title isEqualToString:NSLocalizedString(@"enter_username_button", nil)])
        {
            _username = [alertView textFieldAtIndex:0].text;
            
            if(_username.length >= 3 && _username.length <= 12)
            {
                [self performSegueWithIdentifier:@"Main_To_Game" sender:self];
            }
            else
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"invalid_username_title", nil)
                                                                message:NSLocalizedString(@"invalid_username_message", nil)
                                                               delegate:self
                                                      cancelButtonTitle:NSLocalizedString(@"invalid_username_cancel", nil)
                                                      otherButtonTitles:nil];
                [alert show];
            }
        }
        else if([title isEqualToString:NSLocalizedString(@"enter_username_play", nil)])
        {
            _username = [alertView textFieldAtIndex:0].text;
            
            if(_username.length >= 3 && _username.length <= 12)
            {
                [self performSegueWithIdentifier:@"Main_To_Offline_Game" sender:self];
            }
            else
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"invalid_username_title", nil)
                                                                message:NSLocalizedString(@"invalid_username_message", nil)
                                                               delegate:self
                                                      cancelButtonTitle:NSLocalizedString(@"invalid_username_cancel", nil)
                                                      otherButtonTitles:nil];
                [alert show];
            }
        }
        
        NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
        [userDefaults setObject:_username forKey:@"USERNAME"];
        [userDefaults synchronize];
    }
}

#pragma mark <Public API>

- (void)showConnectionErrorMessage
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"connection_error_title", nil)
                                                    message:NSLocalizedString(@"connection_error_message", nil)
                                                   delegate:self
                                          cancelButtonTitle:NSLocalizedString(@"connection_error_cancel", nil)
                                          otherButtonTitles:nil];
    [alert show];
}

@end
