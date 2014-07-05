using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;
using System.Diagnostics;
using System.IO.IsolatedStorage;
using com.shephertz.app42.gaming.multiplayer.client;
using com.shephertz.app42.gaming.multiplayer.client.events;
using BomberPartyComp;

namespace BomberParty
{
    public partial class GamePage : PhoneApplicationPage
    {
        private com.shephertz.app42.gaming.multiplayer.client.listener.ChatRequestListener m_chatRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.ConnectionRequestListener m_connectionRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.LobbyRequestListener m_lobbyRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.NotifyListener m_notifyListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.RoomRequestListener m_roomRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.ZoneRequestListener m_zoneRequestListener;

        private Direct3DInterop m_d3dInterop = null;
        private string m_joinedRoomId;
        private string m_username;

        public GamePage()
        {
            InitializeComponent();

            initializeAppWarp();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;

            if (NavigationContext.QueryString.TryGetValue("joinedRoomId", out m_joinedRoomId))
            {
                Console.WriteLine("m_joinedRoomId = " + m_joinedRoomId);
            }

            if (NavigationContext.QueryString.TryGetValue("username", out m_username))
            {
                Console.WriteLine("m_username = " + m_username);
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);

            WarpClient.GetInstance().RemoveChatRequestListener(m_chatRequestListener);
            WarpClient.GetInstance().RemoveConnectionRequestListener(m_connectionRequestListener);
            WarpClient.GetInstance().RemoveLobbyRequestListener(m_lobbyRequestListener);
            WarpClient.GetInstance().RemoveNotificationListener(m_notifyListener);
            WarpClient.GetInstance().RemoveRoomRequestListener(m_roomRequestListener);
            WarpClient.GetInstance().RemoveZoneRequestListener(m_zoneRequestListener);

            WarpClient.GetInstance().LeaveRoom(m_joinedRoomId);
            WarpClient.GetInstance().Disconnect();

            NavigationService.GoBack();
        }

        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);

