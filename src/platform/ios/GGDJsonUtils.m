//
//  GGDJsonUtils.m
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "GGDJsonUtils.h"
#import "Logger.h"

@implementation GGDJsonUtils

static Logger *logger = nil;

+ (void)initialize
{
    logger = [[Logger alloc] initWithClass:[GGDJsonUtils class]];
}

+ (NSData *)readJSONFromFile:(NSString *)fileName withBundle:(NSBundle *)bundle
{
    NSString *filePath = [bundle pathForResource:fileName ofType:@"json"];
    
    NSData *jsonData = [NSData dataWithContentsOfFile:filePath];
    
    return jsonData;
}

+ (NSData *)readJSONFromFile:(NSString *)fileName
{
    return [self readJSONFromFile:fileName withBundle:[NSBundle mainBundle]];
}

+ (void)generateJsonDictionaryFromData:(NSData *)data withOnSuccessHandler:(OnSuccess)onSuccess andOnErrorHandler:(OnError)onError
{
    NSError *error;
    NSDictionary *jsonResponse = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
    
    if(error)
    {
        [logger error:@"Error processing JSON data" withError:error];
        
        onError();
    }
    else
    {
        onSuccess(jsonResponse);
    }
}

+ (void)generateJsonDictionaryFromString:(NSString *)str withOnSuccessHandler:(OnSuccess)onSuccess andOnErrorHandler:(OnError)onError
{
    NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
    [GGDJsonUtils generateJsonDictionaryFromData:data withOnSuccessHandler:onSuccess andOnErrorHandler:onError];
}

@end