using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using System.Windows.Input;
using Microsoft.Phone.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using Newtonsoft.Json;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;
using System.Diagnostics;
using System.IO.IsolatedStorage;
using com.shephertz.app42.gaming.multiplayer.client;
using com.shephertz.app42.gaming.multiplayer.client.events;

namespace BattleBombs
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Enabled;
        }

        private void Start_Quick_Match(object sender, RoutedEventArgs e)
        {
            SystemTray.ProgressIndicator = new ProgressIndicator();
            SystemTray.ProgressIndicator.Text = "One Moment...";
            SystemTray.ProgressIndicator.IsIndeterminate = true;
            SystemTray.ProgressIndicator.IsVisible = true;

            WebClient webClient = new WebClient();
            webClient.DownloadStringCompleted += webClient_DownloadStringCompleted;
            webClient.DownloadStringAsync(new Uri("http://technegames.com/techne-games/bb_01.json"));
        }

        private void Start_Quick_Offline_Match(object sender, RoutedEventArgs e)
        {
            showEnterPlayerNameDialog(false);
        }

        private void webClient_DownloadStringCompleted(object sender, DownloadStringCompletedEventArgs e)
        {
            SystemTray.ProgressIndicator.IsIndeterminate = true;
            SystemTray.ProgressIndicator.IsVisible = true;

            try
            {
                if (string.IsNullOrEmpty(e.Result))
                {
                    MessageBox.Show("Your device is unable to connect to Techne Games. Please check your connection and try again.");
                }
                else
                {
                    IsCurrentVersionResponse isCurrentVersionResponse = JsonConvert.DeserializeObject<IsCurrentVersionResponse>(e.Result);
                    if (isCurrentVersionResponse.isCurrentVersion)
                    {
                        showEnterPlayerNameDialog(true);
                    }
                    else
                    {
                        MessageBox.Show("A new version of Battle Bombs is available, please update before playing online.");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Your device is unable to connect to Techne Games. Please check your connection and try again: " + ex.ToString());
            }
        }

        private void showEnterPlayerNameDialog(Boolean forOnlinePlay)
        {
            var tb = new TextBox();
            var box = new CustomMessageBox()
            {
                Caption = "Player Name",
                Message = "Enter your Battle Bombs name",
                LeftButtonContent = forOnlinePlay ? "Connect" : "Play",
                RightButtonContent = "Cancel",
                Content = tb,
                IsFullScreen = false
            };
            box.Dismissed += (s, e) =>
            {
                if (e.Result == CustomMessageBoxResult.LeftButton)
                {
                    var username = tb.Text;
                    if (username.Length >= 3 && username.Length <= 15)
                    {
                        NavigationService.Navigate(new Uri((forOnlinePlay ? "/GamePage.xaml?username=" : "/OfflineGamePage.xaml?username=") + username, UriKind.Relative));
                    }
                    else
                    {
                        MessageBox.Show("Please enter a username between 3 and 15 characters.");
                    }
                }
            };
            box.Show();
        }
    }

    public class IsCurrentVersionResponse
    {
        public bool isCurrentVersion { get; set; }
    }
}