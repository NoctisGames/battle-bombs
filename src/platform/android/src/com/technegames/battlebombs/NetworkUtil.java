package com.technegames.battlebombs;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

public enum NetworkUtil
{
    NOT_CONNECTED, WIFI_CONNECTED, FOUR_G_CONNECTED, THREE_G_CONNECTED;

    public static NetworkUtil getNetworkConnectivity(Context context)
    {
        final ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

        if (connectivityManager == null)
        {
            return NOT_CONNECTED;
        }

        NetworkInfo.State wifiNetwork = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState();

        android.net.NetworkInfo mobileNetwork = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
        NetworkInfo.State mobileNetworkState = mobileNetwork != null ? mobileNetwork.getState() : null;

        NetworkInfo wimaxNetwork = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIMAX);
        NetworkInfo.State wimaxNetworkState = wimaxNetwork != null ? wimaxNetwork.getState() : null;

        if (wifiNetwork != null && (wifiNetwork == NetworkInfo.State.CONNECTED || wifiNetwork == NetworkInfo.State.CONNECTING))
        {
            return WIFI_CONNECTED;
        }

        if (mobileNetwork != null && (mobileNetworkState == NetworkInfo.State.CONNECTED || mobileNetworkState == NetworkInfo.State.CONNECTING))
        {
            return THREE_G_CONNECTED;
        }

        if (wimaxNetworkState != null && (wimaxNetworkState == NetworkInfo.State.CONNECTED || wimaxNetworkState == NetworkInfo.State.CONNECTING))
        {
            return FOUR_G_CONNECTED;
        }

        return NOT_CONNECTED;
    }

    public static boolean isConnected(Context context)
    {
        NetworkUtil networkStatus = getNetworkConnectivity(context);
        return networkStatus == WIFI_CONNECTED || networkStatus == FOUR_G_CONNECTED || networkStatus == THREE_G_CONNECTED;
    }
}