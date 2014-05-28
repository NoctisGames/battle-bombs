//
//  SoundListener.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/28/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "SoundListener.h"
#include "Assets.h"

SoundListener::SoundListener()
{
    
}

void SoundListener::playSound(short soundId)
{
    Assets::getInstance()->addSoundIdToPlayQueue(soundId);
}