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

namespace BomberParty
{
    public partial class MainPage : PhoneApplicationPage
    {
        private static string username;

        static MainPage()
        {
            String UUID = Guid.NewGuid().ToString();
            username = "Player_" + UUID.ToUpper().Substring(0, 16);
        }

        private com.shephertz.app42.gaming.multiplayer.client.listener.ChatRequestListener m_chatRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.ConnectionRequestListener m_connectionRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.LobbyRequestListener m_lobbyRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.NotifyListener m_notifyListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.RoomRequestListener m_roomRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.ZoneRequestListener m_zoneRequestListener;

        public MainPage()
        {
            InitializeComponent();

            initializeAppWarp();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Enabled;

            WarpClient.GetInstance().AddChatRequestListener(m_chatRequestListener);
            WarpClient.GetInstance().AddConnectionRequestListener(m_connectionRequestListener);
            WarpClient.GetInstance().AddLobbyRequestListener(m_lobbyRequestListener);
            WarpClient.GetInstance().AddNotificationListener(m_notifyListener);
            WarpClient.GetInstance().AddRoomRequestListener(m_roomRequestListener);
            WarpClient.GetInstance().AddZoneRequestListener(m_zoneRequestListener);
        }

        private void Start_Quick_Match(object sender, RoutedEventArgs e)
        {
            WarpClient.GetInstance().Connect(username, "WP8");
        }

        private void Start_Quick_Offline_Match(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new Uri("/OptionsPage.xaml", UriKind.Relative));
        }

        private void initializeAppWarp()
        {
            WarpClient.initialize(AppWarpConstants.APPWARP_APP_KEY, AppWarpConstants.APPWARP_HOST_ADDRESS);

            // We pass an instance of MainPage so that these listeners can change properties of the page, like show chats or other game status
            m_chatRequestListener = new MyChatRequestListener(this);
            m_connectionRequestListener = new MyConnectionRequestListener(this);
            m_lobbyRequestListener = new MyLobbyRequestListener(this);
            m_notifyListener = new MyNotifyListener(this);
            m_roomRequestListener = new MyRoomRequestListener(this);
            m_zoneRequestListener = new MyZoneRequestListener(this);
        }

        private class MyChatRequestListener : com.shephertz.app42.gaming.multiplayer.client.listener.ChatRequestListener
        {
            private MainPage _page;

            public MyChatRequestListener(MainPage result)
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
            private MainPage _page;

            public MyConnectionRequestListener(MainPage result)
            {
                _page = result;
            }

            public void onConnectDone(ConnectEvent eventObj)
            {
                Debug.WriteLine("onConnectDone");

                WarpClient.GetInstance().JoinRoomInRange(0, 7, true);
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
            private MainPage _page;

            public MyLobbyRequestListener(MainPage result)
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
            private MainPage _page;

            public MyNotifyListener(MainPage page)
            {
                _page = page;
            }

            public void onChatReceived(ChatEvent eventObj)
            {
                Debug.WriteLine("onChatReceived");
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

                if (eventObj != null)
                {
                    WarpClient.GetInstance().RemoveChatRequestListener(_page.m_chatRequestListener);
                    WarpClient.GetInstance().RemoveConnectionRequestListener(_page.m_connectionRequestListener);
                    WarpClient.GetInstance().RemoveLobbyRequestListener(_page.m_lobbyRequestListener);
                    WarpClient.GetInstance().RemoveNotificationListener(_page.m_notifyListener);
                    WarpClient.GetInstance().RemoveRoomRequestListener(_page.m_roomRequestListener);
                    WarpClient.GetInstance().RemoveZoneRequestListener(_page.m_zoneRequestListener);

                    string joinedRoomId = eventObj.getId();

                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        _page.NavigationService.Navigate(new Uri("/GamePage.xaml?joinedRoomId=" + joinedRoomId + "&username=" + username, UriKind.Relative));
                    });
                }
            }

            public void onUserLeftLobby(LobbyData eventObj, string username)
            {
                Debug.WriteLine("onUserLeftLobby");
            }

            public void onUserLeftRoom(RoomData eventObj, string username)
            {
                Debug.WriteLine("onUserLeftRoom");
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
            private MainPage _page;

            public MyRoomRequestListener(MainPage page)
            {
                _page = page;
            }

            public void onGetLiveRoomInfoDone(LiveRoomInfoEvent eventObj)
            {
                Debug.WriteLine("onGetLiveRoomInfoDone");
            }

            public void onInvokeRoomRPCDone(RPCEvent rpcEvent)
            {
                Debug.WriteLine("onInvokeRoomRPCDone");
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
            private MainPage _page;

            public MyZoneRequestListener(MainPage page)
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

            public void onInvokeZoneRPCDone(RPCEvent rpcEvent)
            {
                Debug.WriteLine("onInvokeZoneRPCDone");
            }

            public void onSetCustomUserDataDone(LiveUserInfoEvent eventObj)
            {
                Debug.WriteLine("onSetCustomUserDataDone");
            }
        }
    }
}