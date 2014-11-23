using System;
using System.ComponentModel;
using System.Threading;
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
        public static Boolean isConnectionError = false;

        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Enabled;

            if (isConnectionError)
            {
                showConnectionErrorMessageAfterDelay(1000);
            }

            isConnectionError = false;
        }

        private void Start_Quick_Match(object sender, RoutedEventArgs e)
        {
            SystemTray.ProgressIndicator = new ProgressIndicator();
            SystemTray.ProgressIndicator.Text = "One Moment...";
            SystemTray.ProgressIndicator.IsIndeterminate = true;
            SystemTray.ProgressIndicator.IsVisible = true;

            WebClient webClient = new WebClient();
            webClient.DownloadStringCompleted += webClient_DownloadStringCompleted;
            webClient.DownloadStringAsync(new Uri(ApiConstants.SERVER_STATUS_URL));
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
                    MessageBox.Show("Your device is unable to connect to Battle Bombs. Please check your connection and try again.");
                }
                else
                {
                    ServerStatusResponse serverStatusResponse = JsonConvert.DeserializeObject<ServerStatusResponse>(e.Result);
                    if (serverStatusResponse.isCurrentVersion)
                    {
                        if (serverStatusResponse.isDownForMaintenance)
                        {
                            MessageBox.Show("Battle Bombs is down for maintenance, but will be back online soon.\nFeel free to enjoy the offline mode in the meantime.");
                        }
                        else
                        {
                            showEnterPlayerNameDialog(true);
                        }
                    }
                    else
                    {
                        MessageBox.Show("A new version of Battle Bombs is available, please update before playing online.");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Your device is unable to connect to Battle Bombs. Please check your connection and try again: " + ex.ToString());
            }
        }

        private void showEnterPlayerNameDialog(Boolean forOnlinePlay)
        {
            TextBox tb = new TextBox();
            String previousUsername = SaveData.getUsername();
            tb.Text = previousUsername;
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
                    SaveData.saveUsername(username);
                    if (username.Length >= 3 && username.Length <= 12)
                    {
                        Deployment.Current.Dispatcher.BeginInvoke(() =>
                        {
                            NavigationService.Navigate(new Uri((forOnlinePlay ? "/GamePage.xaml?username=" : "/OfflineGamePage.xaml?username=") + username, UriKind.Relative));
                        });
                    }
                    else
                    {
                        MessageBox.Show("Please enter a username between 3 and 12 characters.");
                    }
                }
            };
            box.Show();
        }

        private void showConnectionErrorMessageAfterDelay(int delayInMilliseconds)
        {
            BackgroundWorker worker = new BackgroundWorker();

            worker.DoWork += (s, e) => Thread.Sleep(delayInMilliseconds);

            worker.RunWorkerCompleted += (s, e) => Deployment.Current.Dispatcher.BeginInvoke(() =>
            {
                MessageBox.Show("Your device is unable to connect to Battle Bombs. Please check your connection and try again.");
            });

            worker.RunWorkerAsync();
        }
    }

    public class ServerStatusResponse
    {
        public bool isCurrentVersion { get; set; }

        public bool isDownForMaintenance { get; set; }
    }
}