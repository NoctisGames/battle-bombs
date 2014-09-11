package com.technegames.battlebombs;

import java.net.URLConnection;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.http.Header;
import org.apache.http.HttpMessage;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;

public final class HttpHeaderLogger
{
    private static final Logger logger = new Logger(HttpHeaderLogger.class);

    public static void logHeaders(URLConnection urlConnection)
    {
        StringBuilder sb = new StringBuilder();
        sb.append("Headers: \n");

        Map<String, List<String>> headers = urlConnection.getHeaderFields();
        Set<Map.Entry<String, List<String>>> entrySet = headers.entrySet();
        for (Map.Entry<String, List<String>> entry : entrySet)
        {
            sb.append(entry.getKey());
            sb.append(" : ");
            List<String> headerValues = entry.getValue();
            for (String value : headerValues)
            {
                sb.append(value);
            }

            sb.append("\n");
        }

        logger.debug(sb.toString());
    }

    public static void logHeaders(HttpPost httpPost)
    {
        StringBuilder sb = new StringBuilder();
        sb.append("Request Headers: \n");
        logHeaders(httpPost, sb);
    }

    public static void logHeaders(HttpResponse httpResponse)
    {
        StringBuilder sb = new StringBuilder();
        sb.append("Response Headers: \n");
        logHeaders(httpResponse, sb);
    }

    private static void logHeaders(HttpMessage httpMessage, StringBuilder sb)
    {
        for (Header header : httpMessage.getAllHeaders())
        {
            sb.append(header.getName());
            sb.append(" : ");
            sb.append(header.getValue());
            sb.append("\n");
        }

        logger.debug(sb.toString());
    }

    private HttpHeaderLogger()
    {
        // Hide Constructor
    }
}