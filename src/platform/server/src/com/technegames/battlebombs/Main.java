package com.technegames.battlebombs;

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
    private static Properties mailServerProperties;
    private static Session getMailSession;
    private static MimeMessage generateMailMessage;

    public static void main(String[] args)
    {
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler()
        {
            @Override
            public void uncaughtException(Thread t, Throwable e)
            {
                generateAndSendEmailInResponseToUncaughtException(e);
            }
        });

        String appconfigPath = System.getProperty("user.dir") + System.getProperty("file.separator") + "AppConfig.json";

        boolean started = AppWarpServer.start(new BbServerAdaptor(), appconfigPath);

        if (started)
        {
            System.out.println("Server started!");
        }
    }

    private static void generateAndSendEmailInResponseToUncaughtException(Throwable t)
    {
        try
        {
            mailServerProperties = System.getProperties();
            mailServerProperties.put("mail.smtp.port", "587");
            mailServerProperties.put("mail.smtp.auth", "true");
            mailServerProperties.put("mail.smtp.starttls.enable", "true");

            getMailSession = Session.getDefaultInstance(mailServerProperties, null);
            generateMailMessage = new MimeMessage(getMailSession);
            generateMailMessage.addRecipient(Message.RecipientType.TO, new InternetAddress("dev.sgowen+battlebombs@gmail.com"));
            generateMailMessage.setSubject("Battle Bombs Uncaught Exception!");
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            t.printStackTrace(pw);
            String emailBody = sw.toString(); // stack trace as a string
            generateMailMessage.setContent(emailBody, "text/html");

            Transport transport = getMailSession.getTransport("smtp");

            transport.connect("smtp.gmail.com", "dev.sgowen@gmail.com", "Loz-lttp90");
            transport.sendMessage(generateMailMessage, generateMailMessage.getAllRecipients());
            transport.close();
        }
        catch (MessagingException e)
        {
            System.err.println(e.toString());
        }
    }
}