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
    private static final short SOUND_PU_MEGA_FIRE = 13;
    private static final short SOUND_PU_REMOTE_BOMB = 14;
    private static final short SOUND_PU_LAND_MINE = 15;
    private static final short SOUND_PU_CURSE = 16;
    private static final short SOUND_FORCE_FIELD_DOWN = 17;
    private static final short SOUND_DEATH = 18;
    private static final short SOUND_HURRY = 19;
    private static final short SOUND_GAME_SET = 20;
    private static final short SOUND_DRAW = 21;
    private static final short SOUND_RAISE_SHIELD = 22;
    private static final short SOUND_DISLODGING_SPACE_TILE = 23;
    private static final short SOUND_FALLING_SPACE_TILE = 24;
    private static final short SOUND_FALLING_OBJECT = 25;
    private static final short SOUND_CRASHING_FIRE_BALL = 26;
    private static final short SOUND_CRASHING_ICE_BALL = 27;
    private static final short SOUND_MINI_EXPLOSION = 28;
    private static final short SOUND_BASE_TILE_TRIGGER = 29;

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

    private final Activity _activity;
    private final int _deviceScreenWidth;
    private final int _deviceScreenHeight;
    private final String _username;
    private final Audio _audio;
    private final Sound _countDown3Sound;
    private final Sound _countDown2Sound;
    private final Sound _countDown1Sound;
    private final Sound _baseTileTriggerSound;
    private final Sound _miniExplosionSound;
    private final Sound _battleSound;
    private final Sound _plantBombSound;
    private final Sound _explosionSound;
    private final Sound _powerUpBombSound;
    private final Sound _powerUpFireSound;
    private final Sound _powerUpSpeedSound;
    private final Sound _powerUpForceFieldSound;
    private final Sound _powerUpPushSound;
    private final Sound _powerUpShieldSound;
    private final Sound _powerUpMegaFireSound;
    private final Sound _powerUpRemoteBombSound;
    private final Sound _powerUpLandmineSound;
    private final Sound _powerUpCurseSound;
    private final Sound _forceFieldDownSound;
    private final Sound _deathSound;
    private final Sound _hurrySound;
    private final Sound _gameSetSound;
    private final Sound _drawSound;
    private final Sound _raiseShieldSound;
    private final Sound _dislodgingSpaceTileSound;
    private final Sound _fallingSpaceTileSound;
    private final Sound _fallingObjectSound;
    private final Sound _crashingFireBallSound;
    private final Sound _crashingIceBallSound;

    private final boolean _isOffline;

    private Music _bgm;

    private float _smoothedDeltaRealTime_ms = 17.5f;
    private float _movAverageDeltaTime_ms = _smoothedDeltaRealTime_ms;
    private long _lastRealTimeMeasurement_ms;
    private boolean _isInitialized;

    public RendererWrapper(Activity activity, int deviceScreenWidth, int deviceScreenHeight, String username, boolean isOffline)
    {
        _activity = activity;
        _deviceScreenWidth = deviceScreenWidth;
        _deviceScreenHeight = deviceScreenHeight;
        _username = username;
        _audio = new Audio(activity.getAssets());
        _countDown3Sound = _audio.newSound("countdown_3.ogg");
        _countDown2Sound = _audio.newSound("countdown_2.ogg");
        _countDown1Sound = _audio.newSound("countdown_1.ogg");
        _baseTileTriggerSound = _audio.newSound("base_tile_trigger.ogg");
        _miniExplosionSound = _audio.newSound("mini_explosion.ogg");
        _battleSound = _audio.newSound("battle.ogg");
        _plantBombSound = _audio.newSound("plant_bomb.ogg");
        _explosionSound = _audio.newSound("explosion.ogg");
        _powerUpBombSound = _audio.newSound("pu_bomb.ogg");
        _powerUpFireSound = _audio.newSound("pu_fire.ogg");
        _powerUpSpeedSound = _audio.newSound("pu_speed.ogg");
        _powerUpForceFieldSound = _audio.newSound("pu_force_field.ogg");
        _powerUpPushSound = _audio.newSound("pu_push.ogg");
        _powerUpShieldSound = _audio.newSound("pu_shield.ogg");
        _powerUpMegaFireSound = _audio.newSound("pu_mega_fire.ogg");
        _powerUpRemoteBombSound = _audio.newSound("pu_remote_bomb.ogg");
        _powerUpLandmineSound = _audio.newSound("pu_landmine.ogg");
        _powerUpCurseSound = _audio.newSound("pu_curse.ogg");
        _forceFieldDownSound = _audio.newSound("force_field_down.ogg");
        _deathSound = _audio.newSound("death.ogg");
        _hurrySound = _audio.newSound("hurry.ogg");
        _gameSetSound = _audio.newSound("game_set.ogg");
        _drawSound = _audio.newSound("draw.ogg");
        _raiseShieldSound = _audio.newSound("shield_raise.ogg");
        _dislodgingSpaceTileSound = _audio.newSound("dislodging_space_tile.ogg");
        _fallingSpaceTileSound = _audio.newSound("falling_space_tile.ogg");
        _fallingObjectSound = _audio.newSound("falling_object.ogg");
        _crashingFireBallSound = _audio.newSound("crashing_fire_ball.ogg");
        _crashingIceBallSound = _audio.newSound("crashing_ice_ball.ogg");
        _isOffline = isOffline;

        _isInitialized = false;

        init(username, isOffline);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        logger.debug("GL Surface created!");

        if (!_isInitialized)
        {
            PlatformFileUtils.init_asset_manager(_activity.getAssets());
            _isInitialized = true;
        }

        on_surface_created(_deviceScreenWidth, _deviceScreenHeight);
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
                displayToastOnUiThread(_activity.getString(R.string.you_are_in_spectator_mode));
                clear_state();
            case SCREEN_STATE_NORMAL:
                update(_smoothedDeltaRealTime_ms / 1000);
                if (!_isOffline)
                {
                    pushEvents();
                }
                break;
            case SCREEN_STATE_CONNECTION_ERROR:
                _activity.setResult(GameActivity.RESULT_CONNECTION_ERROR);
            case SCREEN_STATE_EXIT:
                _activity.finish();
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
        if (_lastRealTimeMeasurement_ms > 0)
        {
            realTimeElapsed_ms = (currTimePick_ms - _lastRealTimeMeasurement_ms);
        }
        else
        {
            realTimeElapsed_ms = _smoothedDeltaRealTime_ms; // just the first
                                                            // time
        }

        _movAverageDeltaTime_ms = (realTimeElapsed_ms + _movAverageDeltaTime_ms * (movAveragePeriod - 1)) / movAveragePeriod;

        // Calc a better aproximation for smooth stepTime
        _smoothedDeltaRealTime_ms = _smoothedDeltaRealTime_ms + (_movAverageDeltaTime_ms - _smoothedDeltaRealTime_ms) * smoothFactor;

        _lastRealTimeMeasurement_ms = currTimePick_ms;
    }

    public void onResume()
    {
        on_resume();
    }

    public void onPause()
    {
        if (_bgm != null)
        {
            _bgm.stop();
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
                    _countDown3Sound.play(1);
                    break;
                case SOUND_COUNT_DOWN_2:
                    _countDown2Sound.play(1);
                    break;
                case SOUND_COUNT_DOWN_1:
                    _countDown1Sound.play(1);
                    break;
                case SOUND_BATTLE:
                    _battleSound.play(1);
                    break;
                case SOUND_PLANT_BOMB:
                    _plantBombSound.play(1);
                    break;
                case SOUND_EXPLOSION:
                    _explosionSound.play(1);
                    break;
                case SOUND_PU_BOMB:
                    _powerUpBombSound.play(1);
                    break;
                case SOUND_PU_FIRE:
                    _powerUpFireSound.play(1);
                    break;
                case SOUND_PU_SPEED:
                    _powerUpSpeedSound.play(1);
                    break;
                case SOUND_PU_FORCE_FIELD:
                    _powerUpForceFieldSound.play(1);
                    break;
                case SOUND_PU_PUSH:
                    _powerUpPushSound.play(1);
                    break;
                case SOUND_PU_SHIELD:
                    _powerUpShieldSound.play(1);
                    break;
                case SOUND_PU_MEGA_FIRE:
                    _powerUpMegaFireSound.play(1);
                    break;
                case SOUND_PU_REMOTE_BOMB:
                    _powerUpRemoteBombSound.play(1);
                    break;
                case SOUND_PU_LAND_MINE:
                    _powerUpLandmineSound.play(1);
                    break;
                case SOUND_PU_CURSE:
                    _powerUpCurseSound.play(1);
                    break;
                case SOUND_FORCE_FIELD_DOWN:
                    _forceFieldDownSound.play(1);
                    break;
                case SOUND_DEATH:
                    _deathSound.play(1);
                    break;
                case SOUND_HURRY:
                    _hurrySound.play(1);
                    break;
                case SOUND_GAME_SET:
                    _gameSetSound.play(1);
                    break;
                case SOUND_DRAW:
                    _drawSound.play(1);
                    break;
                case SOUND_RAISE_SHIELD:
                    _raiseShieldSound.play(1);
                    break;
                case SOUND_DISLODGING_SPACE_TILE:
                    _dislodgingSpaceTileSound.play(1);
                    break;
                case SOUND_FALLING_SPACE_TILE:
                    _fallingSpaceTileSound.play(1);
                    break;
                case SOUND_FALLING_OBJECT:
                    _fallingObjectSound.play(1);
                    break;
                case SOUND_CRASHING_FIRE_BALL:
                    _crashingFireBallSound.play(1);
                    break;
                case SOUND_CRASHING_ICE_BALL:
                    _crashingIceBallSound.play(1);
                    break;
                case SOUND_MINI_EXPLOSION:
                    _miniExplosionSound.play(1);
                    break;
                case SOUND_BASE_TILE_TRIGGER:
                    _baseTileTriggerSound.play(1);
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
                if (_bgm != null)
                {
                    _bgm.stop();
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
        if (_bgm != null && _bgm.isPlaying())
        {
            _bgm.dispose();
            _bgm = null;
        }

        _bgm = _audio.newMusic(fileName);
        _bgm.setLooping(true);
        _bgm.play();
    }

    private void displayToastOnUiThread(final String toast)
    {
        _activity.runOnUiThread(new Runnable()
        {
            public void run()
            {
                Toast.makeText(_activity, toast, Toast.LENGTH_SHORT).show();
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