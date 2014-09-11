package com.technegames.battlebombs;

import java.util.Locale;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.json.JSONObject;

import android.app.Activity;
import android.opengl.GLSurfaceView.Renderer;
import android.os.SystemClock;

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

    // Definitions from src/core/GameEvent.h
    private static final short CLIENT_UPDATE = 1338;
    private static final short GAME_OVER = 1339;
    private static final int PLAYER_EVENT_BASE = 100000000;
    private static final int PLAYER_EVENT_DIRECTION_BASE = 10000000;
    private static final int PLAYER_EVENT_GRID_X_BASE = 100000;
    private static final int PLAYER_EVENT_GRID_Y_BASE = 1000;
    private static final int PLAYER_EVENT_MOD_BASE = 100;
    private static final short PLAYER_DEATH = 9;

    // Definitions from src/core/ResourceConstants.h
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
    private static final short SOUND_FORCE_FIELD_DOWN = 12;
    private static final short SOUND_DEATH = 13;
    private static final short SOUND_GAME_SET = 14;
    private static final short SOUND_DRAW = 15;

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
    private final Sound forceFieldDownSound;
    private final Sound deathSound;
    private final Sound gameSetSound;
    private final Sound drawSound;
    private final boolean isOffline;

    private Music bgm;

    private float smoothedDeltaRealTime_ms = 17.5f;
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms;
    private long lastRealTimeMeasurement_ms;
    private boolean isInitialized;

    // For Offline Mode
    private static int _beginGameMessagesIndex = 0;
    private static final String[] _beginGameMessages = new String[12];
    private boolean[] _playersAlive = { true, true, true, true, true, true, true, true };

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
        this.forceFieldDownSound = audio.newSound("force_field_down.ogg");
        this.deathSound = audio.newSound("death.ogg");
        this.gameSetSound = audio.newSound("game_set.ogg");
        this.drawSound = audio.newSound("draw.ogg");

        this.isInitialized = false;

        init(username, isOffline);

        if (isOffline)
        {
            _beginGameMessages[0] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"3,3,1,5,3,0,1,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,1,5,0,0,0,0,3,0,4,0,0,0,0,3,0,0,5,3,4,0,0,0,0,0,0,2,0,0,4,0,1,0,3,2,4,0,0,0,1,2,0,4,0,0,0,0,1,2,0,0,0,0,0,3,4,0,0,0,0,0,0,0,1,3,0,0,3,0,0,0,1,0,2,0,0,0,0,0,0\", \"breakableBlockXValues\": \"6,8,4,5,7,9,10,5,7,11,0,2,3,4,5,6,7,8,10,13,14,2,4,8,10,12,2,3,4,5,6,7,9,10,11,12,14,6,10,4,5,8,9,11,2,6,8,10,2,4,6,8,12,2,4,7,8,9,10,12,2,8,10,12,0,1,2,4,5,6,7,8,9,10,12,14,0,2,4,6,12,14,2,3,4,6,7,9,11,12,14,2,4,6,8,10,12,3,4,5,6,8,9,11\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 104, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Norman Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Jay Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 12.179104804992676, \"playerIndex3\": \"Dave Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Mikel Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Don Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Brandi Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 6.447761058807373, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Kathy Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}", this.username);
            _beginGameMessages[1] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,5,0,4,0,0,4,0,0,0,0,0,5,4,3,0,2,0,1,0,0,0,3,0,0,5,1,0,0,0,0,1,0,0,0,0,0,0,0,3,0,1,0,0,4,2,0,0,0,0,0,0,0,2,1,0,0,4,0,0,3,0,0,0,0,0,3,0,0,0,0,0,0,2,2,0,0,4,0,0,2,0,2,0,0,0,0,0,4,0,4,3,1,0,0,5,0,3,0,0,0,0,3,0,1,0\", \"breakableBlockXValues\": \"6,4,6,8,9,10,3,5,7,9,11,0,2,4,6,7,9,11,13,0,2,4,10,14,0,1,2,5,6,7,8,9,11,12,4,6,8,12,2,3,5,6,9,10,11,12,2,4,6,10,12,2,4,6,10,2,4,5,6,7,10,11,12,8,10,12,0,1,3,5,6,9,11,12,14,2,4,6,8,10,12,14,2,5,6,7,9,10,11,13,14,2,6,8,10,12,2,3,4,5,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 106, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Norman Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 17.91044807434082, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Kenny Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Adam Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Don Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Dexter Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Tyler Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 12.179104804992676, \"playerIndex7\": \"Peter Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 6.447761058807373, \"playerIndex7Y\": 2.1492538452148438}", this.username);
            _beginGameMessages[2] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,3,0,3,0,0,0,4,0,0,2,0,4,5,0,5,0,0,0,0,3,0,0,0,1,0,3,0,3,2,0,0,5,4,0,1,0,0,0,1,0,0,0,0,2,0,0,3,4,1,0,1,0,2,0,0,0,0,4,1,2,0,4,3,1,3,0,0,0,3,0,1,0,0,3,0,0,0,4,2,5,0,0,0,0,0,0,0,0,1,0,4,0,0,1,0,4,2,0,0,3,1,3,0,0,2,0,5\", \"breakableBlockXValues\": \"8,4,5,7,8,9,3,5,7,9,11,0,2,3,5,6,7,8,9,14,0,2,4,8,10,14,0,2,3,4,6,7,8,9,10,11,2,4,6,8,10,12,3,4,5,7,9,10,11,2,4,8,10,12,4,8,10,12,2,5,6,7,8,9,11,12,2,4,6,8,10,12,0,1,2,4,6,7,8,2,4,10,12,1,2,3,5,6,7,9,10,11,12,13,14,2,4,5,6,7,10,12,2,3,4,5,9,10,11\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 109, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Bobby Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 17.91044807434082, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Aaron Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 12.179104804992676, \"playerIndex3\": \"Matt Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Don Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Kenny Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Dylan Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 6.447761058807373, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Dan Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}", this.username);
            _beginGameMessages[3] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,0,0,5,0,2,3,0,2,2,2,0,3,0,0,5,0,4,2,3,2,0,0,2,0,3,0,3,2,0,0,0,4,2,0,0,2,0,0,0,0,0,3,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,4,2,0,0,2,0,0,2,1,5,2,0,0,0,0,0,0,4,3,5,0,0,0,0,0,0,0,2,0,0,3,0,0,3,3,0,0,0,0,0,5,5,0,0,0,0,0,0,4,0,1\", \"breakableBlockXValues\": \"6,4,5,7,8,9,10,3,7,2,3,6,7,8,9,11,12,13,0,2,4,6,8,10,12,14,1,4,7,8,12,13,14,2,6,8,10,12,2,3,4,6,8,9,10,12,2,4,6,8,10,12,2,6,10,12,2,3,4,5,6,7,8,10,11,2,4,6,8,10,12,1,3,4,5,6,7,8,9,11,13,14,0,4,6,8,10,12,14,0,2,3,4,5,7,8,9,12,13,2,4,6,8,12,3,4,6,7,8,9,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 112, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 17.91044807434082, \"playerIndex0Y\": 2.1492538452148438, \"playerIndex1\": \"Ryan Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.074626922607422, \"playerIndex2\": \"Wesley Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Dean Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Kurt Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 16.477611541748047, \"playerIndex5\": \"Jay Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Kyle Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 16.477611541748047, \"playerIndex7\": \"Peter Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 12.179104804992676}", this.username);
            _beginGameMessages[4] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,3,2,0,0,0,2,3,0,0,0,0,0,5,2,1,0,5,4,1,1,2,0,2,1,0,5,5,0,0,0,4,2,0,0,0,0,0,0,4,0,0,4,4,5,0,0,0,0,0,2,0,2,0,0,3,0,0,2,4,0,1,0,1,1,0,4,0,0,0,5,0,0,0,0,0,0,0,0,2,0,0,5,0,5,0,1,2,0,0,0,0,1,0,0,0,0,0,0,3,1,0,0,0,0,3,5,0,0,1,0,1,0,2\", \"breakableBlockXValues\": \"8,6,7,10,3,5,9,11,0,1,2,3,4,5,6,7,8,11,13,14,2,4,10,12,0,1,2,4,6,7,8,9,10,11,12,14,2,4,6,8,12,4,5,6,7,8,9,2,4,8,12,2,4,8,10,2,3,4,5,7,8,10,11,12,2,4,6,8,10,0,1,2,3,4,5,6,8,10,11,13,14,0,4,6,8,10,12,14,0,1,2,3,4,5,6,7,8,11,12,13,14,2,4,6,8,3,4,5,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 114, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Heather Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Adam Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Morgan Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 16.477611541748047, \"playerIndex4\": \"Mikel Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Jay Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 17.91044807434082, \"playerIndex5Y\": 2.1492538452148438, \"playerIndex6\": \"Ryan Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Dick Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 6.447761058807373, \"playerIndex7Y\": 2.1492538452148438}", this.username);
            _beginGameMessages[5] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,1,0,0,0,0,0,3,0,2,0,1,2,0,2,0,0,0,0,2,5,0,0,0,0,0,0,5,2,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,4,4,5,0,4,1,0,2,0,0,0,0,0,0,0,5,0,0,0,2,0,0,0,0,5,0,0,3,0,0,4,0,3,2,0,3,3,4,1,2,4,0,0,0,3,4,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,3,0,0,0,3,0\", \"breakableBlockXValues\": \"6,8,4,5,6,7,8,9,10,3,5,7,9,11,0,2,3,6,7,8,9,10,11,12,14,0,2,6,8,10,14,0,2,3,4,6,7,8,9,10,11,12,13,14,2,6,8,12,3,5,6,7,8,10,11,2,6,8,12,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,11,4,8,10,0,1,2,3,4,5,6,9,13,14,0,2,4,6,8,10,14,0,1,2,4,5,6,7,8,9,10,12,14,2,4,5,6,7,8,12,4,5,10,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 118, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 17.91044807434082, \"playerIndex0Y\": 2.1492538452148438, \"playerIndex1\": \"Dean Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 16.477611541748047, \"playerIndex2\": \"Bradlee Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Ryan Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Jason Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 16.477611541748047, \"playerIndex5\": \"Aaron Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Marshall Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Travis Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 25.074626922607422}", this.username);
            _beginGameMessages[6] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,0,0,0,0,1,0,1,3,3,0,5,0,0,3,0,0,0,0,0,0,3,0,0,2,4,0,0,1,0,5,1,0,0,0,0,0,0,0,3,0,1,1,0,0,0,0,1,0,0,0,1,0,3,2,1,2,0,4,0,5,0,5,0,0,1,4,0,0,4,0,0,2,0,0,2,0,0,2,2,2,1,0,0,3,2,0,0,0,0,4,3,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,3,0,4\", \"breakableBlockXValues\": \"6,8,5,7,8,9,10,3,5,7,9,11,0,2,3,4,5,6,7,9,10,11,12,13,0,4,8,10,12,1,2,6,7,8,9,10,14,2,4,6,8,12,3,4,5,6,7,8,9,11,12,2,4,8,10,12,2,4,6,2,3,4,6,7,8,9,10,11,12,2,4,6,8,10,12,1,2,6,8,9,10,11,14,2,6,8,10,12,1,3,4,5,6,7,8,9,10,11,12,13,14,2,4,6,10,12,3,6,9,10,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 111, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Roy Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.074626922607422, \"playerIndex2\": \"Bobby Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Todd Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Debra Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 6.447761058807373, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Mariel Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Kenny Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 17.91044807434082, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Dante Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 12.179104804992676}", this.username);
            _beginGameMessages[7] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,0,3,3,0,0,0,0,0,0,4,2,0,0,0,0,4,0,0,2,0,2,0,2,0,3,0,0,4,2,0,1,0,2,0,0,1,1,1,0,0,3,0,4,0,0,0,1,0,3,0,3,0,5,2,0,0,0,5,0,0,0,0,2,2,0,1,3,0,0,0,0,0,0,0,2,0,0,0,0,5,0,0,0,4,0,0,2,4,3,0,0,0,0,3,5,0,2,0,0,0,3,0,0\", \"breakableBlockXValues\": \"8,5,6,8,9,10,5,7,0,1,3,5,6,7,8,11,14,0,2,4,6,8,10,12,1,2,3,4,5,6,7,8,10,11,14,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,11,12,4,10,2,6,12,2,3,4,6,7,8,9,12,4,6,8,10,0,1,2,4,7,9,10,11,12,13,14,0,2,4,6,8,14,2,4,5,9,10,12,2,8,10,2,3,4,6,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,8,8,9,9,9,10,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 105, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Bill Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 25.074626922607422, \"playerIndex2\": \"Stephen Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 25.074626922607422, \"playerIndex3\": \"Morgan Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 16.477611541748047, \"playerIndex4\": \"Kurt Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Robin Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Sam Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 6.447761058807373, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Tyler Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 17.91044807434082, \"playerIndex7Y\": 2.1492538452148438}", this.username);
            _beginGameMessages[8] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,0,4,0,4,0,0,0,4,0,5,4,1,0,1,0,1,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,3,2,0,2,3,0,0,1,0,0,1,0,5,0,0,0,0,0,1,0,0,0,3,0,1,0,0,2,0,0,0,0,3,0,2,0,4,1,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,5,2,5,0,1,0,4,1,0,5,0,0,5,5,0,0,0,0\", \"breakableBlockXValues\": \"6,8,4,5,7,9,10,3,5,7,9,11,0,2,3,4,10,11,13,0,4,10,14,1,5,6,7,8,9,12,13,14,2,8,10,2,3,5,6,7,9,10,11,12,2,4,6,10,8,2,3,4,5,6,7,8,10,11,12,4,6,0,2,3,4,6,7,8,10,12,13,14,0,4,6,10,12,1,2,3,4,5,8,9,10,11,12,13,14,2,4,5,7,8,9,10,12,2,3,5,10,11,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,9,10,10,10,10,10,10,10,10,10,10,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 103, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Matt Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 17.91044807434082, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Stan Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Wesley Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Jason Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 16.477611541748047, \"playerIndex5\": \"Bruce Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Pier Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Don Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 25.074626922607422}", this.username);
            _beginGameMessages[9] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,1,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,2,0,0,2,0,0,0,0,0,3,2,0,0,0,0,5,4,5,4,0,2,4,0,0,0,1,0,0,2,0,2,0,0,3,0,3,3,0,5,0,3,0,0,0,3,2,0,4,0,0,0,3,0,3,2,5,0,1,0,0,2,0,1,0,3,0,0,4,2,0,0,3,2,0,2,0,0,0,2,0,1\", \"breakableBlockXValues\": \"6,8,4,5,6,3,7,0,2,3,6,7,8,9,11,12,13,4,6,8,10,12,0,1,2,4,5,6,7,9,11,12,4,6,10,2,4,5,9,11,12,2,4,6,2,4,6,10,2,4,5,6,7,9,2,4,6,8,10,0,3,4,5,6,7,8,11,13,14,0,2,4,6,8,10,12,14,0,1,3,4,5,6,8,9,10,11,12,13,6,8,10,2,3,5,6,7,11\", \"breakableBlockYValues\": \"0,0,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,7,7,7,7,7,7,8,8,8,9,9,9,9,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 98, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Todd Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 16.477611541748047, \"playerIndex2\": \"Kathy Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"James Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Kenny Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Mariel Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Sam Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Mary Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 17.91044807434082, \"playerIndex7Y\": 2.1492538452148438}", this.username);
            _beginGameMessages[10] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"5,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,3,4,1,5,0,3,1,5,0,0,1,1,3,0,0,0,3,3,0,0,4,0,0,0,0,3,0,0,5,1,0,0,3,0,0,0,1,1,2,0,0,0,4,0,2,4,1,0,0,0,4,0,0,0,0,0,3,0,0,5,0,0,2,0,0,3,0,0,0,3,0,3,0,0,1,0,0,0,0,0,0,0,0,2,0,3,0,2,0,0,0,0,1\", \"breakableBlockXValues\": \"6,8,5,6,8,9,10,3,5,0,1,2,4,5,7,8,11,13,14,2,10,14,0,3,4,7,8,9,10,12,13,14,2,4,6,8,10,12,2,4,6,7,8,10,11,6,8,10,12,2,4,6,8,10,2,3,5,6,7,8,10,11,12,2,4,6,8,10,12,0,2,5,7,8,9,11,13,14,4,6,8,10,14,1,2,3,4,5,6,7,9,10,11,13,2,4,6,8,10,12,2,5,6,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 109, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Morgan Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Dick Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Mike Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Joe Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Brandi Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 25.074626922607422, \"playerIndex6\": \"Jay Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 16.477611541748047, \"playerIndex7\": \"Roy Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 12.179104804992676}", this.username);
            _beginGameMessages[11] = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"2,3,1,0,0,0,0,1,5,2,4,0,0,0,0,3,0,0,0,2,0,3,0,0,0,4,1,0,4,0,0,1,0,3,0,0,0,2,0,0,0,3,0,4,2,0,0,3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,5,2,0,0,0,1,5,0,1,2,0,0,0,4,0,0,0,0,0,3,4,1,0,5,0,0,0,5,3,5,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,0,3\", \"breakableBlockXValues\": \"8,5,6,7,8,9,10,3,7,9,11,1,2,3,5,6,7,8,9,10,11,13,14,0,2,4,6,12,14,0,1,2,4,5,6,7,9,10,11,12,13,14,4,6,8,10,12,2,3,4,5,7,8,12,2,4,8,10,12,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,12,2,4,0,3,4,6,8,9,13,14,2,4,6,10,12,14,2,3,4,5,7,8,9,11,12,13,2,4,6,7,8,9,10,12,4,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 114, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Ruby Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 6.447761058807373, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Stephen Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 12.179104804992676, \"playerIndex3\": \"Joe Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 17.91044807434082, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Wesley Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Eric Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Jay Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Kenny Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 25.074626922607422}", this.username);

            handleGameStateOne();
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
        int gameState = get_state();
        switch (gameState)
        {
            case 0:
                update(smoothedDeltaRealTime_ms / 1000);
                pushEvents();
                break;
            case 1:
                if (isOffline)
                {
                    handleGameStateOne();
                }
                else
                {
                    activity.finish();
                }
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

    private void handleGameStateOne()
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

        String beginGameMessage = _beginGameMessages[_beginGameMessagesIndex++];

        on_chat_received(beginGameMessage);

        if (_beginGameMessagesIndex >= _beginGameMessages.length)
        {
            _beginGameMessagesIndex = 0;
        }
    }

    private void handleDeathForPlayerIndex(int playerIndex)
    {
        _playersAlive[playerIndex] = false;

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
                case SOUND_FORCE_FIELD_DOWN:
                    forceFieldDownSound.play(1);
                    break;
                case SOUND_DEATH:
                    deathSound.play(1);
                    break;
                case SOUND_GAME_SET:
                    gameSetSound.play(1);
                    break;
                case SOUND_DRAW:
                    drawSound.play(1);
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

    private static native void on_surface_created(int pixelWidth, int pixelHeight);

    private static native void on_surface_changed(int pixelWidth, int pixelHeight, int dpWidth, int dpHeight);

    private static native void init(String username, boolean isOffline);

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