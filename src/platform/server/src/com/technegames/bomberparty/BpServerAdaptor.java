package com.technegames.bomberparty;

import com.shephertz.app42.server.idomain.BaseServerAdaptor;
import com.shephertz.app42.server.idomain.IZone;

/**
 *
 * @author sgowen
 */
public final class BpServerAdaptor extends BaseServerAdaptor
{
    @Override
    public void onZoneCreated(IZone zone)
    {
        System.out.println("Zone Created " + zone.getName() + " with key " + zone.getAppKey());
        zone.setAdaptor(new BpZoneAdaptor());
    }
}