            if (m_d3dInterop.onBackPressed())
            {
                e.Cancel = true;
            }
        }

        private void DrawingSurface_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_d3dInterop == null)
            {
                m_d3dInterop = new Direct3DInterop();

                // Set window bounds in dips
                m_d3dInterop.WindowBounds = new Windows.Foundation.Size((float)DrawingSurface.ActualWidth, (float)DrawingSurface.ActualHeight);

                // Set native resolution in pixels
                m_d3dInterop.NativeResolution = new Windows.Foundation.Size((float)Math.Floor(DrawingSurface.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f), (float)Math.Floor(DrawingSurface.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f));

                // Set render resolution to the full native resolution
                m_d3dInterop.RenderResolution = m_d3dInterop.NativeResolution;

                // Hook-up native component to DrawingSurface
                DrawingSurface.SetBackgroundContentProvider(m_d3dInterop.CreateContentProvider(m_username));
                DrawingSurface.SetBackgroundManipulationHandler(m_d3dInterop);

                m_d3dInterop.setWinRtCallback(new WinRtCallback(ProcessCallback));
            }
        }

        private void ProcessCallback(String command, String param)
        {
            Console.WriteLine("Incoming callback from C++ : " + command);

            if (command.Equals("SEND_CHAT"))
            {
                WarpClient.GetInstance().SendChat(param);
            }
        }

        private void initializeAppWarp()
        {
            // We pass an instance of GamePage so that these listeners can change properties of the page, like show chats or other game status
            m_chatRequestListener = new MyChatRequestListener(this);
            m_connectionRequestListener = new MyConnectionRequestListener(this);
            m_lobbyRequestListener = new MyLobbyRequestListener(this);
            m_notifyListener = new MyNotifyListener(this);
            m_roomRequestListener = new MyRoomRequestListener(this);
            m_zoneRequestListener = new MyZoneRequestListener(this);

            WarpClient.GetInstance().AddChatRequestListener(m_chatRequestListener);
            WarpClient.GetInstance().AddConnectionRequestListener(m_connectionRequestListener);
            WarpClient.GetInstance().AddLobbyRequestListener(m_lobbyRequestListener);
            WarpClient.GetInstance().AddNotificationListener(m_notifyListener);
            WarpClient.GetInstance().AddRoomRequestListener(m_roomRequestListener);
            WarpClient.GetInstance().AddZoneRequestListener(m_zoneRequestListener);
        }

        private class MyChatRequestListener : com.shephertz.app42.gaming.multiplayer.client.listener.ChatRequestListener
        {
            private GamePage _page;

            public MyChatRequestListener(GamePage result)
            {
                _page = result;
            }

            public void onSendChatDone(byte result)
            {
                Debug.WriteLine("onSendChatDone");
            }

            public void onSendPrivateChatDone(byte result)
            {
                Debug.WriteLine("onSendPrivateChatDone");
            }
        }

        private class MyConnectionRequestListener : com.shephertz.app42.gaming.multiplayer.client.listener.ConnectionRequestListener
        {
            private GamePage _page;

            public MyConnectionRequestListener(GamePage result)
            {
                _page = result;
            }

            public void onConnectDone(ConnectEvent eventObj)
            {
                Debug.WriteLine("onConnectDone");
            }

            public void onDisconnectDone(ConnectEvent eventObj)
            {
                Debug.WriteLine("onDisconnectDone");
            }

            public void onInitUDPDone(byte resultCode)
            {
                Debug.WriteLine("onInitUDPDone");
            }
        }

        private class MyLobbyRequestListener : com.shephertz.app42.gaming.multiplayer.client.listener.LobbyRequestListener
        {
            private GamePage _page;

            public MyLobbyRequestListener(GamePage result)
            {
                _page = result;
            }

            public void onGetLiveLobbyInfoDone(LiveRoomInfoEvent eventObj)
            {
                Debug.WriteLine("onGetLiveLobbyInfoDone");
            }

            public void onJoinLobbyDone(LobbyEvent eventObj)
            {
                Debug.WriteLine("onJoinLobbyDone");
            }

            public void onLeaveLobbyDone(LobbyEvent eventObj)
            {
                Debug.WriteLine("onLeaveLobbyDone");
            }

            public void onSubscribeLobbyDone(LobbyEvent eventObj)
            {
                Debug.WriteLine("onSubscribeLobbyDone");
            }

            public void onUnSubscribeLobbyDone(LobbyEvent eventObj)
            {
                Debug.WriteLine("onUnSubscribeLobbyDone");
            }
        }

        private class MyNotifyListener : com.shephertz.app42.gaming.multiplayer.client.listener.NotifyListener
        {
            private GamePage _page;

            public MyNotifyListener(GamePage page)
            {
                _page = page;
            }

            public void onChatReceived(ChatEvent eventObj)
            {
                Debug.WriteLine("onChatReceived");

                _page.m_d3dInterop.onChatReceived(eventObj.getMessage());
            }

            public void onGameStarted(string sender, string roomId, string nextTurn)
            {
                Debug.WriteLine("onGameStarted");
            }

            public void onGameStopped(string sender, string roomId)
            {
                Debug.WriteLine("onGameStopped");
            }

            public void onMoveCompleted(MoveEvent moveEvent)
            {
                Debug.WriteLine("onMoveCompleted");
            }

            public void onPrivateChatReceived(string sender, string message)
            {
                Debug.WriteLine("onPrivateChatReceived");
            }

            public void onRoomCreated(RoomData eventObj)
            {
                Debug.WriteLine("onRoomCreated");
            }

            public void onRoomDestroyed(RoomData eventObj)
            {
                Debug.WriteLine("onRoomDestroyed");
            }

            public void onUpdatePeersReceived(UpdateEvent eventObj)
            {
                Debug.WriteLine("onUpdatePeersReceived");
            }

            public void onUserChangeRoomProperty(RoomData roomData, string sender, Dictionary<string, object> properties, Dictionary<string, string> lockedPropertiesTable)
            {
                Debug.WriteLine("onUserChangeRoomProperty");
            }

            public void onUserJoinedLobby(LobbyData eventObj, string username)
            {
                Debug.WriteLine("onUserJoinedLobby");
            }

            public void onUserJoinedRoom(RoomData eventObj, string username)
            {
                Debug.WriteLine("onUserJoinedRoom");
            }

            public void onUserLeftLobby(LobbyData eventObj, string username)
            {
                Debug.WriteLine("onUserLeftLobby");
            }

            public void onUserLeftRoom(RoomData eventObj, string username)
            {
                Debug.WriteLine("onUserLeftRoom");

                if (username.Equals(_page.m_username))
                {
                    _page.NavigationService.GoBack();
                }
            }

            public void onUserPaused(string locid, bool isLobby, string username)
            {
                Debug.WriteLine("onUserPaused");
            }

            public void onUserResumed(string locid, bool isLobby, string username)
            {
                Debug.WriteLine("onUserResumed");
            }
        }

        private class MyRoomRequestListener : com.shephertz.app42.gaming.multiplayer.client.listener.RoomRequestListener
        {
            private GamePage _page;

            public MyRoomRequestListener(GamePage page)
            {
                _page = page;
            }

            public void onGetLiveRoomInfoDone(LiveRoomInfoEvent eventObj)
            {
                Debug.WriteLine("onGetLiveRoomInfoDone");
            }

            public void onJoinRoomDone(RoomEvent eventObj)
            {
                Debug.WriteLine("onJoinRoomDone");
            }

            public void onLeaveRoomDone(RoomEvent eventObj)
            {
                Debug.WriteLine("onLeaveRoomDone");
            }

            public void onLockPropertiesDone(byte result)
            {
                Debug.WriteLine("onLockPropertiesDone");
            }

            public void onSetCustomRoomDataDone(LiveRoomInfoEvent eventObj)
            {
                Debug.WriteLine("onSetCustomRoomDataDone");
            }

            public void onSubscribeRoomDone(RoomEvent eventObj)
            {
                Debug.WriteLine("onSubscribeRoomDone");
            }

            public void onUnlockPropertiesDone(byte result)
            {
                Debug.WriteLine("onUnlockPropertiesDone");
            }

            public void onUnSubscribeRoomDone(RoomEvent eventObj)
            {
                Debug.WriteLine("onUnSubscribeRoomDone");
            }

            public void onUpdatePropertyDone(LiveRoomInfoEvent lifeLiveRoomInfoEvent)
            {
                Debug.WriteLine("onUpdatePropertyDone");
            }
        }

        private class MyZoneRequestListener : com.shephertz.app42.gaming.multiplayer.client.listener.ZoneRequestListener
        {
            private GamePage _page;

            public MyZoneRequestListener(GamePage page)
            {
                _page = page;
            }

            public void onCreateRoomDone(RoomEvent eventObj)
            {
                Debug.WriteLine("onCreateRoomDone");
            }

            public void onDeleteRoomDone(RoomEvent eventObj)
            {
                Debug.WriteLine("onDeleteRoomDone");
            }

            public void onGetAllRoomsDone(AllRoomsEvent eventObj)
            {
                Debug.WriteLine("onGetAllRoomsDone");
            }

            public void onGetLiveUserInfoDone(LiveUserInfoEvent eventObj)
            {
                Debug.WriteLine("onGetLiveUserInfoDone");
            }

            public void onGetMatchedRoomsDone(MatchedRoomsEvent matchedRoomsEvent)
            {
                Debug.WriteLine("onGetMatchedRoomsDone");
            }

            public void onGetOnlineUsersDone(AllUsersEvent eventObj)
            {
                Debug.WriteLine("onGetOnlineUsersDone");
            }

            public void onSetCustomUserDataDone(LiveUserInfoEvent eventObj)
            {
                Debug.WriteLine("onSetCustomUserDataDone");
            }
        }
    }
}