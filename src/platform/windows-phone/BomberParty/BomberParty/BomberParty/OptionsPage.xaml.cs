using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace BomberParty
{
    public partial class Page1 : PhoneApplicationPage
    {
        private int numberOfBots = 0;
        private string mapSelection = "Grasslands";
        private bool firepowerEnabled = true;
        private bool bombsEnabled = true;
        private bool speedEnabled = true;
        private bool kickEnabled = true;
        private bool throwEnabled = true;

        public Page1()
        {
            InitializeComponent();
            checkFirepower.IsChecked = true;
            checkBombs.IsChecked = true;
            checkSpeed.IsChecked = true;
            checkKick.IsChecked = true;
            checkThrow.IsChecked = true;
            mapList.SelectedIndex = 0;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
           numberOfBots = Convert.ToInt32(txtSliderValue.Text);
           firepowerEnabled = Convert.ToBoolean(checkFirepower.IsChecked);
           bombsEnabled = Convert.ToBoolean(checkBombs.IsChecked);
           speedEnabled = Convert.ToBoolean(checkSpeed.IsChecked);
           kickEnabled = Convert.ToBoolean(checkKick.IsChecked);
           throwEnabled = Convert.ToBoolean(checkThrow.IsChecked);
           mapSelection = Convert.ToString(mapList.SelectedItem);

           NavigationService.Navigate(new Uri("/OfflineGamePage.xaml?numberOfBots=" + numberOfBots
                                              + "&firepoewrEnabled" + firepowerEnabled
                                              + "&bombsEnabled" + bombsEnabled
                                              + "&speedEnabled" + speedEnabled
                                              + "&kickEnabled" + kickEnabled
                                              + "&throwEnabled" + throwEnabled
                                              + "&mapSelection" + mapSelection, 
                                              UriKind.Relative));
        }

        private void BotSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            double roundedValue = Math.Round(BotSlider.Value, 0);
            //Check if it is a rounded value
            if (BotSlider.Value != roundedValue)
                BotSlider.Value = roundedValue;
        }
    }
}