//
//  TGApiManager.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/8/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * Callback that is fired once a request has returned
 *
 * @param jsonResponse
 */
typedef void (^OnComplete)(NSDictionary *jsonResponse);

@class CMBaseRequest;

@interface TGApiManager : NSObject

+ (id)getInstance;

- (void)sendJsonRequestWithOnSuccess:(OnComplete)os andOnFailure:(OnComplete)of;

@end
