//
//  GGDJsonUtils.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

typedef void (^OnSuccess)(NSDictionary *jsonResponse);

typedef void (^OnError)();

@interface GGDJsonUtils : NSObject

+ (NSData *)readJSONFromFile:(NSString *)fileName withBundle:(NSBundle *)bundle;

+ (NSData *)readJSONFromFile:(NSString *)fileName;

+ (void)generateJsonDictionaryFromData:(NSData *)data withOnSuccessHandler:(OnSuccess)onSuccess andOnErrorHandler:(OnError)onError;

+ (void)generateJsonDictionaryFromString:(NSString *)str withOnSuccessHandler:(OnSuccess)onSuccess andOnErrorHandler:(OnError)onError;

@end