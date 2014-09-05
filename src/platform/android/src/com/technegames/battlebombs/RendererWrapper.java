package com.technegames.battlebombs;

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
    private static final short CLIENT_UPDATE = 1338;

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
    private final Sound deathSound;
    private final boolean isOnline;

    private Music bgm;

    private float smoothedDeltaRealTime_ms = 17.5f; // initial value, Optionally
                                                    // you can save the new
                                                    // computed value (will
                                                    // change with each
                                                    // hardware) in Preferences
                                                    // to optimize the first
                                                    // drawing frames
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms; // mov
                                                                     // Average
                                                                     // start
                                                                     // with
                                                                     // default
                                                                     // value
    private long lastRealTimeMeasurement_ms; // temporal storage for last time
                                             // measurement
    private boolean isInitialized;

    public RendererWrapper(Activity activity, int deviceScreenWidth, int deviceScreenHeight, String username)
    {
        this.activity = activity;
        this.deviceScreenWidth = deviceScreenWidth;
        this.deviceScreenHeight = deviceScreenHeight;
        this.username = username;
        this.isOnline = !username.equalsIgnoreCase("Player_Offline");
        this.audio = new Audio(activity.getAssets());
        this.plantBombSound = audio.newSound("plant_bomb.ogg");
        this.explosionSound = audio.newSound("explosion.ogg");
        this.deathSound = audio.newSound("death.ogg");

        this.isInitialized = false;

        init(username);
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
                init(username);
                break;
            case 2:
                activity.finish();
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
        } else
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
        if (isOnline)
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
                } else if (is_time_to_send_keep_alive())
                {
                    reset_time_since_last_client_event();

                    WarpClient.getInstance().sendChat(KEEP_ALIVE);
                }
            } catch (Exception e)
            {
                logger.error(e.toString(), e);
            }
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

    private static native void init(String username);

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
}