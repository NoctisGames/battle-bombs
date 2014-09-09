package com.technegames.battlebombs;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

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

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        _rendererWrapper.onChatReceived("{\"breakableBlockPowerUpFlags\": \"3,1,0,1,0,4,4,1,0,4,1,0,3,2,1,1,0,3,0,2,0,4,1,0,0,0,0,3,2,4,3,3,1,0,2,4,3,4,4,4,0,1,4,2,3,3,4,2,0,4,0,1,2,1,4,3,2,1,4,4,0,4,0,4,3,1,0,0,4,4,0,0,4,3,3,0,0,0,0,1,2,0,3,2,0,1,0,4,4,4,3,0,4,4,0,0,4,4,0,2,2,0,2,0,4\", \"breakableBlockXValues\": \"8,4,5,7,8,9,10,5,7,11,0,1,3,4,5,7,10,11,12,14,2,6,8,10,14,0,2,5,6,7,9,10,11,12,14,2,4,6,8,12,2,3,7,10,12,6,8,10,12,2,4,6,8,12,2,6,7,8,9,10,12,2,4,6,10,0,1,2,3,4,5,8,9,10,11,12,14,0,4,6,8,10,12,14,1,2,4,5,7,8,9,10,12,13,2,4,6,7,10,4,5,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 105, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"Player_Offline\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Bot 1\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Bot 3\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Bot 4\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Bot 5\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Bot 6\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Bot 7\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}");
    }
}