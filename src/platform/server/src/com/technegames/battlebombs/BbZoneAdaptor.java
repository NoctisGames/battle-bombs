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
    }
}