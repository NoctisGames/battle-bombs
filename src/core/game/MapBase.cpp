//
//  MapBase.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 12/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "MapBase.h"
#include "ResourceConstants.h"
#include "GameSession.h"
#include "InsideBlock.h"
#include "Rectangle.h"
#include "Vector2D.h"

bool MapBase::updatePlayerForSuddenDeath(GameSession *gameSession, PlayerDynamicGameObject *player)
{
    // TODO
    
    return false;
}

short MapBase::getMusicId()
{
    return MUSIC_PLAY_MAP_BASE;
}

void MapBase::handleSpectatorModeSuddenDeath(GameSession *gameSession, float timeSinceSuddenDeathModeBegan)
{
    // TODO
}

void MapBase::suddenDeath(GameSession *gameSession)
{
    // TODO
}

void MapBase::initializeMap(GameSession *gameSession)
{
    Map::initializeMap(gameSession);
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 0)));
}

void MapBase::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    Map::update(gameSession, deltaTime, isSuddenDeath);
    
    if(isSuddenDeath)
    {
        // TODO
    }
}