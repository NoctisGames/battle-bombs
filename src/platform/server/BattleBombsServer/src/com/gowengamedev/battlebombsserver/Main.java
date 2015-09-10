package com.gowengamedev.battlebombsserver;

import com.shephertz.app42.server.AppWarpServer;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Properties;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

/**
 *
 * @author sgowen
 */
public final class Main
{
    public static String NATIVE_LIBRARY_LOCATION = ""; // Temporary solution, will need to think of something better if/when I release version 1.4.0

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