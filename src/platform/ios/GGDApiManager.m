//
//  GGDApiManager.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#define SERVER_STATUS_URL @"https://s3.amazonaws.com/battlebombs/bb_1.3.1.json"

#import "GGDApiManager.h"

#import "Logger.h"
#import "STHTTPRequest.h"
#import "GGDProgressDialog.h"
#import "GGDJsonUtils.h"

@interface GGDApiManager ()
{
    // Empty
}

@property (strong, nonatomic) GGDProgressDialog *currentProgressDialog;

- (void)showProgressDialogWithMessage:(NSString *)message;
- (void)hideProgressDialog;
- (void)processResponseData:(NSData *)responseData withOnSuccessHandler:(OnComplete)onSuccess andOnFailureHandler:(OnComplete)onFailure;

@end

@implementation GGDApiManager

static GGDApiManager *instance = nil;
static Logger *logger = nil;

+ (void)initialize
{
    instance = [[GGDApiManager alloc] init];
    logger = [[Logger alloc] initWithClass:[GGDApiManager class]];
}

+ (id)getInstance
{
    return instance;
}

- (void)sendJsonRequestWithOnSuccess:(OnComplete)os andOnFailure:(OnComplete)of
{
    STHTTPRequest *r = [STHTTPRequest requestWithURLString:SERVER_STATUS_URL];
    [r setHeaderWithName:@"Accept" value:@"application/json"];
    
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    
    [self showProgressDialogWithMessage:NSLocalizedString(@"json_request_progress_message", nil)];
    
    r.completionDataBlock = ^(NSDictionary *headers, NSData *body)
    {
        [self processResponseData:body withOnSuccessHandler:os andOnFailureHandler:of];
    };
    
    r.errorBlock = ^(NSError *error)
    {
        [logger error:[error description]];
        [self processResponseData:nil withOnSuccessHandler:os andOnFailureHandler:of];
    };
    
    [r startAsynchronous];
}

#pragma mark <Private>

- (void)showProgressDialogWithMessage:(NSString *)message
{
    [[UIApplication sharedApplication] endIgnoringInteractionEvents];
    self.currentProgressDialog = [[GGDProgressDialog alloc] initWithMessage:message];
    [self.currentProgressDialog show];
}

- (void)hideProgressDialog
{
    [self.currentProgressDialog remove];
}

- (void)processResponseData:(NSData *)responseData withOnSuccessHandler:(OnComplete)onSuccess andOnFailureHandler:(OnComplete)onFailure
{
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    
	[self hideProgressDialog];
    
    if(responseData)
    {
        NSString *rawResponseString = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
        
        if([Logger isDebugEnabled])
        {
            [logger debug:[[NSString alloc] initWithFormat:@"Http Response Body:\n%@", rawResponseString]];
        }
        
        [GGDJsonUtils generateJsonDictionaryFromString:rawResponseString withOnSuccessHandler:^(NSDictionary *jsonResponse)
        {
            onSuccess(jsonResponse);
        } andOnErrorHandler:^
        {
            onFailure(nil);
        }];
    }
    else
    {
        onFailure(nil);
    }
}

@end
