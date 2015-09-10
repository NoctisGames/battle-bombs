using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.Diagnostics;
using com.shephertz.app42.gaming.multiplayer.client;
using com.shephertz.app42.gaming.multiplayer.client.events;
using Coding4Fun.Toolkit.Controls;
using BattleBombsComp;

namespace BattleBombs
{
    public partial class GamePage : PhoneApplicationPage
    {
        private static String EVENT_TYPE = "eventType";
        private static String PHASE = "phase";

        // Definitions from src/core/GameEvent.h
        // Event Type
        private static int PRE_GAME = 1334;

        // Pre Game Phases
        private static int CONNECTING = 1;
        private static int FINDING_ROOM_TO_JOIN = 2;
        private static int ROOM_JOINED_WAITING_FOR_SERVER = 3;
        private static int CONNECTION_ERROR = 4;
        private static int BATTLE_BOMBS_BETA_CLOSED = 5;

        static GamePage()
        {
            WarpClient.initialize(AppWarpConstants.APPWARP_APP_KEY, AppWarpConstants.APPWARP_HOST_ADDRESS);
        }

        private com.shephertz.app42.gaming.multiplayer.client.listener.ConnectionRequestListener m_connectionRequestListener;
        private com.shephertz.app42.gaming.multiplayer.client.listener.NotifyListener m_notifyListener;

        private Direct3DInterop m_d3dInterop = null;
        private string m_joinedRoomId;
        private string m_username;

        public GamePage()
        {
            InitializeComponent();

            m_connectionRequestListener = new MyConnectionRequestListener(this);
            m_notifyListener = new MyNotifyListener(this);
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;

            if (NavigationContext.QueryString.TryGetValue("username", out m_username))
            {
                Console.WriteLine("m_username = " + m_username);
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);

            WarpClient.GetInstance().RemoveConnectionRequestListener(m_connectionRequestListener);
            WarpClient.GetInstance().RemoveNotificationListener(m_notifyListener);

            WarpClient.GetInstance().LeaveRoom(m_joinedRoomId);
            WarpClient.GetInstance().Disconnect();
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
                DrawingSurface.SetContentProvider(m_d3dInterop.CreateContentProvider(m_username, false));
                DrawingSurface.SetManipulationHandler(m_d3dInterop);

                m_d3dInterop.setWinRtCallback(new WinRtCallback(ProcessCallback));

                string preGameUpdate = "{\"" + EVENT_TYPE + "\":" + PRE_GAME + ",\"" + PHASE + "\":" + CONNECTING + "}";

                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    m_d3dInterop.onChatReceived(preGameUpdate);

                    WarpClient.GetInstance().AddConnectionRequestListener(m_connectionRequestListener);
                    WarpClient.GetInstance().AddNotificationListener(m_notifyListener);

                    WarpClient.GetInstance().Connect(m_username, AppWarpConstants.APPWARP_AUTH_DATA);
                });
            }
        }

        private void ProcessCallback(String command, String param)
        {
            if (command.Equals("SEND_CHAT"))
            {
                WarpClient.GetInstance().SendChat(param);
            }
            else if (command.Equals("EXIT"))
            {
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    NavigationService.GoBack();
                });
            }
            else if (command.Equals("CONNECTION_ERROR"))
            {
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    MainPage.isConnectionError = true;
                    NavigationService.GoBack();
                });
            }
            else if (command.Equals("DISPLAY_TOAST"))
            {
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    ToastPrompt toast = new ToastPrompt();
                    toast.FontSize = 30;
                    toast.Title = param;
                    toast.TextOrientation = System.Windows.Controls.Orientation.Horizontal;

                    toast.Show();
                });
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

                if (eventObj.getResult() == 0)
                {
                    string preGameUpdate = "{\"" + EVENT_TYPE + "\":" + PRE_GAME + ",\"" + PHASE + "\":" + FINDING_ROOM_TO_JOIN + "}";

                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        _page.m_d3dInterop.onChatReceived(preGameUpdate);

                        WarpClient.GetInstance().JoinRoomInRange(0, 7, true);
                    });
                }
                else if (eventObj.getResult() == 2)
                {
                    string preGameUpdate = "{\"" + EVENT_TYPE + "\":" + PRE_GAME + ",\"" + PHASE + "\":" + BATTLE_BOMBS_BETA_CLOSED + "}";

                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        _page.m_d3dInterop.onChatReceived(preGameUpdate);
                    });
                }
                else
                {
                    string preGameUpdate = "{\"" + EVENT_TYPE + "\":" + PRE_GAME + ",\"" + PHASE + "\":" + CONNECTION_ERROR + "}";

                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        _page.m_d3dInterop.onChatReceived(preGameUpdate);
                    });
                }
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

                if (eventObj != null && _page.m_username.Equals(username))
                {
                    _page.m_joinedRoomId = eventObj.getId();

                    string preGameUpdate = "{\"" + EVENT_TYPE + "\":" + PRE_GAME + ",\"" + PHASE + "\":" + ROOM_JOINED_WAITING_FOR_SERVER + "}";

                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        _page.m_d3dInterop.onChatReceived(preGameUpdate);
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
    }
}