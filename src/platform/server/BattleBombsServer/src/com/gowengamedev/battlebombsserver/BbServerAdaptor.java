package com.gowengamedev.battlebombsserver;

import com.shephertz.app42.server.idomain.BaseServerAdaptor;
import com.shephertz.app42.server.idomain.IZone;

/**
 *
 * @author sgowen
 */
public final class BbServerAdaptor extends BaseServerAdaptor
{
    @Override
    public void onZoneCreated(IZone zone)
    {
        System.out.println("Zone Created " + zone.getName() + " with key " + zone.getAppKey());

        if (zone.getName().equals("Battle Bombs"))
        {
            zone.setAdaptor(new Bb131ZoneAdaptor(zone));
        }
        else if (zone.getName().equals("BB_1.4.0"))
        {
            // TODO, next version will get its own zone so that I don't have to maintain multiple servers for multiple versions
        }
    }
}