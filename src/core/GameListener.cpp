//
//  GameListener.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "GameListener.h"
#include "Assets.h"

GameListener::GameListener()
{
    // No setup required
}

void GameListener::playSound(short soundId)
{
    Assets::getInstance()->addSoundIdToPlayQueue(soundId);
}