package com.technegames.battlebombs;

import com.shephertz.app42.server.idomain.BaseZoneAdaptor;
import com.shephertz.app42.server.idomain.HandlingResult;
import com.shephertz.app42.server.idomain.IRoom;
import com.shephertz.app42.server.idomain.IUser;

/**
 *
 * @author sgowen
 */
public final class BpZoneAdaptor extends BaseZoneAdaptor
{
    @Override
    public void onAdminRoomAdded(IRoom room)
    {
        System.out.println("Room Created " + room.getName() + " with ID " + room.getId());
        room.setAdaptor(new BpRoomAdaptor(room));
    }

    @Override
    public void handleAddUserRequest(IUser user, String authString, HandlingResult result)
    {
        System.out.println("UserRequest " + user.getName());

        // TODO, write the authString (which is actually the user's platform: iOS, Android, or WP8 to a database or something)
    }
}