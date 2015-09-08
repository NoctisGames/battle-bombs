package com.gowengamedev.battlebombsserver;

import com.shephertz.app42.server.AppWarpServer;

/**
 *
 * @author sgowen
 */
public final class Main
{
    public static void main(String[] args)
    {
        String appconfigPath = System.getProperty("user.dir") + System.getProperty("file.separator") + "AppConfig.json";

        boolean started = AppWarpServer.start(new BbServerAdaptor(), appconfigPath);

        if (started)
        {
            System.out.println("Server started! Unix Timestamp: " + System.currentTimeMillis());
        }
    }
}