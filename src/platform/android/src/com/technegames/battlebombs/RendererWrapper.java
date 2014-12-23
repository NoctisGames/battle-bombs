package com.technegames.battlebombs;

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
    
    private final boolean _isOffline;

    private Music bgm;

    private float smoothedDeltaRealTime_ms = 17.5f;
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms;
    private long lastRealTimeMeasurement_ms;
    private boolean isInitialized;

    public RendererWrapper(Activity activity, int deviceScreenWidth, int deviceScreenHeight, String username, boolean isOffline)
    {
        this.activity = activity;
        this.deviceScreenWidth = deviceScreenWidth;
        this.deviceScreenHeight = deviceScreenHeight;
        this.username = username;
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
        _isOffline = isOffline;

        this.isInitialized = false;

        init(username, isOffline);
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
                if (!_isOffline)
                {
                    pushEvents();
                }
                break;
            case SCREEN_STATE_CONNECTION_ERROR:
                activity.setResult(GameActivity.RESULT_CONNECTION_ERROR);
            case SCREEN_STATE_EXIT:
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