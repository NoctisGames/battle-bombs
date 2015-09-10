package com.gowengamedev.battlebombsserver;

import com.shephertz.app42.server.AppWarpServer;

/**
 *
 * @author sgowen
 */
public final class Main
{
    public static String NATIVE_LIBRARY_LOCATION = "";
    
    public static void main(String[] args)
    {
        if (args != null && args.length > 0)
        {
            NATIVE_LIBRARY_LOCATION = args[0];
        }
        
        String appconfigPath = System.getProperty("user.dir") + System.getProperty("file.separator") + "AppConfig.json";

        boolean started = AppWarpServer.start(new BbServerAdaptor(), appconfigPath);

        if (started)
        {
            System.out.println("Server started! Unix Timestamp: " + System.currentTimeMillis());
        }
    }
}