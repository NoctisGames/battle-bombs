package com.technegames.battlebombs;

import java.util.Locale;
import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.json.JSONObject;

import android.app.Activity;
import android.opengl.GLSurfaceView.Renderer;
import android.os.SystemClock;
import android.widget.Toast;

import com.shephertz.app42.gaming.multiplayer.client.WarpClient;
import com.technegames.battlebombs.platform.PlatformFileUtils;

public final class RendererWrapper implements Renderer
{
    private static final Logger logger = new Logger(RendererWrapper.class);
    private static final String KEEP_ALIVE = "KEEP_ALIVE,1";

    private static final String EVENT_TYPE = "eventType";
    private static final String EVENTS = "events";
    private static final String PLAYER_INDEX = "playerIndex";
    private static final String X = "X";
    private static final String Y = "Y";
    private static final String DIRECTION = "Direction";

    // Definitions from src/core/game/GameEvent.h
    private static final short CLIENT_UPDATE = 1338;
    private static final short SUDDEN_DEATH = 1339;
    private static final short GAME_OVER = 1340;
    private static final int PLAYER_EVENT_BASE = 100000000;
    private static final int PLAYER_EVENT_DIRECTION_BASE = 10000000;
    private static final int PLAYER_EVENT_GRID_X_BASE = 100000;
    private static final int PLAYER_EVENT_GRID_Y_BASE = 1000;
    private static final int PLAYER_EVENT_MOD_BASE = 100;
    private static final int PLAYER_DEATH = 11;
    private static final int PLAYER_ABOUT_TO_FALL = 12;
    private static final int PLAYER_FREEZE = 14;

    // Definitions from src/core/game/ResourceConstants.h
    private static final short MUSIC_STOP = 1;
    private static final short MUSIC_PLAY_MAP_SPACE = 2;
    private static final short MUSIC_PLAY_MAP_GRASSLANDS = 3;
    private static final short MUSIC_PLAY_MAP_MOUNTAINS = 4;
    private static final short MUSIC_PLAY_MAP_BASE = 5;
    private static final short SOUND_COUNT_DOWN_3 = 1;
    private static final short SOUND_COUNT_DOWN_2 = 2;
    private static final short SOUND_COUNT_DOWN_1 = 3;
    private static final short SOUND_BATTLE = 4;
    private static final short SOUND_PLANT_BOMB = 5;
    private static final short SOUND_EXPLOSION = 6;
    private static final short SOUND_PU_BOMB = 7;
    private static final short SOUND_PU_FIRE = 8;
    private static final short SOUND_PU_SPEED = 9;
    private static final short SOUND_PU_FORCE_FIELD = 10;
    private static final short SOUND_PU_PUSH = 11;
    private static final short SOUND_PU_SHIELD = 12;
    private static final short SOUND_FORCE_FIELD_DOWN = 13;
    private static final short SOUND_DEATH = 14;
    private static final short SOUND_HURRY = 15;
    private static final short SOUND_GAME_SET = 16;
    private static final short SOUND_DRAW = 17;
    private static final short SOUND_RAISE_SHIELD = 18;
    private static final short SOUND_DISLODGING_SPACE_TILE = 19;
    private static final short SOUND_FALLING_SPACE_TILE = 20;
    private static final short SOUND_FALLING_OBJECT = 21;
    private static final short SOUND_CRASHING_FIRE_BALL = 22;
    private static final short SOUND_CRASHING_ICE_BALL = 23;

    // Definitions from src/core/game/ScreenState.h
    private static final short SCREEN_STATE_NORMAL = 0;
    private static final short SCREEN_STATE_ENTERED_SPECTATOR_MODE = 1;
    private static final short SCREEN_STATE_CONNECTION_ERROR = 2;
    private static final short SCREEN_STATE_EXIT = 3;
    private static final short SCREEN_STATE_OFFLINE_MODE_NEXT_MAP = 4;

    // #frames involved in average calc (suggested values 5-100)
    private static final float movAveragePeriod = 40;

    // adjusting ratio (suggested values 0.01-0.5)
    private static final float smoothFactor = 0.1f;

    static
    {
        System.loadLibrary("game");
    }

