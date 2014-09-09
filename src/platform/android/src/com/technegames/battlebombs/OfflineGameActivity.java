package com.technegames.battlebombs;

import android.app.Activity;
import android.content.Intent;

public final class OfflineGameActivity extends BaseGameActivity
{
    public static void startActivity(Activity activity, String username)
    {
        Intent i = new Intent(activity, OfflineGameActivity.class);
        i.putExtra(EXTRA_USERNAME, username);
        activity.startActivity(i);
    }

    @Override
    protected boolean isOffline()
    {
        return true;
    }
}