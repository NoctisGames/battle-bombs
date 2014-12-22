//
//  OfflineGameViewController.mm
//  battlebombs
//
//  Created by Stephen Gowen on 5/19/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#define NUM_OFFLINE_MAPS 6

#import "OfflineGameViewController.h"

// C++
#include "game.h"
#include "GameEvent.h"
#include "ScreenState.h"

@interface OfflineGameViewController ()
{
    bool _playersAlive[8];
    NSMutableArray *_beginGameMessages;
    bool _isSuddenDeath;
    bool _isGameOver;
}

@end

@implementation OfflineGameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _beginGameMessages = [[NSMutableArray alloc] initWithCapacity:12];
    
    [_beginGameMessages addObject:[NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"3,0,0,0,5,0,0,0,0,4,1,0,2,0,5,0,0,6,5,5,1,3,4,6,0,0,0,0,0,3,3,3,0,0,0,4,2,1,0,0,0,4,0,1,3,0,2,0,1,0,6,0,1,0,1,6,2,4,1,0,6,1,0,3,0,4,0,0,5,4,3,0,0,3,0,0,0,2,2,0,0,0,0,0,2,4,1,1,0,0,0,5,1,0,0,0,0,5,0,1,0,0,0,3,0,6,5,0,4,0,0\", \"breakableBlockXValues\": \"8,5,6,8,10,3,9,11,1,2,5,6,7,8,9,10,11,13,2,4,6,8,14,0,1,2,3,4,6,7,8,9,10,11,12,13,14,2,4,6,8,12,2,3,4,6,7,8,9,12,2,6,10,12,2,4,6,8,10,12,3,5,6,7,8,9,10,2,4,6,8,10,0,1,2,3,4,5,6,8,9,11,12,13,14,2,8,10,0,1,2,3,6,11,14,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,12\", \"breakableBlockYValues\": \"0,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 111, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.537313461303711, \"playerIndex1\": \"Roy Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 16.8358211517334, \"playerIndex2\": \"Heather Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 16.8358211517334, \"playerIndex3\": \"Tyler Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.507462739944458, \"playerIndex4\": \"Andre Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Dave Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.537313461303711, \"playerIndex6\": \"Debra Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 17.91044807434082, \"playerIndex6Y\": 2.507462739944458, \"playerIndex7\": \"Stephen Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 25.432836532592773}", self.username]];
    
    [_beginGameMessages addObject:[NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"2,2,1,1,0,0,3,0,1,5,0,0,2,0,3,0,0,0,2,3,0,4,0,3,2,0,2,2,0,0,1,0,3,1,2,1,0,4,0,2,0,0,0,6,3,3,3,2,2,2,2,0,3,5,3,2,0,6,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,2,4,0,3,2,0,1,0,2,0,4,3,0,2,0,3,4,0\", \"breakableBlockXValues\": \"8,5,6,7,8,10,5,9,11,1,2,3,4,6,9,12,13,14,0,2,4,6,8,10,12,14,1,2,3,4,5,6,7,9,10,11,12,2,4,8,3,4,7,10,2,4,6,12,6,10,5,6,7,8,9,10,11,12,4,6,8,1,2,3,4,7,8,10,11,13,14,4,6,8,10,0,1,2,3,7,8,9,10,11,14,4,6,8,10,12,3,6,7,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 96, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.537313461303711, \"playerIndex1\": \"Stephen Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.432836532592773, \"playerIndex2\": \"Barney Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.507462739944458, \"playerIndex3\": \"Kyle Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 12.537313461303711, \"playerIndex4\": \"Andre Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.507462739944458, \"playerIndex5\": \"Pier Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.8358211517334, \"playerIndex6\": \"Glen Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.432836532592773, \"playerIndex7\": \"Randy Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.8358211517334}", self.username]];
    
    [_beginGameMessages addObject:[NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"0,0,0,2,2,0,2,0,4,0,2,0,0,0,0,2,3,5,5,0,0,2,0,0,1,0,2,1,0,1,3,2,5,0,6,0,0,0,0,2,3,0,1,0,4,6,1,0,0,0,1,0,1,0,0,1,3,6,3,0,4,0,0,0,4,0,0,2,0,0,0,5,5,6,0,2,0,5,0,0,1,1,2,0,0,2,0,2,2,0,0,3,0,0,1,3,5,3,0\", \"breakableBlockXValues\": \"6,8,4,7,8,9,10,3,5,7,9,11,0,1,3,6,7,11,12,14,0,2,4,6,12,14,0,1,2,4,6,8,10,11,13,2,8,10,12,3,4,5,7,10,11,6,8,12,2,8,10,5,6,7,10,11,12,4,10,0,1,2,4,5,6,7,8,9,10,11,12,13,2,4,8,10,12,14,0,1,2,3,7,8,9,10,13,14,2,4,5,7,8,12,2,3,5,9,10\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,8,8,8,9,9,9,10,10,10,10,10,10,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 99, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 16.8358211517334, \"playerIndex1\": \"Brandi Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.537313461303711, \"playerIndex2\": \"Bradlee Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 16.8358211517334, \"playerIndex3\": \"Heather Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.537313461303711, \"playerIndex4\": \"Matt Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Glen Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 6.447761058807373, \"playerIndex5Y\": 2.507462739944458, \"playerIndex6\": \"Barney Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 17.91044807434082, \"playerIndex6Y\": 2.507462739944458, \"playerIndex7\": \"Dean Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 25.432836532592773}", self.username]];
    
    [_beginGameMessages addObject:[NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"0,1,0,0,1,1,3,2,3,5,2,1,4,0,3,0,2,2,4,0,0,3,0,0,0,0,5,0,1,0,0,3,0,1,3,0,3,0,2,2,3,3,2,0,1,0,0,1,3,0,0,3,0,0,0,0,0,0,2,3,2,4,0,0,0,0,6,2,0,4,0,1,5,6,5,1,0,3,0,2,0,5,3,5,2,0,0,1,4,0,2,0,0,0,0,1\", \"breakableBlockXValues\": \"6,8,4,5,6,7,8,9,10,3,5,7,11,0,1,2,3,4,5,6,7,9,11,13,12,2,4,5,6,8,9,10,12,6,10,2,3,4,6,7,8,10,11,2,6,8,12,2,4,10,12,2,3,4,7,9,10,2,8,10,12,0,10,11,12,13,14,0,2,4,8,10,12,14,1,2,3,5,6,7,9,10,11,12,13,4,6,8,12,2,3,5,7,8,10,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,4,5,5,5,5,5,5,5,5,6,6,7,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 96, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 6.447761058807373, \"playerIndex0Y\": 2.507462739944458, \"playerIndex1\": \"Kenny Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.432836532592773, \"playerIndex2\": \"Wesley Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 12.537313461303711, \"playerIndex3\": \"Tyler Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 17.91044807434082, \"playerIndex3Y\": 2.507462739944458, \"playerIndex4\": \"Morgan Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 16.8358211517334, \"playerIndex5\": \"Paul Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.537313461303711, \"playerIndex6\": \"Lily Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.432836532592773, \"playerIndex7\": \"Bruce Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 16.8358211517334}", self.username]];
    
    [_beginGameMessages addObject:[NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"4,0,5,0,0,6,0,2,0,3,3,0,2,3,3,2,0,5,0,3,0,0,5,5,3,0,0,4,2,2,0,0,0,0,2,3,0,0,0,3,0,3,0,0,0,0,0,2,1,0,1,2,0,0,2,4,0,0,0,0,5,0,0,3,0,2,5,0,4,3,5,0,0,0,0,0,0,6,2,0,3,1,0,3,3,0,0,0,0,0,6,0,0,0,5,1,2\", \"breakableBlockXValues\": \"6,4,6,9,7,9,11,0,1,2,3,7,11,14,0,4,8,12,14,0,1,2,3,5,6,8,10,11,14,2,4,6,8,12,2,5,6,8,9,10,11,12,2,4,10,2,4,6,10,2,6,9,10,11,2,4,6,8,12,0,1,2,3,5,6,7,8,10,12,13,2,4,8,10,12,1,2,3,5,6,7,8,11,12,13,14,4,6,8,10,12,3,4,5,7,8,10\", \"breakableBlockYValues\": \"0,1,1,1,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,9,9,9,9,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 97, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.8358211517334, \"playerIndex1\": \"Tyler Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.537313461303711, \"playerIndex2\": \"Joe Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.507462739944458, \"playerIndex3\": \"Lily Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.432836532592773, \"playerIndex4\": \"Brandi Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Kathy Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 6.447761058807373, \"playerIndex5Y\": 2.507462739944458, \"playerIndex6\": \"Dexter Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 16.8358211517334, \"playerIndex7\": \"Paul Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 12.537313461303711}", self.username]];
    
    [_beginGameMessages addObject:[NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"0,0,0,2,2,4,0,3,1,5,4,0,0,0,0,6,6,0,3,0,0,2,1,4,0,0,3,1,3,0,1,1,3,5,0,0,0,1,2,0,1,6,3,0,5,0,0,0,0,0,1,0,0,3,3,3,1,6,0,0,0,0,4,4,2,0,0,0,0,0,0,0,0,0,0,1,0,6,3,3,0,3,3,0,1,6,2,0,0,3,5,0,0,0,2,1,2,1,1,3,0,5,1,0,5,0,3,0,0,0\", \"breakableBlockXValues\": \"6,8,4,5,6,7,9,5,7,0,3,4,5,7,9,10,11,12,13,14,0,4,6,8,10,12,14,1,4,5,6,9,11,12,13,14,2,4,6,8,10,3,4,6,7,8,9,11,4,6,10,12,2,4,6,8,10,2,3,4,6,7,8,9,10,11,12,4,6,8,10,12,0,3,4,5,6,7,9,10,11,12,13,14,0,2,4,6,10,12,14,1,2,3,7,8,10,12,13,14,2,5,6,12,3,4,5,10,11,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 110, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 6.447761058807373, \"playerIndex0Y\": 2.507462739944458, \"playerIndex1\": \"Eric Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 25.432836532592773, \"playerIndex2\": \"Frank Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.507462739944458, \"playerIndex3\": \"James Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 12.537313461303711, \"playerIndex4\": \"Ryan Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Roy Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 12.537313461303711, \"playerIndex6\": \"Alfred Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 16.8358211517334, \"playerIndex7\": \"Ted Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.8358211517334}", self.username]];
    
    [self performSelector:@selector(handleScreenStateOfflineModeNextMap) withObject:nil afterDelay:3];
}

