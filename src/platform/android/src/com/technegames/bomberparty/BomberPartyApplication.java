package com.technegames.bomberparty;

import android.app.Application;

public final class BomberPartyApplication extends Application
{
	@Override
	public void onCreate()
	{
		Logger.initializeLoggingLevel("DEBUG");
	}
}