package com.technegames.battlebombs;

import android.app.Application;

import com.technegames.battlebombs.json.JsonDeserializer;
import com.technegames.battlebombs.json.JsonSerializer;
import com.technegames.battlebombs.json.TgJsonDeserializer;
import com.technegames.battlebombs.json.TgJsonSerializer;

public final class BattleBombsApplication extends Application
{
    @Override
    public void onCreate()
    {
        Logger.initializeLoggingLevel("DEBUG");

        InstanceManager.getInstance().storeInstance(JsonSerializer.class, new TgJsonSerializer());
        InstanceManager.getInstance().storeInstance(JsonDeserializer.class, new TgJsonDeserializer());
    }
}