#pragma mark <Protected>

- (void)handleScreenState:(int)screenState
{
    if(screenState == SCREEN_STATE_OFFLINE_MODE_NEXT_MAP)
    {
        _playersAlive[0] = true;
        _playersAlive[1] = true;
        _playersAlive[2] = true;
        _playersAlive[3] = true;
        _playersAlive[4] = true;
        _playersAlive[5] = true;
        _playersAlive[6] = true;
        _playersAlive[7] = true;
        
        _isSuddenDeath = false;
        _isGameOver = false;
        
        clear_state();
        
        srand((int)time(NULL));
        
        int beginGameMessagesIndex = rand() % NUM_OFFLINE_MAPS;
        
        NSString *beginGameMessage = [_beginGameMessages objectAtIndex:beginGameMessagesIndex];
        
        on_chat_received([beginGameMessage UTF8String]);
    }
}

- (void)pushEvents
{
    int event;
    while ((event = get_oldest_event_id()) > 0)
    {
        int playerIndex = 0;
        while(event >= PLAYER_EVENT_BASE)
        {
            event -= PLAYER_EVENT_BASE;
            playerIndex++;
        }
        
        while (event >= PLAYER_EVENT_DIRECTION_BASE)
        {
            event -= PLAYER_EVENT_DIRECTION_BASE;
        }
        
        while (event >= PLAYER_EVENT_GRID_X_BASE)
        {
            event -= PLAYER_EVENT_GRID_X_BASE;
        }
        
        while (event >= PLAYER_EVENT_GRID_Y_BASE)
        {
            event -= PLAYER_EVENT_GRID_Y_BASE;
        }
        
        while (event >= PLAYER_EVENT_MOD_BASE)
        {
            event -= PLAYER_EVENT_MOD_BASE;
        }
        
        switch (event)
        {
            case PLAYER_DEATH:
            case PLAYER_ABOUT_TO_FALL:
            case PLAYER_FREEZE:
                [self handleDeathForPlayerIndex:playerIndex];
                continue;
            default:
                continue;
        }
    }
    
    if(get_num_seconds_left() <= 0)
    {
        NSString *gameOverMessage = [NSString stringWithFormat:@"{\"eventType\": %i, \"hasWinner\": false, \"winningPlayerIndex\": %i}", GAME_OVER, -1];
        
        on_chat_received([gameOverMessage UTF8String]);
    }
    else if(!_isSuddenDeath && get_num_seconds_left() <= 60)
    {
        NSString *suddenDeathMessage = [NSString stringWithFormat:@"{\"eventType\": %i}", SUDDEN_DEATH];
        
        on_chat_received([suddenDeathMessage UTF8String]);
        
        _isSuddenDeath = true;
    }
}

