package com.technegames.battlebombs;

import java.util.HashMap;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Point;
import android.media.AudioManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.shephertz.app42.gaming.multiplayer.client.WarpClient;
import com.shephertz.app42.gaming.multiplayer.client.events.AllRoomsEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.AllUsersEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.ChatEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.ConnectEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.LiveRoomInfoEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.LiveUserInfoEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.LobbyData;
import com.shephertz.app42.gaming.multiplayer.client.events.LobbyEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.MatchedRoomsEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.MoveEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.RoomData;
import com.shephertz.app42.gaming.multiplayer.client.events.RoomEvent;
import com.shephertz.app42.gaming.multiplayer.client.events.UpdateEvent;
import com.shephertz.app42.gaming.multiplayer.client.listener.ChatRequestListener;
import com.shephertz.app42.gaming.multiplayer.client.listener.ConnectionRequestListener;
import com.shephertz.app42.gaming.multiplayer.client.listener.LobbyRequestListener;
import com.shephertz.app42.gaming.multiplayer.client.listener.NotifyListener;
import com.shephertz.app42.gaming.multiplayer.client.listener.RoomRequestListener;
import com.shephertz.app42.gaming.multiplayer.client.listener.ZoneRequestListener;
import com.technegames.battlebombs.R;

public final class GameActivity extends Activity implements ChatRequestListener,ConnectionRequestListener,LobbyRequestListener,NotifyListener,RoomRequestListener,ZoneRequestListener
{
	private static final Logger logger = new Logger(GameActivity.class);
	private static final String EXTRA_JOINED_ROOM_ID = "EXTRA_JOINED_ROOM_ID";
	private static final String EXTRA_USERNAME = "EXTRA_USERNAME";

	public static void startActivity(Activity activity, String roomId, String username)
	{
		Intent i = new Intent(activity, GameActivity.class);
		i.putExtra(EXTRA_JOINED_ROOM_ID, roomId);
		i.putExtra(EXTRA_USERNAME, username);
		activity.startActivity(i);
	}

