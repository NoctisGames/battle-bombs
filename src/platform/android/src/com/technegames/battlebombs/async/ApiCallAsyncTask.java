package com.technegames.battlebombs.async;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.os.AsyncTask;

import com.technegames.battlebombs.ApiConstants;
import com.technegames.battlebombs.HttpHeaderLogger;
import com.technegames.battlebombs.IOUtils;
import com.technegames.battlebombs.InstanceManager;
import com.technegames.battlebombs.Logger;
import com.technegames.battlebombs.NetworkUtil;
import com.technegames.battlebombs.R;
import com.technegames.battlebombs.api.ServerStatusResponse;
import com.technegames.battlebombs.callback.AsyncTaskCallback;
import com.technegames.battlebombs.json.JsonDeserializer;

public final class ApiCallAsyncTask extends AsyncTask<Object, Void, ServerStatusResponse>
{
    private static final Logger logger = new Logger(ApiCallAsyncTask.class);

    protected final AsyncTaskCallback callback;

    public ApiCallAsyncTask(AsyncTaskCallback callback)
    {
        this.callback = callback;
    }

    @Override
    protected void onPreExecute()
    {
        super.onPreExecute();

        callback.safeShowProgressDialogForMessage(callback.getActivity().getString(R.string.json_request_progress_message));
    }

    @Override
    protected ServerStatusResponse doInBackground(Object... params)
    {
        if (NetworkUtil.isConnected(callback.getActivity()))
        {
            try
            {
                URL url = new URL(ApiConstants.SERVER_STATUS_URL);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();

                conn.setReadTimeout(70000);
                conn.setConnectTimeout(70000);
                conn.setRequestMethod("GET");
                conn.addRequestProperty("Accept", "application/json");

                String serverResponse = getServerResponse(conn);

                JsonDeserializer jsonDeserializer = InstanceManager.getInstance().retrieveInstance(JsonDeserializer.class);
                ServerStatusResponse jsonResponse = jsonDeserializer.deserializeJsonIntoType(serverResponse, ServerStatusResponse.class);

                return jsonResponse;
            }
            catch (Exception e)
            {
                logger.error("Encountered Exception while trying to execute request: " + e.toString(), e);
            }
        }

        return null;
    }

    @Override
    protected void onPostExecute(ServerStatusResponse response)
    {
        callback.onComplete(response);
    }

    private String getServerResponse(HttpURLConnection httpURLConnection) throws IOException
    {
        if (Logger.isDebugEnabled())
        {
            HttpHeaderLogger.logHeaders(httpURLConnection);
            logger.debug("Connecting to URL: " + httpURLConnection.getURL().toString());
        }

        httpURLConnection.connect();

        InputStream in;
        String serverResponse;
        int responseCode = httpURLConnection.getResponseCode();

        if (responseCode >= 200 && responseCode <= 202)
        {
            in = httpURLConnection.getInputStream();
            serverResponse = IOUtils.extractStringFromInputStream(in);
        }
        else
        {
            in = httpURLConnection.getErrorStream();
            serverResponse = IOUtils.extractStringFromInputStream(in);
        }

        IOUtils.safeClose(in);

        httpURLConnection.disconnect();

        if (Logger.isDebugEnabled())
        {
            logger.debug("Http Response Body: ");
            logger.debug(serverResponse);
        }

        return serverResponse;
    }
}