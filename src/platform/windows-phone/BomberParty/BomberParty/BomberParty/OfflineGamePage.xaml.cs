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

        public OfflineGamePage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);

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
                DrawingSurface.SetContentProvider(m_d3dInterop.CreateContentProvider("Player_Offline"));
                DrawingSurface.SetManipulationHandler(m_d3dInterop);

                m_d3dInterop.setWinRtCallback(new WinRtCallback(ProcessCallback));

                m_d3dInterop.onChatReceived("{\"breakableBlockPowerUpFlags\": \"4,4,2,3,4,0,0,0,2,4,0,0,1,1,0,2,1,2,0,1,0,0,3,2,0,4,4,0,0,1,0,0,4,1,0,0,0,0,1,4,0,0,3,1,1,3,2,3,2,1,0,4,2,0,2,3,3,1,0,3,0,0,4,1,0,0,0,4,3,0,3,1,2,0,0,2,2,0,0,4,0,1,2,4,0,0,3,1,3,3,4,4,2,0,0,4,4,2,2,1,4,4,0,0,0,1,2\", \"breakableBlockXValues\": \"8,4,5,7,7,11,0,1,2,3,4,6,7,8,9,11,12,13,14,0,6,8,10,12,14,1,2,3,4,5,6,8,10,11,12,13,14,2,4,8,10,12,4,5,6,7,9,10,12,2,6,12,2,4,10,3,4,5,6,9,10,11,2,6,10,12,0,1,2,3,4,7,8,9,11,12,13,14,0,2,6,8,12,14,0,1,2,3,5,7,9,10,12,2,4,6,8,10,12,4,5,6,7,8,9,10,12\", \"breakableBlockYValues\": \"0,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"numBreakableBlocks\": 107, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"Player_Offline\", \"playerIndex0Direction\": 0, \"playerIndex0X\": 6.447761058807373, \"playerIndex0Y\": 2.1492538452148438, \"playerIndex1\": \"Bot 1\", \"playerIndex1Direction\": 2, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Direction\": 0, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Bot 3\", \"playerIndex3Direction\": 2, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Bot 4\", \"playerIndex4Direction\": 1, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Bot 5\", \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Bot 6\", \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 16.477611541748047, \"playerIndex7\": \"Bot 7\", \"playerIndex7Direction\": 3, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 25.074626922607422}");
            }
        }

        private void ProcessCallback(String command, String param)
        {
            Console.WriteLine("Incoming callback from C++ : " + command);
        }
    }
}