	private RendererWrapper rendererWrapper;
	private GLSurfaceView glSurfaceView;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_game);

		Point size = ViewUtils.getScreenSize(this);

		if (Logger.isDebugEnabled())
		{
			logger.debug("dimension " + size.x + " x " + size.y);
		}

		rendererWrapper = new RendererWrapper(this, size.x, size.y, getIntent().getStringExtra(EXTRA_USERNAME));
		glSurfaceView = new GLSurfaceView(this);
		glSurfaceView.setEGLContextClientVersion(1);
		glSurfaceView.setRenderer(rendererWrapper);

		LinearLayout gameContainer = (LinearLayout) findViewById(R.id.game);
		gameContainer.addView(glSurfaceView);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED);
		setVolumeControlStream(AudioManager.STREAM_MUSIC);

		glSurfaceView.setOnTouchListener(new OnTouchListener()
		{
			@Override
			public boolean onTouch(View v, final MotionEvent event)
			{
				synchronized (this)
				{
					int action = event.getAction() & MotionEvent.ACTION_MASK;
					int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;

					switch (action)
					{
						case MotionEvent.ACTION_DOWN:
						case MotionEvent.ACTION_POINTER_DOWN:
							rendererWrapper.handleTouchDown(event.getX(pointerIndex), event.getY(pointerIndex));
							break;
						case MotionEvent.ACTION_UP:
						case MotionEvent.ACTION_POINTER_UP:
						case MotionEvent.ACTION_CANCEL:
							rendererWrapper.handleTouchUp(event.getX(pointerIndex), event.getY(pointerIndex));
							break;
						case MotionEvent.ACTION_MOVE:
							for (int i = 0; i < event.getPointerCount(); i++)
							{
								pointerIndex = i;
								rendererWrapper.handleTouchDragged(event.getX(pointerIndex), event.getY(pointerIndex));
							}
							break;
					}

					return true;
				}
			}
		});

		try
		{
			WarpClient.getInstance().addChatRequestListener(this);
			WarpClient.getInstance().addConnectionRequestListener(this);
			WarpClient.getInstance().addLobbyRequestListener(this);
			WarpClient.getInstance().addNotificationListener(this);
			WarpClient.getInstance().addRoomRequestListener(this);
			WarpClient.getInstance().addZoneRequestListener(this);
		}
		catch (Exception e)
		{
			logger.error(e.toString(), e);
		}
	}

	@Override
	protected void onResume()
	{
		super.onResume();

		glSurfaceView.onResume();
		rendererWrapper.onResume();
	}

	@Override
	protected void onPause()
	{
		rendererWrapper.onPause();
		glSurfaceView.onPause();

		try
		{
			WarpClient.getInstance().removeChatRequestListener(this);
			WarpClient.getInstance().removeConnectionRequestListener(this);
			WarpClient.getInstance().removeLobbyRequestListener(this);
			WarpClient.getInstance().removeNotificationListener(this);
			WarpClient.getInstance().removeRoomRequestListener(this);
			WarpClient.getInstance().removeZoneRequestListener(this);

			WarpClient.getInstance().leaveRoom(getIntent().getStringExtra(EXTRA_JOINED_ROOM_ID));

			WarpClient.getInstance().disconnect();
		}
		catch (Exception e)
		{
			logger.error(e.toString(), e);
		}

		finish();

		super.onPause();
	}

	@Override
	public void onBackPressed()
	{
		if (rendererWrapper.handleOnBackPressed())
		{
			return;
		}

		super.onBackPressed();
	}

	/** ChatRequestListener **/

	@Override
	public void onSendChatDone(byte arg0, String arg1)
	{
		logger.debug("onSendChatDone");
	}

	@Override
	public void onSendPrivateChatDone(byte arg0, String arg1)
	{
		logger.debug("onSendPrivateChatDone");
	}

	/** ConnectionRequestListener **/

	@Override
	public void onConnectDone(ConnectEvent arg0, String arg1)
	{
		logger.debug("onConnectDone");
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

	/** LobbyRequestListener **/

	@Override
	public void onGetLiveLobbyInfoDone(LiveRoomInfoEvent arg0)
	{
		logger.debug("onGetLiveLobbyInfoDone");
	}

	@Override
	public void onJoinLobbyDone(LobbyEvent arg0)
	{
		logger.debug("onJoinLobbyDone");
	}

	@Override
	public void onLeaveLobbyDone(LobbyEvent arg0)
	{
		logger.debug("onLeaveLobbyDone");
	}

	@Override
	public void onSubscribeLobbyDone(LobbyEvent arg0)
	{
		logger.debug("onSubscribeLobbyDone");
	}

	@Override
	public void onUnSubscribeLobbyDone(LobbyEvent arg0)
	{
		logger.debug("onUnSubscribeLobbyDone");
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
				rendererWrapper.onChatReceived(arg0.getMessage());
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

		if (getIntent().getStringExtra(EXTRA_USERNAME).equals(arg1))
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

	/** RoomRequestListener **/

	@Override
	public void onGetLiveRoomInfoDone(LiveRoomInfoEvent arg0)
	{
		logger.debug("onGetLiveRoomInfoDone");
	}

	@Override
	public void onJoinRoomDone(RoomEvent arg0, String arg1)
	{
		logger.debug("onJoinRoomDone");
	}

	@Override
	public void onLeaveRoomDone(RoomEvent arg0)
	{
		logger.debug("onLeaveRoomDone");
	}

	@Override
	public void onLockPropertiesDone(byte arg0)
	{
		logger.debug("onLockPropertiesDone");
	}

	@Override
	public void onSetCustomRoomDataDone(LiveRoomInfoEvent arg0)
	{
		logger.debug("onSetCustomRoomDataDone");
	}

	@Override
	public void onSubscribeRoomDone(RoomEvent arg0)
	{
		logger.debug("onSubscribeRoomDone");
	}

	@Override
	public void onUnSubscribeRoomDone(RoomEvent arg0)
	{
		logger.debug("onUnSubscribeRoomDone");
	}

	@Override
	public void onUnlockPropertiesDone(byte arg0)
	{
		logger.debug("onUnlockPropertiesDone");
	}

	@Override
	public void onUpdatePropertyDone(LiveRoomInfoEvent arg0, String arg1)
	{
		logger.debug("onUpdatePropertyDone");
	}

	/** ZoneRequestListener **/

	@Override
	public void onCreateRoomDone(RoomEvent arg0, String arg1)
	{
		logger.debug("onCreateRoomDone");
	}

	@Override
	public void onDeleteRoomDone(RoomEvent arg0, String arg1)
	{
		logger.debug("onDeleteRoomDone");
	}

	@Override
	public void onGetAllRoomsDone(final AllRoomsEvent arg0)
	{
		logger.debug("onGetAllRoomsDone");
	}

	@Override
	public void onGetLiveUserInfoDone(LiveUserInfoEvent arg0)
	{
		logger.debug("onGetLiveUserInfoDone");
	}

	@Override
	public void onGetMatchedRoomsDone(MatchedRoomsEvent arg0)
	{
		logger.debug("onGetMatchedRoomsDone");
	}

	@Override
	public void onGetOnlineUsersDone(AllUsersEvent arg0)
	{
		logger.debug("onGetOnlineUsersDone");
	}

	@Override
	public void onSetCustomUserDataDone(LiveUserInfoEvent arg0)
	{
		logger.debug("onSetCustomUserDataDone");
	}

	@Override
	public void onRPCDone(byte arg0, String arg1, Object arg2)
	{
		logger.debug("onRPCDone");
	}
}