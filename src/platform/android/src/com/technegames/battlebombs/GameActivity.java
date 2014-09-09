package com.technegames.battlebombs;

import java.util.HashMap;
import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.shephertz.app42.gaming.multiplayer.client.WarpClient;
import com.shephertz.app42.gaming.multiplayer.client.events.ChatEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.ConnectEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.LobbyData;
import com.shephertz.app42.gaming.multiplayer.client.events.MoveEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.RoomData;
import com.shephertz.app42.gaming.multiplayer.client.events.UpdateEvent;
import com.shephertz.app42.gaming.multiplayer.client.listener.ConnectionRequestListener;
import com.shephertz.app42.gaming.multiplayer.client.listener.NotifyListener;

public final class GameActivity extends BaseGameActivity implements ConnectionRequestListener, NotifyListener
{
    private static final Logger logger = new Logger(GameActivity.class);

    private static final String EVENT_TYPE = "eventType";
    private static final String PHASE = "phase";

    // Definitions from src/core/GameEvent.h
    // Event Type
    private static final int PRE_GAME = 1334;

    // Pre Game Phases
    private static final int CONNECTING = 1;
    private static final int FINDING_ROOM_TO_JOIN = 2;
    private static final int ROOM_JOINED_WAITING_FOR_SERVER = 3;
    private static final int CONNECTION_ERROR = 4;

    static
    {
        WarpClient.initialize(AppWarpConstants.APPWARP_APP_KEY, AppWarpConstants.APPWARP_HOST_ADDRESS);
    }

    public static void startActivity(Activity activity, String username)
    {
        Intent i = new Intent(activity, GameActivity.class);
        i.putExtra(EXTRA_USERNAME, username);
        activity.startActivity(i);
    }

    private String _joinedRoomId;

    @Override
    protected boolean isOffline()
    {
        return false;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        try
        {
            WarpClient.getInstance().addConnectionRequestListener(this);
            WarpClient.getInstance().addNotificationListener(this);

            final String preGameUpdate = String.format(Locale.US, "{\"%s\":%d,\"%s\":%d}", EVENT_TYPE, PRE_GAME, PHASE, CONNECTING);

            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    _rendererWrapper.onChatReceived(preGameUpdate);
                }
            });

            WarpClient.getInstance().connectWithUserName(_username, "T3chn3G4m35");
        }
        catch (Exception e)
        {
            logger.error(e.toString(), e);
        }
    }

    @Override
    protected void onPause()
    {
        super.onPause();

        try
        {
            WarpClient.getInstance().removeConnectionRequestListener(this);
            WarpClient.getInstance().removeNotificationListener(this);

            WarpClient.getInstance().leaveRoom(_joinedRoomId);

            WarpClient.getInstance().disconnect();
        }
        catch (Exception e)
        {
            logger.error(e.toString(), e);
        }
    }

    /** ConnectionRequestListener **/

    @Override
    public void onConnectDone(ConnectEvent arg0, String arg1)
    {
        logger.debug("onConnectDone");

        if (arg0.getResult() == 0)
        {
            final String preGameUpdate = String.format(Locale.US, "{\"%s\":%d,\"%s\":%d}", EVENT_TYPE, PRE_GAME, PHASE, FINDING_ROOM_TO_JOIN);

            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    _rendererWrapper.onChatReceived(preGameUpdate);
                }
            });

            try
            {
                WarpClient.getInstance().joinRoomInRange(0, 7, true);
            }
            catch (Exception e)
            {
                logger.error(e.toString(), e);
            }
        }
        else
        {
            final String preGameUpdate = String.format(Locale.US, "{\"%s\":%d,\"%s\":%d}", EVENT_TYPE, PRE_GAME, PHASE, CONNECTION_ERROR);

            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    _rendererWrapper.onChatReceived(preGameUpdate);
                }
            });
        }
    }

    @Override
    public void onDisconnectDone(ConnectEvent arg0)
    {
        logger.debug("onDisconnectDone");
    }

    @Override
    public void onInitUDPDone(byte arg0)
    {
        logger.debug("onInitUDPDone");
    }

    /** NotifyListener **/

    @Override
    public void onChatReceived(final ChatEvent arg0)
    {
        logger.debug("onChatReceived");

        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                _rendererWrapper.onChatReceived(arg0.getMessage());
            }
        });
    }

    @Override
    public void onGameStarted(String arg0, String arg1, String arg2)
    {
        logger.debug("onGameStarted");
    }

    @Override
    public void onGameStopped(String arg0, String arg1)
    {
        logger.debug("onGameStopped");
    }

    @Override
    public void onMoveCompleted(MoveEvent arg0)
    {
        logger.debug("onMoveCompleted");
    }

    @Override
    public void onPrivateChatReceived(String arg0, String arg1)
    {
        logger.debug("onPrivateChatReceived");
    }

    @Override
    public void onRoomCreated(RoomData arg0)
    {
        logger.debug("onRoomCreated");
    }

    @Override
    public void onRoomDestroyed(RoomData arg0)
    {
        logger.debug("onRoomDestroyed");
    }

    @Override
    public void onUpdatePeersReceived(UpdateEvent arg0)
    {
        logger.debug("onUpdatePeersReceived");
    }

    @Override
    public void onUserChangeRoomProperty(RoomData arg0, String arg1, HashMap<String, Object> arg2, HashMap<String, String> arg3)
    {
        logger.debug("onUserChangeRoomProperty");
    }

    @Override
    public void onUserJoinedLobby(LobbyData arg0, String arg1)
    {
        logger.debug("onUserJoinedLobby");
    }

    @Override
    public void onUserJoinedRoom(RoomData arg0, String arg1)
    {
        logger.debug("onUserJoinedRoom");

        if (arg0 != null)
        {
            if (_username.equals(arg1))
            {
                _joinedRoomId = arg0.getId();

                final String preGameUpdate = String.format(Locale.US, "{\"%s\":%d,\"%s\":%d}", EVENT_TYPE, PRE_GAME, PHASE, ROOM_JOINED_WAITING_FOR_SERVER);

                runOnUiThread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        _rendererWrapper.onChatReceived(preGameUpdate);
                    }
                });
            }
        }
    }

    @Override
    public void onUserLeftLobby(LobbyData arg0, String arg1)
    {
        logger.debug("onUserLeftLobby");
    }

    @Override
    public void onUserLeftRoom(RoomData arg0, String arg1)
    {
        logger.debug("onUserLeftRoom");

        if (_username.equals(arg1))
        {
            finish();
        }
    }

    @Override
    public void onUserPaused(String arg0, boolean arg1, String arg2)
    {
        logger.debug("onUserPaused");
    }

    @Override
    public void onUserResumed(String arg0, boolean arg1, String arg2)
    {
        logger.debug("onUserResumed");
    }
}