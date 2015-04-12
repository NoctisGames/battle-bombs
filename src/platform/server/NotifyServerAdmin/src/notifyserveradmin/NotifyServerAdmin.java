package notifyserveradmin;

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
public final class NotifyServerAdmin
{
    private static Properties mailServerProperties;
    private static Session getMailSession;
    private static MimeMessage generateMailMessage;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        generateAndSendEmail();
    }

    private static void generateAndSendEmail()
    {
        try
        {
            System.out.println("Sending email to notify Admin of Battle Bombs server termination...");

            mailServerProperties = System.getProperties();
            mailServerProperties.put("mail.smtp.port", "587");
            mailServerProperties.put("mail.smtp.auth", "true");
            mailServerProperties.put("mail.smtp.starttls.enable", "true");

            getMailSession = Session.getDefaultInstance(mailServerProperties, null);
            generateMailMessage = new MimeMessage(getMailSession);
            generateMailMessage.addRecipient(Message.RecipientType.TO, new InternetAddress("dev.sgowen+battlebombs@gmail.com"));
            generateMailMessage.setSubject("Battle Bombs Server has Stopped!");
            generateMailMessage.setContent("You should go check on that...", "text/html");

            Transport transport = getMailSession.getTransport("smtp");

            transport.connect("smtp.gmail.com", "dev.sgowen@gmail.com", "Loz-lttp90");
            transport.sendMessage(generateMailMessage, generateMailMessage.getAllRecipients());
            transport.close();

            System.out.println("Email sent!");
        }
        catch (MessagingException e)
        {
            System.err.println(e.toString());
        }
    }
}