    private final Activity activity;
    private final int deviceScreenWidth;
    private final int deviceScreenHeight;
    private final String username;
    private final Audio audio;
    private final Sound countDown3Sound;
    private final Sound countDown2Sound;
    private final Sound countDown1Sound;
    private final Sound battleSound;
    private final Sound plantBombSound;
    private final Sound explosionSound;
    private final Sound powerUpBombSound;
    private final Sound powerUpFireSound;
    private final Sound powerUpSpeedSound;
    private final Sound powerUpForceFieldSound;
    private final Sound powerUpPushSound;
    private final Sound powerUpShieldSound;
    private final Sound forceFieldDownSound;
    private final Sound deathSound;
    private final Sound hurrySound;
    private final Sound gameSetSound;
    private final Sound drawSound;
    private final Sound raiseShieldSound;
    private final Sound dislodgingSpaceTileSound;
    private final Sound fallingSpaceTileSound;
    private final Sound fallingObjectSound;
    private final Sound crashingFireBallSound;
    private final Sound crashingIceBallSound;
    private final boolean isOffline;

    private Music bgm;

    private float smoothedDeltaRealTime_ms = 17.5f;
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms;
    private long lastRealTimeMeasurement_ms;
    private boolean isInitialized;

    // For Offline Mode
    private static final String[] _beginGameMessages = new String[12];
    private boolean[] _playersAlive = { true, true, true, true, true, true, true, true };
    boolean _isSuddenDeath;
    private float _timeSinceOneOrLessPlayersRemaining;
    private boolean _isGameOver;

