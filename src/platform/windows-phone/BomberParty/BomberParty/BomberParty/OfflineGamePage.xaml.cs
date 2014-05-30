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

                m_d3dInterop.onChatReceived("{\"breakableBlockPowerUpFlags\":\"2,1,3,1,1,3,3,4,0,4,3,0,0,2,0,0,2,4,2,4,4,2,0,0,0,1,0,1,3,2,3,1,2,3,0,1,1,0,4,0,3,0,0,2,2,2,4,4,1,4,0,1,0,0,0,0,0,0,4,1,0,0,3,0,0,1,1,4,1,0,1,1,0,1,1,1,2,2,4,0,0,1,0,0,1,0,2,1,0,0,2,1,0,1,4,0,0,3,0,2,0,0,0,0,4,3,0,0,3,2,1,0,4,1,0,3,0,0,4,3,2,2,0,0,1,1,4,3,2,1,0,0,1,0,1,2,3,0,0,1,1,0,4,0,3\",\"breakableBlockXValues\":\"2,3,5,6,7,8,9,10,13,15,16,2,8,10,12,0,1,3,6,7,9,14,16,17,4,6,8,0,1,3,4,5,7,10,11,14,15,17,18,0,2,4,12,14,18,0,1,2,3,6,8,10,11,12,13,15,16,17,18,2,4,6,12,14,16,4,5,6,7,8,9,11,12,14,15,2,4,6,10,12,16,2,3,5,6,7,8,9,11,14,15,4,6,8,1,2,4,5,6,9,10,11,13,16,17,2,6,8,10,12,14,16,18,0,1,2,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,6,10,16,2,4,5,6,7,8,10,11,13,15,16\",\"breakableBlockYValues\":\"0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16,16,16,16,16,16\",\"eventType\":1337,\"numBreakableBlocks\":145,\"numPlayers\":1,\"numClientBots\":7,\"playerIndex0\":\"Player_Offline\",\"playerIndex0Direction\":0,\"playerIndex0X\":3.747559070587158,\"playerIndex0Y\":3.418919086456299,\"playerIndex1\":\"Bot1\",\"playerIndex1Direction\":2,\"playerIndex1X\":22.08089256286621,\"playerIndex1Y\":13.41891860961914,\"playerIndex2\":\"Bot2\",\"playerIndex2Direction\":0,\"playerIndex2X\":3.747559070587158,\"playerIndex2Y\":15.91891860961914,\"playerIndex3\":\"Bot3\",\"playerIndex3Direction\":2,\"playerIndex3X\":22.08089256286621,\"playerIndex3Y\":24.16891860961914,\"playerIndex4\":\"Bot4\",\"playerIndex4Direction\":1,\"playerIndex4X\":22.08089256286621,\"playerIndex4Y\":3.418919086456299,\"playerIndex5\":\"Bot5\",\"playerIndex5Direction\":1,\"playerIndex5X\":22.08089256286621,\"playerIndex5Y\":15.91891860961914,\"playerIndex6\":\"Bot6\",\"playerIndex6Direction\":3,\"playerIndex6X\":3.747559070587158,\"playerIndex6Y\":13.41891860961914,\"playerIndex7\":\"Bot7\",\"playerIndex7Direction\":3,\"playerIndex7X\":3.747559070587158,\"playerIndex7Y\":24.16891860961914}");
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
    }
}