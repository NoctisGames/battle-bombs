using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.IsolatedStorage;

namespace BattleBombs
{
    public class SaveData
    {
        private static String USERNAME = "USERNAME";

        public static String getUsername()
        {
            IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;
            if (settings.Contains(USERNAME))
            {
                String username = settings[USERNAME] as String;
                return username;
            }

            return "";
        }

        public static void saveUsername(String username)
        {
            IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;
            if (settings.Contains(USERNAME))
            {
                settings.Remove(USERNAME);
            }

            settings.Add(USERNAME, username);
            settings.Save();
        }
    }
}