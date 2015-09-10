package com.gowengamedev.battlebombsservernotifyadmin;

import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.WebResource;
import com.sun.jersey.api.client.filter.HTTPBasicAuthFilter;
import com.sun.jersey.core.util.MultivaluedMapImpl;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Date;
import javax.ws.rs.core.MediaType;

/**
 *
 * @author sgowen
 */
public final class NotifyServerAdmin
{
    private static final long ONCE_PER_MINUTE = 1000 * 60; // expressed in milliseconds

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

            showAllProcessesRunningOnUbuntu();

            TimerTask scanPidsTimerTask = new ScanPidsTimerTask(pids);
            //perform the task once a minute starting now
            Timer timer = new Timer();
            timer.scheduleAtFixedRate(scanPidsTimerTask, new Date(), ONCE_PER_MINUTE);
        }
    }

    private static boolean isProcessIdRunningOnUbuntu(int pid)
    {
        //this queries tasklist if the pid passed in is running.
        //If the pid is running it returns true, else false.
        try
        {
            Process process = Runtime.getRuntime().exec("ps -A");
            InputStream inputstream = process.getInputStream();
            InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
            BufferedReader bufferedreader = new BufferedReader(inputstreamreader);
            String line;
            while ((line = bufferedreader.readLine()) != null)
            {
                //Search the PID matched lines single line for the sequence: "1300"
                //if you find it, then the PID is still running.
                if (line.contains("" + pid + ""))
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

    private static void showAllProcessesRunningOnUbuntu()
    {
        try
        {
            Process process = Runtime.getRuntime().exec("ps -A");
            InputStream inputstream = process.getInputStream();
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
        System.out.println("Sending email to notify Admin of Battle Bombs server termination...");

        Client client = Client.create();
        client.addFilter(new HTTPBasicAuthFilter("api", "key-6c5956134ef684e1051c38e9d776110f"));
        WebResource webResource = client.resource("https://api.mailgun.net/v3/sandboxf37f8766c6ba4fcf9f7cde3291da73bd.mailgun.org/messages");
        MultivaluedMapImpl formData = new MultivaluedMapImpl();
        formData.add("from", "Mailgun Sandbox <postmaster@sandboxf37f8766c6ba4fcf9f7cde3291da73bd.mailgun.org>");
        formData.add("to", "Stephen Gowen <dev.sgowen+battlebombs@gmail.com>");
        formData.add("subject", "Battle Bombs Server has Stopped!");
        formData.add("text", "Congratulations Stephen Gowen, you just sent an email with Mailgun!  You are truly awesome!  You can see a record of this email in your logs: https://mailgun.com/cp/log .  You can send up to 300 emails/day from this sandbox server.  Next, you should add your own domain so you can send 10,000 emails/month for free.");

        ClientResponse response = webResource.type(MediaType.APPLICATION_FORM_URLENCODED).post(ClientResponse.class, formData);

        System.out.println("Email sent!");

        // An email has been sent, so this program no longer needs to run
        System.exit(0);
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
                boolean isPidRunning = isProcessIdRunningOnUbuntu(pid);
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