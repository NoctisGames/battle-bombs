package com.technegames.bomberparty;

import java.util.HashMap;
import java.util.Locale;
import java.util.UUID;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

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

public final class MainActivity extends Activity implements ChatRequestListener,ConnectionRequestListener,LobbyRequestListener,NotifyListener,RoomRequestListener,ZoneRequestListener
{
	private static final Logger logger = new Logger(MainActivity.class);

	private String joinedRoomId;
	private static final String username;

	static
	{
		UUID uuid = UUID.randomUUID();
		username = "Player_" + uuid.toString().toUpperCase(Locale.US).substring(0, 16);

		WarpClient.initialize(AppWarpConstants.APPWARP_API_KEY, AppWarpConstants.APPWARP_HOST_ADDRESS);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);
	}

	@Override
	protected void onResume()
	{
		super.onResume();

		logger.debug("onResume");

		try
		{
			WarpClient warpClient = WarpClient.getInstance();
			warpClient.addChatRequestListener(this);
			warpClient.addConnectionRequestListener(this);
			warpClient.addLobbyRequestListener(this);
			warpClient.addNotificationListener(this);
			warpClient.addRoomRequestListener(this);
			warpClient.addZoneRequestListener(this);
		}
		catch (Exception e)
		{
			logger.error("Exception encountered in initializeAppWarp", e);
		}
	}

	@Override
	protected void onPause()
	{
		super.onPause();

		logger.debug("onPause");
	}

	public void startQuickMatch(View view)
	{
		try
		{
			WarpClient.getInstance().connectWithUserName(username, "android");
		}
		catch (Exception e)
		{
			logger.error(e.toString(), e);
		}
	}

	public void startQuickOfflineMatch(View view)
	{
		OfflineGameActivity.startActivity(this);
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

		try
		{
			WarpClient.getInstance().joinRoomInRange(0, 7, true);
		}
		catch (Exception e)
		{
			logger.error(e.toString(), e);
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
	public void onChatReceived(ChatEvent arg0)
	{
		logger.debug("onChatReceived");
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

		if (arg0.getId() != null)
		{
			joinedRoomId = arg0.getId();

			try
			{
				WarpClient.getInstance().removeChatRequestListener(this);
				WarpClient.getInstance().removeConnectionRequestListener(this);
				WarpClient.getInstance().removeLobbyRequestListener(this);
				WarpClient.getInstance().removeNotificationListener(this);
				WarpClient.getInstance().removeRoomRequestListener(this);
				WarpClient.getInstance().removeZoneRequestListener(this);

				GameActivity.startActivity(this, joinedRoomId, username);
			}
			catch (Exception e)
			{
				logger.error(e.toString(), e);
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