    public RendererWrapper(Activity activity, int deviceScreenWidth, int deviceScreenHeight, String username, boolean isOffline)
    {
        this.activity = activity;
        this.deviceScreenWidth = deviceScreenWidth;
        this.deviceScreenHeight = deviceScreenHeight;
        this.username = username;
        this.isOffline = isOffline;
        this.audio = new Audio(activity.getAssets());
        this.countDown3Sound = audio.newSound("countdown_3.ogg");
        this.countDown2Sound = audio.newSound("countdown_2.ogg");
        this.countDown1Sound = audio.newSound("countdown_1.ogg");
        this.battleSound = audio.newSound("battle.ogg");
        this.plantBombSound = audio.newSound("plant_bomb.ogg");
        this.explosionSound = audio.newSound("explosion.ogg");
        this.powerUpBombSound = audio.newSound("pu_bomb.ogg");
        this.powerUpFireSound = audio.newSound("pu_fire.ogg");
        this.powerUpSpeedSound = audio.newSound("pu_speed.ogg");
        this.powerUpForceFieldSound = audio.newSound("pu_force_field.ogg");
        this.powerUpPushSound = audio.newSound("pu_push.ogg");
        this.powerUpShieldSound = audio.newSound("pu_shield.ogg");
        this.forceFieldDownSound = audio.newSound("force_field_down.ogg");
        this.deathSound = audio.newSound("death.ogg");
        this.hurrySound = audio.newSound("hurry.ogg");
        this.gameSetSound = audio.newSound("game_set.ogg");
        this.drawSound = audio.newSound("draw.ogg");
        this.raiseShieldSound = audio.newSound("shield_raise.ogg");
        this.dislodgingSpaceTileSound = audio.newSound("dislodging_space_tile.ogg");
        this.fallingSpaceTileSound = audio.newSound("falling_space_tile.ogg");
        this.fallingObjectSound = audio.newSound("falling_object.ogg");
        this.crashingFireBallSound = audio.newSound("crashing_fire_ball.ogg");
        this.crashingIceBallSound = audio.newSound("crashing_ice_ball.ogg");

        this.isInitialized = false;

        init(username, isOffline);

        if (isOffline)
        {
            _beginGameMessages[0] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"3,0,0,0,5,0,0,0,0,4,1,0,2,0,5,0,0,6,5,5,1,3,4,6,0,0,0,0,0,3,3,3,0,0,0,4,2,1,0,0,0,4,0,1,3,0,2,0,1,0,6,0,1,0,1,6,2,4,1,0,6,1,0,3,0,4,0,0,5,4,3,0,0,3,0,0,0,2,2,0,0,0,0,0,2,4,1,1,0,0,0,5,1,0,0,0,0,5,0,1,0,0,0,3,0,6,5,0,4,0,0\", \"breakableBlockXValues\": \"8,5,6,8,10,3,9,11,1,2,5,6,7,8,9,10,11,13,2,4,6,8,14,0,1,2,3,4,6,7,8,9,10,11,12,13,14,2,4,6,8,12,2,3,4,6,7,8,9,12,2,6,10,12,2,4,6,8,10,12,3,5,6,7,8,9,10,2,4,6,8,10,0,1,2,3,4,5,6,8,9,11,12,13,14,2,8,10,0,1,2,3,6,11,14,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,12\", \"breakableBlockYValues\": \"0,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 111, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.537313461303711, \"playerIndex1\": \"Roy Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 16.8358211517334, \"playerIndex2\": \"Heather Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 16.8358211517334, \"playerIndex3\": \"Tyler Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.507462739944458, \"playerIndex4\": \"Andre Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Dave Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.537313461303711, \"playerIndex6\": \"Debra Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 17.91044807434082, \"playerIndex6Y\": 2.507462739944458, \"playerIndex7\": \"Stephen Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 25.432836532592773}", this.username);
            _beginGameMessages[1] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"2,2,1,1,0,0,3,0,1,5,0,0,2,0,3,0,0,0,2,3,0,4,0,3,2,0,2,2,0,0,1,0,3,1,2,1,0,4,0,2,0,0,0,6,3,3,3,2,2,2,2,0,3,5,3,2,0,6,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,2,4,0,3,2,0,1,0,2,0,4,3,0,2,0,3,4,0\", \"breakableBlockXValues\": \"8,5,6,7,8,10,5,9,11,1,2,3,4,6,9,12,13,14,0,2,4,6,8,10,12,14,1,2,3,4,5,6,7,9,10,11,12,2,4,8,3,4,7,10,2,4,6,12,6,10,5,6,7,8,9,10,11,12,4,6,8,1,2,3,4,7,8,10,11,13,14,4,6,8,10,0,1,2,3,7,8,9,10,11,14,4,6,8,10,12,3,6,7,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 96, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.537313461303711, \"playerIndex1\": \"Stephen Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.432836532592773, \"playerIndex2\": \"Barney Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.507462739944458, \"playerIndex3\": \"Kyle Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 12.537313461303711, \"playerIndex4\": \"Andre Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.507462739944458, \"playerIndex5\": \"Pier Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.8358211517334, \"playerIndex6\": \"Glen Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.432836532592773, \"playerIndex7\": \"Randy Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.8358211517334}", this.username);
            _beginGameMessages[2] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,0,2,2,0,2,0,4,0,2,0,0,0,0,2,3,5,5,0,0,2,0,0,1,0,2,1,0,1,3,2,5,0,6,0,0,0,0,2,3,0,1,0,4,6,1,0,0,0,1,0,1,0,0,1,3,6,3,0,4,0,0,0,4,0,0,2,0,0,0,5,5,6,0,2,0,5,0,0,1,1,2,0,0,2,0,2,2,0,0,3,0,0,1,3,5,3,0\", \"breakableBlockXValues\": \"6,8,4,7,8,9,10,3,5,7,9,11,0,1,3,6,7,11,12,14,0,2,4,6,12,14,0,1,2,4,6,8,10,11,13,2,8,10,12,3,4,5,7,10,11,6,8,12,2,8,10,5,6,7,10,11,12,4,10,0,1,2,4,5,6,7,8,9,10,11,12,13,2,4,8,10,12,14,0,1,2,3,7,8,9,10,13,14,2,4,5,7,8,12,2,3,5,9,10\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,8,8,8,9,9,9,10,10,10,10,10,10,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 99, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 16.8358211517334, \"playerIndex1\": \"Brandi Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.537313461303711, \"playerIndex2\": \"Bradlee Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 16.8358211517334, \"playerIndex3\": \"Heather Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.537313461303711, \"playerIndex4\": \"Matt Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Glen Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 6.447761058807373, \"playerIndex5Y\": 2.507462739944458, \"playerIndex6\": \"Barney Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 17.91044807434082, \"playerIndex6Y\": 2.507462739944458, \"playerIndex7\": \"Dean Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 25.432836532592773}", this.username);
            _beginGameMessages[3] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,1,0,0,1,1,3,2,3,5,2,1,4,0,3,0,2,2,4,0,0,3,0,0,0,0,5,0,1,0,0,3,0,1,3,0,3,0,2,2,3,3,2,0,1,0,0,1,3,0,0,3,0,0,0,0,0,0,2,3,2,4,0,0,0,0,6,2,0,4,0,1,5,6,5,1,0,3,0,2,0,5,3,5,2,0,0,1,4,0,2,0,0,0,0,1\", \"breakableBlockXValues\": \"6,8,4,5,6,7,8,9,10,3,5,7,11,0,1,2,3,4,5,6,7,9,11,13,12,2,4,5,6,8,9,10,12,6,10,2,3,4,6,7,8,10,11,2,6,8,12,2,4,10,12,2,3,4,7,9,10,2,8,10,12,0,10,11,12,13,14,0,2,4,8,10,12,14,1,2,3,5,6,7,9,10,11,12,13,4,6,8,12,2,3,5,7,8,10,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,4,5,5,5,5,5,5,5,5,6,6,7,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 96, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 6.447761058807373, \"playerIndex0Y\": 2.507462739944458, \"playerIndex1\": \"Kenny Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.432836532592773, \"playerIndex2\": \"Wesley Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 12.537313461303711, \"playerIndex3\": \"Tyler Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 17.91044807434082, \"playerIndex3Y\": 2.507462739944458, \"playerIndex4\": \"Morgan Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 16.8358211517334, \"playerIndex5\": \"Paul Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.537313461303711, \"playerIndex6\": \"Lily Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.432836532592773, \"playerIndex7\": \"Bruce Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 16.8358211517334}", this.username);
            _beginGameMessages[4] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"4,0,5,0,0,6,0,2,0,3,3,0,2,3,3,2,0,5,0,3,0,0,5,5,3,0,0,4,2,2,0,0,0,0,2,3,0,0,0,3,0,3,0,0,0,0,0,2,1,0,1,2,0,0,2,4,0,0,0,0,5,0,0,3,0,2,5,0,4,3,5,0,0,0,0,0,0,6,2,0,3,1,0,3,3,0,0,0,0,0,6,0,0,0,5,1,2\", \"breakableBlockXValues\": \"6,4,6,9,7,9,11,0,1,2,3,7,11,14,0,4,8,12,14,0,1,2,3,5,6,8,10,11,14,2,4,6,8,12,2,5,6,8,9,10,11,12,2,4,10,2,4,6,10,2,6,9,10,11,2,4,6,8,12,0,1,2,3,5,6,7,8,10,12,13,2,4,8,10,12,1,2,3,5,6,7,8,11,12,13,14,4,6,8,10,12,3,4,5,7,8,10\", \"breakableBlockYValues\": \"0,1,1,1,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,9,9,9,9,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 97, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.8358211517334, \"playerIndex1\": \"Tyler Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.537313461303711, \"playerIndex2\": \"Joe Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.507462739944458, \"playerIndex3\": \"Lily Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.432836532592773, \"playerIndex4\": \"Brandi Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Kathy Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 6.447761058807373, \"playerIndex5Y\": 2.507462739944458, \"playerIndex6\": \"Dexter Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 16.8358211517334, \"playerIndex7\": \"Paul Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 12.537313461303711}", this.username);
            _beginGameMessages[5] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,0,2,2,4,0,3,1,5,4,0,0,0,0,6,6,0,3,0,0,2,1,4,0,0,3,1,3,0,1,1,3,5,0,0,0,1,2,0,1,6,3,0,5,0,0,0,0,0,1,0,0,3,3,3,1,6,0,0,0,0,4,4,2,0,0,0,0,0,0,0,0,0,0,1,0,6,3,3,0,3,3,0,1,6,2,0,0,3,5,0,0,0,2,1,2,1,1,3,0,5,1,0,5,0,3,0,0,0\", \"breakableBlockXValues\": \"6,8,4,5,6,7,9,5,7,0,3,4,5,7,9,10,11,12,13,14,0,4,6,8,10,12,14,1,4,5,6,9,11,12,13,14,2,4,6,8,10,3,4,6,7,8,9,11,4,6,10,12,2,4,6,8,10,2,3,4,6,7,8,9,10,11,12,4,6,8,10,12,0,3,4,5,6,7,9,10,11,12,13,14,0,2,4,6,10,12,14,1,2,3,7,8,10,12,13,14,2,5,6,12,3,4,5,10,11,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 110, \"numClientBots\": 7,\"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 6.447761058807373, \"playerIndex0Y\": 2.507462739944458, \"playerIndex1\": \"Eric Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 25.432836532592773, \"playerIndex2\": \"Frank Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.507462739944458, \"playerIndex3\": \"James Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 12.537313461303711, \"playerIndex4\": \"Ryan Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.432836532592773, \"playerIndex5\": \"Roy Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 12.537313461303711, \"playerIndex6\": \"Alfred Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 16.8358211517334, \"playerIndex7\": \"Ted Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.8358211517334}", this.username);

            handleScreenStateOfflineModeNextMap();
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        logger.debug("GL Surface created!");

        if (!isInitialized)
        {
            PlatformFileUtils.init_asset_manager(activity.getAssets());
            isInitialized = true;
        }

        on_surface_created(deviceScreenWidth, deviceScreenHeight);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        logger.debug("GL Surface changed!");

        on_surface_changed(width, height, width, height);
        on_resume();
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        int screenState = get_state();
        switch (screenState)
        {
            case SCREEN_STATE_ENTERED_SPECTATOR_MODE:
                displayToastOnUiThread(activity.getString(R.string.you_are_in_spectator_mode));
                clear_state();
            case SCREEN_STATE_NORMAL:
                update(smoothedDeltaRealTime_ms / 1000);
                pushEvents();
                if (isOffline && _isGameOver)
                {
                    _timeSinceOneOrLessPlayersRemaining += (smoothedDeltaRealTime_ms / 1000);
                    if (_timeSinceOneOrLessPlayersRemaining > 0.5f)
                    {
                        _isGameOver = false;
                        _timeSinceOneOrLessPlayersRemaining = 0;
                        handleGameOver();
                    }
                }
                break;
            case SCREEN_STATE_CONNECTION_ERROR:
                activity.setResult(GameActivity.RESULT_CONNECTION_ERROR);
            case SCREEN_STATE_EXIT:
                activity.finish();
                break;
            case SCREEN_STATE_OFFLINE_MODE_NEXT_MAP:
                handleScreenStateOfflineModeNextMap();
                break;
            default:
                break;
        }

        present();
        handleSound();
        handleMusic();

        // Moving average calc
        long currTimePick_ms = SystemClock.uptimeMillis();
        float realTimeElapsed_ms;
        if (lastRealTimeMeasurement_ms > 0)
        {
            realTimeElapsed_ms = (currTimePick_ms - lastRealTimeMeasurement_ms);
        }
        else
        {
            realTimeElapsed_ms = smoothedDeltaRealTime_ms; // just the first
                                                           // time
        }

        movAverageDeltaTime_ms = (realTimeElapsed_ms + movAverageDeltaTime_ms * (movAveragePeriod - 1)) / movAveragePeriod;

        // Calc a better aproximation for smooth stepTime
        smoothedDeltaRealTime_ms = smoothedDeltaRealTime_ms + (movAverageDeltaTime_ms - smoothedDeltaRealTime_ms) * smoothFactor;

        lastRealTimeMeasurement_ms = currTimePick_ms;
    }

    public void onResume()
    {
        on_resume();
    }

    public void onPause()
    {
        if (bgm != null)
        {
            bgm.stop();
        }

        on_pause();
    }

    public void handleTouchDown(float rawX, float rawY)
    {
        on_touch_down(rawX, rawY);
    }

    public void handleTouchDragged(float rawX, float rawY)
    {
        on_touch_dragged(rawX, rawY);
    }

    public void handleTouchUp(float rawX, float rawY)
    {
        on_touch_up(rawX, rawY);
    }

    public boolean handleOnBackPressed()
    {
        return handle_on_back_pressed();
    }

    public void onChatReceived(String message)
    {
        logger.debug("onChatReceived: " + message);
        on_chat_received(message);
    }

    private void pushEvents()
    {
        if (isOffline)
        {
            int event;
            while ((event = get_oldest_event_id()) > 0)
            {
                int playerIndex = 0;
                while (event >= PLAYER_EVENT_BASE)
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
                        handleDeathForPlayerIndex(playerIndex);
                        continue;
                    default:
                        continue;
                }
            }

            if (get_num_seconds_left() <= 0)
            {
                String gameOverMessage = String.format(Locale.US, "{\"eventType\": %d, \"hasWinner\": false, \"winningPlayerIndex\": %d}", GAME_OVER, -1);

                on_chat_received(gameOverMessage);
            }
            else if (!_isSuddenDeath && get_num_seconds_left() <= 60)
            {
                String gameOverMessage = String.format(Locale.US, "{\"eventType\": %d}", SUDDEN_DEATH);

                on_chat_received(gameOverMessage);

                _isSuddenDeath = true;
            }
        }
        else
        {
            try
            {
                int eventId = get_oldest_event_id();

                if (eventId > 0)
                {
                    String eventsMessage = eventId + ",";
                    while ((eventId = get_oldest_event_id()) > 0)
                    {
                        eventsMessage += eventId + ",";
                    }

                    eventsMessage += "0"; // Terminate with 0

                    JSONObject tobeSent = new JSONObject();
                    tobeSent.put(EVENT_TYPE, CLIENT_UPDATE);
                    tobeSent.put(EVENTS, eventsMessage);
                    tobeSent.put(PLAYER_INDEX + get_player_index() + X, get_player_x());
                    tobeSent.put(PLAYER_INDEX + get_player_index() + Y, get_player_y());
                    tobeSent.put(PLAYER_INDEX + get_player_index() + DIRECTION, get_player_direction());

                    reset_time_since_last_client_event();

                    logger.debug("tobeSent.toString(): " + tobeSent.toString());

                    WarpClient.getInstance().sendChat(tobeSent.toString());
                }
                else if (is_time_to_send_keep_alive())
                {
                    reset_time_since_last_client_event();

                    WarpClient.getInstance().sendChat(KEEP_ALIVE);
                }
            }
            catch (Exception e)
            {
                logger.error(e.toString(), e);
            }
        }
    }

    private void handleScreenStateOfflineModeNextMap()
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

        _timeSinceOneOrLessPlayersRemaining = 0;
        _isGameOver = false;

        clear_state();

        final Random rand = new Random();

        int beginGameMessagesIndex = rand.nextInt(6);

        String beginGameMessage = _beginGameMessages[beginGameMessagesIndex];

        on_chat_received(beginGameMessage);
    }

    private void handleDeathForPlayerIndex(int playerIndex)
    {
        _playersAlive[playerIndex] = false;

        int numAlive = 0;
        for (int i = 0; i < 8; i++)
        {
            if (_playersAlive[i])
            {
                numAlive++;
            }
        }

        if (numAlive <= 1)
        {
            _isGameOver = true;
        }
    }

    private void handleGameOver()
    {
        int numAlive = 0;
        int winningPlayerIndex = -1;
        for (int i = 0; i < 8; i++)
        {
            if (_playersAlive[i])
            {
                winningPlayerIndex = i;
                numAlive++;
            }
        }

        if (numAlive <= 1)
        {
            boolean hasWinner = numAlive == 1;

            String gameOverMessage;
            if (hasWinner)
            {
                gameOverMessage = String.format(Locale.US, "{\"eventType\": %d, \"hasWinner\": true, \"winningPlayerIndex\": %d}", GAME_OVER, winningPlayerIndex);
            }
            else
            {
                gameOverMessage = String.format(Locale.US, "{\"eventType\": %d, \"hasWinner\": false, \"winningPlayerIndex\": %d}", GAME_OVER, winningPlayerIndex);
            }

            on_chat_received(gameOverMessage);
        }
    }

    private void handleSound()
    {
        short soundId;
        while ((soundId = get_current_sound_id()) > 0)
        {
            switch (soundId)
            {
                case SOUND_COUNT_DOWN_3:
                    countDown3Sound.play(1);
                    break;
                case SOUND_COUNT_DOWN_2:
                    countDown2Sound.play(1);
                    break;
                case SOUND_COUNT_DOWN_1:
                    countDown1Sound.play(1);
                    break;
                case SOUND_BATTLE:
                    battleSound.play(1);
                    break;
                case SOUND_PLANT_BOMB:
                    plantBombSound.play(1);
                    break;
                case SOUND_EXPLOSION:
                    explosionSound.play(1);
                    break;
                case SOUND_PU_BOMB:
                    powerUpBombSound.play(1);
                    break;
                case SOUND_PU_FIRE:
                    powerUpFireSound.play(1);
                    break;
                case SOUND_PU_SPEED:
                    powerUpSpeedSound.play(1);
                    break;
                case SOUND_PU_FORCE_FIELD:
                    powerUpForceFieldSound.play(1);
                    break;
                case SOUND_PU_PUSH:
                    powerUpPushSound.play(1);
                    break;
                case SOUND_PU_SHIELD:
                    powerUpShieldSound.play(1);
                    break;
                case SOUND_FORCE_FIELD_DOWN:
                    forceFieldDownSound.play(1);
                    break;
                case SOUND_DEATH:
                    deathSound.play(1);
                    break;
                case SOUND_HURRY:
                    hurrySound.play(1);
                    break;
                case SOUND_GAME_SET:
                    gameSetSound.play(1);
                    break;
                case SOUND_DRAW:
                    drawSound.play(1);
                    break;
                case SOUND_RAISE_SHIELD:
                    raiseShieldSound.play(1);
                    break;
                case SOUND_DISLODGING_SPACE_TILE:
                    dislodgingSpaceTileSound.play(1);
                    break;
                case SOUND_FALLING_SPACE_TILE:
                    fallingSpaceTileSound.play(1);
                    break;
                case SOUND_FALLING_OBJECT:
                    fallingObjectSound.play(1);
                    break;
                case SOUND_CRASHING_FIRE_BALL:
                    crashingFireBallSound.play(1);
                    break;
                case SOUND_CRASHING_ICE_BALL:
                    crashingIceBallSound.play(1);
                    break;
                default:
                    continue;
            }
        }
    }

    private void handleMusic()
    {
        short musicId = get_current_music_id();
        switch (musicId)
        {
            case MUSIC_STOP:
                if (bgm != null)
                {
                    bgm.stop();
                }
                break;
            case MUSIC_PLAY_MAP_SPACE:
                loadAndPlayMusic("map_space.ogg");
                break;
            case MUSIC_PLAY_MAP_GRASSLANDS:
                loadAndPlayMusic("map_grasslands.ogg");
                break;
            case MUSIC_PLAY_MAP_MOUNTAINS:
                loadAndPlayMusic("map_mountains.ogg");
                break;
            case MUSIC_PLAY_MAP_BASE:
                loadAndPlayMusic("map_base.ogg");
                break;
            default:
                break;
        }
    }

    private void loadAndPlayMusic(String fileName)
    {
        if (bgm != null && bgm.isPlaying())
        {
            bgm.dispose();
            bgm = null;
        }

        bgm = audio.newMusic(fileName);
        bgm.setLooping(true);
        bgm.play();
    }

    private void displayToastOnUiThread(final String toast)
    {
        activity.runOnUiThread(new Runnable()
        {
            public void run()
            {
                Toast.makeText(activity, toast, Toast.LENGTH_SHORT).show();
            }
        });
    }

    private static native void init(String username, boolean isOffline);

    private static native void on_surface_created(int pixelWidth, int pixelHeight);

    private static native void on_surface_changed(int pixelWidth, int pixelHeight, int dpWidth, int dpHeight);

    private static native void on_resume();

    private static native void on_pause();

    private static native void update(float deltaTime);

    private static native void present();

    private static native void on_chat_received(String message);

    private static native void on_touch_down(float normalized_x, float normalized_y);

    private static native void on_touch_dragged(float normalized_x, float normalized_y);

    private static native void on_touch_up(float normalized_x, float normalized_y);

    private static native short get_current_music_id();

    private static native short get_current_sound_id();

    private static native int get_oldest_event_id();

    private static native boolean is_time_to_send_keep_alive();

    private static native void reset_time_since_last_client_event();

    private static native int get_state();

    private static native void clear_state();

    private static native short get_player_index();

    private static native float get_player_x();

    private static native float get_player_y();

    private static native int get_player_direction();

    private static native boolean handle_on_back_pressed();

    private static native int get_num_seconds_left();
}