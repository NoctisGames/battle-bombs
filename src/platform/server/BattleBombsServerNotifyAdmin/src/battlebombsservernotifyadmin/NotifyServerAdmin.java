package battlebombsservernotifyadmin;

import java.util.Properties;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Date;

/**
 *
 * @author sgowen
 */
public final class NotifyServerAdmin
{
    //expressed in milliseconds
    private final static long ONCE_PER_MINUTE = 1000 * 60;

    private static Properties mailServerProperties;
    private static Session getMailSession;
    private static MimeMessage generateMailMessage;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        if (args != null && args.length > 0)
        {
            int[] pids = new int[args.length];
            for (int i = 0; i < args.length; i++)
            {
                String arg = args[i];
                pids[i] = Integer.parseInt(arg);
            }

            showAllProcessesRunningOnWindows();

            TimerTask scanPidsTimerTask = new ScanPidsTimerTask(pids);
            //perform the task once a minute starting now
            Timer timer = new Timer();
            timer.scheduleAtFixedRate(scanPidsTimerTask, new Date(), ONCE_PER_MINUTE);
        }
    }

    private static boolean isProcessIdRunningOnWindows(int pid)
    {
        //this queries tasklist if the pid passed in is running.
        //If the pid is running it returns true, else false.
        try
        {
            Runtime runtime = Runtime.getRuntime();
            String cmds[] =
            {
                "cmd", "/c", "tasklist /FI \"PID eq " + pid + "\""
            };
            Process proc = runtime.exec(cmds);

            InputStream inputstream = proc.getInputStream();
            InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
            BufferedReader bufferedreader = new BufferedReader(inputstreamreader);
            String line;
            while ((line = bufferedreader.readLine()) != null)
            {
                //Search the PID matched lines single line for the sequence: " 1300 "
                //if you find it, then the PID is still running.
                if (line.contains(" " + pid + " "))
                {
                    return true;
                }
            }

            return false;
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
            System.out.println("Cannot query the tasklist for some reason.");
            System.exit(0);
        }

        return false;
    }

    private static void showAllProcessesRunningOnWindows()
    {
        //this function prints the contents of tasklist including pid's
        try
        {
            Runtime runtime = Runtime.getRuntime();
            String cmds[] =
            {
                "cmd", "/c", "tasklist"
            };
            Process proc = runtime.exec(cmds);
            InputStream inputstream = proc.getInputStream();
            InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
            BufferedReader bufferedreader = new BufferedReader(inputstreamreader);
            String line;
            while ((line = bufferedreader.readLine()) != null)
            {
                System.out.println(line);
            }
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
            System.out.println("Cannot query the tasklist for some reason.");
        }
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

            // An email has been sent, so this program no longer needs to run
            System.exit(0);
        }
        catch (MessagingException e)
        {
            System.err.println(e.toString());
        }
    }

    private static final class ScanPidsTimerTask extends TimerTask
    {
        private final int[] pids;

        public ScanPidsTimerTask(int[] pids)
        {
            this.pids = pids;
        }

        @Override
        public void run()
        {
            System.out.println("");

            for (int pid : pids)
            {
                boolean isPidRunning = isProcessIdRunningOnWindows(pid);
                System.out.println("is PID " + pid + " running? " + isPidRunning);
                if (!isPidRunning)
                {
                    generateAndSendEmail();
                    break;
                }
            }
        }
    }
}