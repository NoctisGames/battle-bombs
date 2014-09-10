package com.technegames.battlebombs.api;

import com.google.gson.annotations.SerializedName;

public final class ServerStatusResponse
{
    @SerializedName("isCurrentVersion")
    public boolean isCurrentVersion;

    @SerializedName("isDownForMaintenance")
    public boolean isDownForMaintenance;
}