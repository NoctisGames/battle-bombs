//
//  GameStateFactory.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameStateFactory.h"
#include "GameConstants.h"
#include "GameStateWaitingForConnection.h"
#include "GameStateConnectionErrorWaitingForInput.h"
#include "GameStateWaitingForServer.h"
#include "GameStateWaitingForLocalSettings.h"
#include "GameStateCountingDownOffline.h"
#include "GameStateSpectatingOffline.h"
#include "GameStateRunningOffline.h"
#include "GameStateEndingOffline.h"
#include "GameStateCountingDown.h"
#include "GameStateSpectating.h"
#include "GameStateRunning.h"
#include "GameStateEnding.h"

GameState * GameStateFactory::createGameState(int gameState, bool isOffline)
{
    switch (gameState)
    {
        case GAME_STATE_WAITING_FOR_CONNECTION:
            return new GameStateWaitingForConnection();
        case GAME_STATE_CONNECTION_ERROR_WAITING_FOR_INPUT:
            return new GameStateConnectionErrorWaitingForInput();
        case GAME_STATE_WAITING_FOR_SERVER:
            return new GameStateWaitingForServer();
        case GAME_STATE_WAITING_FOR_LOCAL_SETTINGS:
            return new GameStateWaitingForLocalSettings();
        case GAME_STATE_COUNTING_DOWN:
            return isOffline ? new GameStateCountingDownOffline() : new GameStateCountingDown();
        case GAME_STATE_SPECTATING:
            return isOffline ? new GameStateSpectatingOffline() : new GameStateSpectating();
        case GAME_STATE_RUNNING:
            return isOffline ? new GameStateRunningOffline() : new GameStateRunning();
        case GAME_STATE_ENDING:
            return isOffline ? new GameStateEndingOffline() : new GameStateEnding();
        default:
            return nullptr;
    }
}