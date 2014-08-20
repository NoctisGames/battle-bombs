package com.technegames.battlebombs;

import android.app.Application;

public final class BattleBombsApplication extends Application
{
    @Override
    public void onCreate()
    {
        Logger.initializeLoggingLevel("DEBUG");
    }
}