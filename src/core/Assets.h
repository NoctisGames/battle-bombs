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
class MapBorder;
class BreakableBlock;
class PlayerDynamicGameObject;
class BombGameObject;
class Fire;
class BreakableBlock;
class PowerUp;
class PlayerAvatar;
class DPadControl;
class PowerUpBarItem;
class ActiveButton;
class BombButton;

class Assets
{
public:
	static Assets * getInstance();
    
    static TextureRegion getWorldBackgroundTextureRegion();
    
    static TextureRegion getMapBorderTextureRegion(MapBorder &mapBorder);
    
    static TextureRegion getInsideBlockTextureRegion();
    
    static TextureRegion getNineTextureRegion();
    
    static TextureRegion getOneTextureRegion();

	static TextureRegion getBreakableBlockTextureRegion(BreakableBlock &breakableBlock);
    
    static TextureRegion getInterfaceOverlayTextureRegion();
    
    static TextureRegion getSpectatorInterfaceOverlayTextureRegion();
    
    static TextureRegion getPlayerAvatarTextureRegion(PlayerAvatar &playerAvatar);
    
    static TextureRegion getDPadControlTextureRegion(DPadControl &dpadControl);

	static TextureRegion getPowerUpTextureRegion(PowerUp &powerUp);
    
    static TextureRegion getPlayerTextureRegion(PlayerDynamicGameObject &player);
    
    static TextureRegion getBombTextureRegion(BombGameObject &bomb);
    
    static TextureRegion getFireTextureRegion(Fire &fire);
    
    static TextureRegion getPowerUpBarItemTextureRegion(PowerUpBarItem &powerUpBarItem, float powerUpBarItemsStateTime);
    
    static TextureRegion getActiveButtonTextureRegion(ActiveButton &activeButton, float buttonsStateTime);
    
    static TextureRegion getBombButtonTextureRegion(BombButton &bombButton, float buttonsStateTime);
    
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