package com.technegames.battlebombs;

import com.shephertz.app42.server.idomain.BaseZoneAdaptor;
import com.shephertz.app42.server.idomain.HandlingResult;
import com.shephertz.app42.server.idomain.IRoom;
import com.shephertz.app42.server.idomain.IUser;
import com.shephertz.app42.server.idomain.IZone;

/**
 *
 * @author sgowen
 */
public final class BbZoneAdaptor extends BaseZoneAdaptor
{
    private final IZone _zone;

    public BbZoneAdaptor(IZone zone)
    {
        _zone = zone;

        System.out.println("Creating Adapter for Zone: " + _zone.getName());
    }

    @Override
    public void onAdminRoomAdded(IRoom room)
    {
        System.out.println("Room Created " + room.getName() + " with ID " + room.getId());
        room.setAdaptor(new BbRoomAdaptor(room));
    }

    @Override
    public void handleAddUserRequest(IUser user, String authString, HandlingResult result)
    {
        System.out.println("UserRequest " + user.getName());

        if (!authString.equals(Globals.CURRENT_AUTH_DATA))
        {
            if (Globals.IS_BATTLE_BOMBS_BETA_CLOSED)
            {
                result.code = 2; // Not Successful
                result.sendNotification = false;
                result.sendResponse = true;
                result.description = "The Battle Bombs Beta is closed. Please go to the app store to download the official Battle Bombs!";
            }
            else
            {
                result.code = 1; // Not Successful
                result.sendNotification = false;
                result.sendResponse = true;
                result.description = "A new version of Battle Bombs is available, please update before playing online.";
            }
        }
    }
}