- (bool)isOffline
{
    return true;
}

#pragma mark <Private>

- (void)handleDeathForPlayerIndex:(int)playerIndex
{
    _playersAlive[playerIndex] = false;
    
    int numAlive = 0;
    for (int i = 0; i < 8; i++)
    {
        if(_playersAlive[i])
        {
            numAlive++;
        }
    }
    
    if(numAlive <= 1 && !_isGameOver)
    {
        _isGameOver = true;
        [self performSelector:@selector(handleGameOver) withObject:nil afterDelay:0.5];
    }
}

- (void)handleGameOver
{
    int numAlive = 0;
    int winningPlayerIndex = -1;
    for (int i = 0; i < 8; i++)
    {
        if(_playersAlive[i])
        {
            winningPlayerIndex = i;
            numAlive++;
        }
    }
    
    if(numAlive <= 1)
    {
        bool hasWinner = numAlive == 1;
        
        NSString *gameOverMessage;
        if(hasWinner)
        {
            gameOverMessage = [NSString stringWithFormat:@"{\"eventType\": %i, \"hasWinner\": true, \"winningPlayerIndex\": %i}", GAME_OVER, winningPlayerIndex];
        }
        else
        {
            gameOverMessage = [NSString stringWithFormat:@"{\"eventType\": %i, \"hasWinner\": false, \"winningPlayerIndex\": %i}", GAME_OVER, winningPlayerIndex];
        }
        
        on_chat_received([gameOverMessage UTF8String]);
    }
}

- (void)handleScreenStateOfflineModeNextMap
{
    [self handleScreenState:SCREEN_STATE_OFFLINE_MODE_NEXT_MAP];
}

@end
