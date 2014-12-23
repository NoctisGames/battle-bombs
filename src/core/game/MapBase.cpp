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
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(7, 0)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(7, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 1)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 1)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 2)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 2)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 3)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 3)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(1, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(3, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(5, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(9, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(11, 4)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 4)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 5)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(1, 5)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 5)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 5)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(1, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(5, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 6)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(13, 6)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(9, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 7)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 7)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(3, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(7, 8)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 8)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 9)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 9)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 9)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(13, 9)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 10)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 10)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 10)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 10)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 11)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 11)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(5, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 12)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 12)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 13)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 13)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(9, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(12, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(13, 14)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(14, 14)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(2, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(3, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(4, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 15)));
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(8, 15)));
    
    gameSession->getInsideBlocks().push_back(std::unique_ptr<InsideBlock>(new InsideBlock(6, 16)));
}

void MapBase::update(GameSession *gameSession, float deltaTime, bool isSuddenDeath)
{
    Map::update(gameSession, deltaTime, isSuddenDeath);
    
    if(isSuddenDeath)
    {
        // TODO
    }
}