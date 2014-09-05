//
//  OfflineGameViewController.mm
//  battlebombs
//
//  Created by Stephen Gowen on 5/19/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#import "OfflineGameViewController.h"

// C++
#include "game.h"
#include "GameEvent.h"

@interface OfflineGameViewController ()
{
    bool _playersAlive[8];
    int _mapType;
}

@end

@implementation OfflineGameViewController

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    _mapType = 0;
    
    [self handleGameState:1];
}

#pragma mark <Protected>

- (void)handleGameState:(int)gameState
{
    if(gameState == 1)
    {
        _playersAlive[0] = true;
        _playersAlive[1] = true;
        _playersAlive[2] = true;
        _playersAlive[3] = true;
        _playersAlive[4] = true;
        _playersAlive[5] = true;
        _playersAlive[6] = true;
        _playersAlive[7] = true;
        
        clear_state();
        
        NSString *beginGameMessage = [NSString stringWithFormat:@"{\"breakableBlockPowerUpFlags\": \"0,1,2,3,4,5\", \"breakableBlockXValues\": \"8,4,5,7,8,9\", \"breakableBlockYValues\": \"0,1,2,3,4,5\", \"eventType\": 1337, \"mapType\": %i, \"numBreakableBlocks\": 5, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"%@\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Bot 1\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Bot 3\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Bot 4\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Bot 5\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Bot 6\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Bot 7\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}", _mapType, self.username];
        
        on_chat_received([beginGameMessage UTF8String]);
    }
    
    _mapType++;
    if(_mapType >= MAP_BASE)
    {
        _mapType = 0;
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
                NSLog(@"PLAYER_DEATH");
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
    int winningPlayerIndex = -1;
    for (int i = 0; i < 8; i++)
    {
        if(_playersAlive[i])
        {
            winningPlayerIndex = i;
            numAlive++;
            NSLog(@"_playersAlive[%i] = true", i);
        }
        else
        {
            NSLog(@"_playersAlive[%i] = false", i);
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

@end
