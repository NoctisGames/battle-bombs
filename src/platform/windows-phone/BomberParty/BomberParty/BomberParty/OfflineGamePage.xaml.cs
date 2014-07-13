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
    public partial class OfflineGamePage : PhoneApplicationPage
    {
        private Direct3DInterop m_d3dInterop = null;

        private int numberOfBots;
        private bool firepowerEnabled;
        private bool bombsEnabled;
        private bool speedEnabled;
        private bool kickEnabled;
        private bool throwEnabled;
        private string mapSelection;

        public OfflineGamePage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;

            string str_numberOfBots;
            string str_firepowerEnabled;
            string str_bombsEnabled;
            string str_speedEnabled;
            string str_kickEnabled;
            string str_throwEnabled;
            string str_mapSelection;

            if (NavigationContext.QueryString.TryGetValue("numberOfBots", out str_numberOfBots))
            {
                numberOfBots = Convert.ToInt32(str_numberOfBots);
            }

            if (NavigationContext.QueryString.TryGetValue("firepowerEnabled", out str_firepowerEnabled))
            {
                firepowerEnabled = Convert.ToBoolean(str_firepowerEnabled);
            }

            if (NavigationContext.QueryString.TryGetValue("bombsEnabled", out str_bombsEnabled))
            {
                bombsEnabled = Convert.ToBoolean(str_bombsEnabled);
            }

            if (NavigationContext.QueryString.TryGetValue("speedEnabled", out str_speedEnabled))
            {
                speedEnabled = Convert.ToBoolean(str_speedEnabled);
            }

            if (NavigationContext.QueryString.TryGetValue("kickEnabled", out str_kickEnabled))
            {
                kickEnabled = Convert.ToBoolean(str_kickEnabled);
            }

            if (NavigationContext.QueryString.TryGetValue("throwEnabled", out str_throwEnabled))
            {
                throwEnabled = Convert.ToBoolean(str_throwEnabled);
            }

            if (NavigationContext.QueryString.TryGetValue("mapSelection", out str_mapSelection))
            {
                mapSelection = str_mapSelection;
            }
        }

        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);

            //if (m_d3dinterop.onbackpressed())
            //{
            //    e.cancel = true;
            //}
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
                DrawingSurface.SetContentProvider(m_d3dInterop.CreateContentProvider(numberOfBots, "Player_Offline"));
                DrawingSurface.SetManipulationHandler(m_d3dInterop);

                m_d3dInterop.setWinRtCallback(new WinRtCallback(ProcessCallback));

                m_d3dInterop.onChatReceived("{\"breakableBlockPowerUpFlags\": \"3,1,0,1,0,4,4,1,0,4,1,0,3,2,1,1,0,3,0,2,0,4,1,0,0,0,0,3,2,4,3,3,1,0,2,4,3,4,4,4,0,1,4,2,3,3,4,2,0,4,0,1,2,1,4,3,2,1,4,4,0,4,0,4,3,1,0,0,4,4,0,0,4,3,3,0,0,0,0,1,2,0,3,2,0,1,0,4,4,4,3,0,4,4,0,0,4,4,0,2,2,0,2,0,4\", \"breakableBlockXValues\": \"8,4,5,7,8,9,10,5,7,11,0,1,3,4,5,7,10,11,12,14,2,6,8,10,14,0,2,5,6,7,9,10,11,12,14,2,4,6,8,12,2,3,7,10,12,6,8,10,12,2,4,6,8,12,2,6,7,8,9,10,12,2,4,6,10,0,1,2,3,4,5,8,9,10,11,12,14,0,4,6,8,10,12,14,1,2,4,5,7,8,9,10,12,13,2,4,6,7,10,4,5,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 105, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"Player_Offline\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Bot 1\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Bot 3\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Bot 4\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Bot 5\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Bot 6\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Bot 7\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}");
            }
        }

        private void ProcessCallback(String command, String param)
        {
            Console.WriteLine("Incoming callback from C++ : " + command);
        }
    }
}