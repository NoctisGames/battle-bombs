//
//  Assets.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomber_party__Assets__
#define __bomber_party__Assets__

#include "ResourceConstants.h"
#include "PowerUpType.h"

#include <vector>

class TextureRegion;
class InsideBlock;
class BreakableBlock;
class PlayerDynamicGameObject;
class BombGameObject;
class Fire;
class BreakableBlock;
class PowerUp;

class Assets
{
public:
	static Assets * getInstance();
    
    static TextureRegion getWorldBackgroundTextureRegion();
    
    static TextureRegion getInsideBlockTextureRegion();

	static TextureRegion getBreakableBlockTextureRegion(BreakableBlock &breakableBlock);
    
    static TextureRegion getInterfaceOverlayTextureRegion();

	static TextureRegion getPowerUpTextureRegion(PowerUp &powerUp);
    
    static TextureRegion getPlayerTextureRegion(PlayerDynamicGameObject &player);
    
    static TextureRegion getBombTextureRegion(BombGameObject &bomb);
    
    static TextureRegion getFireTextureRegion(Fire &fire);
    
    void setMusicId(short musicId);
    
    short getMusicId();
    
    short getFirstSoundId();
    
    void addSoundIdToPlayQueue(short soundId);
    
    void eraseFirstSoundId();

private:
    Assets();
    
    static int getKeyFrameNumber(float stateTime, float cycleTime, std::vector<float> &frameDurations);
    
    std::vector<short> m_sSoundIds;
    short m_sMusicId;
};

#endif /* defined(__bomber_party__Assets__) */