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
    private static final String KEEP_ALIVE = "KEEP_ALIVE";

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
    private static final short SOUND_PLANT_BOMB = 1;
    private static final short SOUND_EXPLOSION = 2;
    private static final short SOUND_PU_BOMB = 3;
    private static final short SOUND_PU_FIRE = 4;
    private static final short SOUND_PU_SPEED = 5;
    private static final short SOUND_PU_FORCE_FIELD = 6;
    private static final short SOUND_PU_PUSH = 7;
    private static final short SOUND_FORCE_FIELD_DOWN = 8;
    private static final short SOUND_DEATH = 9;

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
    private final Sound plantBombSound;
    private final Sound explosionSound;
    private final Sound powerUpBombSound;
    private final Sound powerUpFireSound;
    private final Sound powerUpSpeedSound;
    private final Sound powerUpForceFieldSound;
    private final Sound powerUpPushSound;
    private final Sound forceFieldDownSound;
    private final Sound deathSound;
    private final boolean isOffline;

    private Music bgm;

    private float smoothedDeltaRealTime_ms = 17.5f;
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms;
    private long lastRealTimeMeasurement_ms;
    private boolean isInitialized;

    // For Offline Mode
    private boolean[] _playersAlive = { true, true, true, true, true, true, true, true };
    int _mapType;

    public RendererWrapper(Activity activity, int deviceScreenWidth, int deviceScreenHeight, String username, boolean isOffline)
    {
        this.activity = activity;
        this.deviceScreenWidth = deviceScreenWidth;
        this.deviceScreenHeight = deviceScreenHeight;
        this.username = username;
        this.isOffline = isOffline;
        this.audio = new Audio(activity.getAssets());
        this.plantBombSound = audio.newSound("plant_bomb.ogg");
        this.explosionSound = audio.newSound("explosion.ogg");
        this.powerUpBombSound = audio.newSound("pu_bomb.ogg");
        this.powerUpFireSound = audio.newSound("pu_fire.ogg");
        this.powerUpSpeedSound = audio.newSound("pu_speed.ogg");
        this.powerUpForceFieldSound = audio.newSound("pu_force_field.ogg");
        this.powerUpPushSound = audio.newSound("pu_push.ogg");
        this.forceFieldDownSound = audio.newSound("force_field_down.ogg");
        this.deathSound = audio.newSound("death.ogg");

        this.isInitialized = false;

        init(username, isOffline);

        if (isOffline)
        {
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
                String gameOverMessage = String.format(Locale.US, "{\"eventType\": %i, \"hasWinner\": false, \"winningPlayerIndex\": %i}", GAME_OVER, -1);

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

        String beginGameMessage = String.format(Locale.US, "{\"breakableBlockPowerUpFlags\": \"0,1,2,3,4,5\", \"breakableBlockXValues\": \"8,4,5,7,8,9\", \"breakableBlockYValues\": \"0,1,2,3,4,5\", \"eventType\": 1337, \"mapType\": %i, \"numBreakableBlocks\": 5, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"%s\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Bot 1\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Bot 3\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Bot 4\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Bot 5\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Bot 6\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Bot 7\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}", _mapType, this.username);

        on_chat_received(beginGameMessage);

        _mapType++;
        if (_mapType >= 3)
        {
            _mapType = 0;
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
                gameOverMessage = String.format(Locale.US, "{\"eventType\": %i, \"hasWinner\": true, \"winningPlayerIndex\": %i}", GAME_OVER, winningPlayerIndex);
            }
            else
            {
                gameOverMessage = String.format(Locale.US, "{\"eventType\": %i, \"hasWinner\": false, \"winningPlayerIndex\": %i}", GAME_OVER, winningPlayerIndex);
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