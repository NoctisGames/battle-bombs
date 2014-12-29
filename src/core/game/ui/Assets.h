//
//  Assets.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battle_bombs__Assets__
#define __battle_bombs__Assets__

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

#include "ResourceConstants.h"
#include "PowerUpType.h"

#include <vector>

class TextureRegion;
class SpaceTile;
class MapBorder;
class BreakableBlock;
class PlayerDynamicGameObject;
class BombGameObject;
class RemoteBomb;
class Fire;
class FallingObjectShadow;
class FireBall;
class IceBall;
class IcePatch;
class InsideBlock;
class BreakableBlock;
class PowerUp;
class PlayerAvatar;
class DPadControl;
class PowerUpBarItem;
class ActiveButton;
class BombButton;
class DetonateButton;
class SpectatorControls;
class PlayerRowAvatar;
class PlayerRowPlatformAvatar;
class CountDownNumberGameObject;
class DisplayGameOverGameObject;
class DisplayXMovingGameObject;
class WaitingForLocalSettingsInterface;
class EnableBotButton;
class EnablePowerUpButton;
class StartButton;
class BaseTile;
class RegeneratingDoor;

class Assets
{
public:
	static Assets * getInstance();

	static TextureRegion& getWorldBackgroundTextureRegion();
    
    static TextureRegion& getSpaceTileTextureRegion(SpaceTile &spaceTile);

	static TextureRegion& getMapBorderTextureRegion(MapBorder &mapBorder);
    
    static TextureRegion& getInsideBlockTextureRegion(InsideBlock &insideBlock);
    
    static TextureRegion& getNineTextureRegion();
    
    static TextureRegion& getOneTextureRegion();

	static TextureRegion& getBreakableBlockTextureRegion(BreakableBlock &breakableBlock);
    
    static TextureRegion& getWaitingForServerInterfaceTextureRegion();
    
    static TextureRegion& getPlayerRowAvatarTextureRegion(PlayerRowAvatar &playerRowAvatar);
    
    static TextureRegion& getPlayerRowPlatformAvatarTextureRegion(PlayerRowPlatformAvatar &playerRowPlatformAvatar);

	static TextureRegion& getPowerUpTextureRegion(PowerUp &powerUp);
    
    static TextureRegion& getPlayerTextureRegion(PlayerDynamicGameObject &player);
    
    static TextureRegion& getPlayerNameBubbleTextureRegion();
    
    static TextureRegion& getPlayerPointerTextureRegion();
    
    static TextureRegion& getBombTextureRegion(BombGameObject &bomb);
    
    static TextureRegion& getRemoteBombTextureRegion(RemoteBomb &bomb);
    
    static TextureRegion& getFireTextureRegion(Fire &fire);
    
    static TextureRegion& getForceFieldTextureRegion(int forceFieldState, float forceFieldStateTime);
    
    static TextureRegion& getFallingObjectShadowTextureRegion(FallingObjectShadow &fallingObjectShadow);
    
    static TextureRegion& getCraterTextureRegion();
    
    static TextureRegion& getFireBallTextureRegion(FireBall &fireBall);
    
    static TextureRegion& getIceBallTextureRegion(IceBall &iceBall);
    
    static TextureRegion& getIcePatchTextureRegion(IcePatch &icePatch);
    
    static TextureRegion& getInterfaceOverlayTextureRegion();
    
    static TextureRegion& getPlayerAvatarTextureRegion(PlayerAvatar &playerAvatar);
    
    static TextureRegion& getPowerUpBarItemTextureRegion(PowerUpBarItem &powerUpBarItem, float powerUpBarItemsStateTime);
    
    static TextureRegion& getActiveButtonTextureRegion(ActiveButton &activeButton, float buttonsStateTime);
    
    static TextureRegion& getBombButtonTextureRegion(BombButton &bombButton, float buttonsStateTime);
    
    static TextureRegion& getDetonateButtonTextureRegion(DetonateButton &detonateButton);
    
    static TextureRegion& getSpectatorInterfaceOverlayTextureRegion();
    
    static TextureRegion& getDPadControlTextureRegion(DPadControl &dpadControl);
    
    static TextureRegion& getSpectatorControlsTextureRegion(SpectatorControls &spectatorControls);
    
    static TextureRegion& getCountDownNumberTextureRegion(CountDownNumberGameObject &countDownNumberGameObject);
    
    static TextureRegion& getDisplayXMovingTextureRegion(DisplayXMovingGameObject &displayXMovingGameObject);
    
    static TextureRegion& getDisplayGameOverTextureRegion(DisplayGameOverGameObject &displayGameOverGameObject);
    
    static TextureRegion& getLocalSettingsInterfaceTextureRegion(WaitingForLocalSettingsInterface &waitingForLocalSettingsInterface);
    
    static TextureRegion& getEnableBotButtonTextureRegion(EnableBotButton &enableBotButton);
    
    static TextureRegion& getBotDifficultyTextTextureRegion(EnableBotButton &enableBotButton);
    
    static TextureRegion& getEnablePowerUpButtonTextureRegion(EnablePowerUpButton &enablePowerUpButton);
    
    static TextureRegion& getStartButtonTextureRegion(StartButton &startButton);
    
    static TextureRegion& getBaseTileTextureRegion(BaseTile &baseTile);
    
    static TextureRegion& getRegeneratingDoorTextureRegion(RegeneratingDoor &regeneratingDoor);
    
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

#endif /* defined(__battle_bombs__Assets__) */