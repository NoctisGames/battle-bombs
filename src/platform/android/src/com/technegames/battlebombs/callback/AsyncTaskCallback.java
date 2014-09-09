package com.technegames.battlebombs.callback;

import java.lang.ref.WeakReference;

import android.app.Activity;
import android.app.ProgressDialog;
import android.view.WindowManager;

import com.technegames.battlebombs.StringUtils;
import com.technegames.battlebombs.api.IsCurrentVersionResponse;

public abstract class AsyncTaskCallback
{
    private final WeakReference<Activity> activityReference;
    private final ProgressDialog progressDialog;

    protected abstract void onSuccess(IsCurrentVersionResponse onSuccessObject);

    protected abstract void onFailure();

    public AsyncTaskCallback(Activity activity)
    {
        this.activityReference = new WeakReference<Activity>(activity);
        this.progressDialog = createProgressDialog();
    }

    public void onComplete(IsCurrentVersionResponse object)
    {
        safeDismissProgressDialog();

        if (object == null)
        {
            onFailure();
        }
        else
        {
            onSuccess(object);
        }
    }

    public final void safeShowProgressDialogForMessage(String message)
    {
        if (!StringUtils.isStringEmpty(message) && progressDialog != null && !progressDialog.isShowing())
        {
            progressDialog.setMessage(message);
            progressDialog.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            progressDialog.show();
        }
    }

    public final void safeDismissProgressDialog()
    {
        if (progressDialog != null && progressDialog.isShowing())
        {
            progressDialog.dismiss();
        }
    }

    public final Activity getActivity()
    {
        return activityReference.get();
    }

    private ProgressDialog createProgressDialog()
    {
        ProgressDialog progressDialog = new ProgressDialog(getActivity());
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);

        return progressDialog;
    }
}