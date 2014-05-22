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

                m_d3dInterop.onChatReceived("{\"playerIndex0\":\"Player_Offline\",\"numPlayers\":1,\"breakableBlockPowerUpFlags\":\"4,1,1,0,0,4,0,0,3,4,0,4,0,3,0,3,4,4,4,0,0,2,4,4,0,1,3,4,2,0,1,2,4,0,3,4,0,3,1,0,3,4,0,2,0,0,4,3,3,1,1,4,3,3,4,4,3,2,4,0,0,1,1,0,4,3,0,3,0,0,0,0,4,4,0,2,1,0,1,1,0,4,0,4,1,2,2,2,2,2,4,4,0,4,1,1,0,0,1,0,3,3,0,4,0,0,0,3,0,0,0,2,3,2,3,0,0,0,4,4,1,0,4,2,0,4,3,1,0,3,3,0,1,0,0,1,4,4\",\"breakableBlockYValues\":\"0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,8,8,8,8,8,8,8,8,8,9,9,9,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16\",\"breakableBlockXValues\":\"2,3,4,6,8,9,10,15,16,2,8,10,14,16,0,1,3,4,5,6,9,12,14,15,16,17,0,2,4,6,8,10,12,16,1,4,5,6,7,8,9,10,12,13,14,15,17,0,4,6,8,10,12,16,1,5,6,8,10,13,14,16,17,6,10,12,16,2,5,7,8,10,12,14,15,16,2,8,16,2,5,6,9,12,14,16,4,10,16,0,1,2,4,5,6,7,8,11,12,13,14,15,2,6,8,10,12,16,0,1,2,5,6,7,8,9,10,12,14,15,16,18,2,6,8,10,12,14,16,2,3,5,6,7,9,14,15,16\",\"eventType\":1337,\"playerIndex0X\":3.747559070587158,\"numBreakableBlocks\":138,\"playerIndex0Direction\":0,\"playerIndex0Y\":3.418919086456299}");
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