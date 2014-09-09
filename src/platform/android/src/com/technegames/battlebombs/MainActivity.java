package com.technegames.battlebombs;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.widget.EditText;

import com.technegames.battlebombs.api.IsCurrentVersionResponse;
import com.technegames.battlebombs.async.ApiCallAsyncTask;
import com.technegames.battlebombs.callback.AsyncTaskCallback;

public final class MainActivity extends Activity
{
    private AsyncTaskCallback _asyncTaskCallback;
    private AsyncTask<Object, Void, IsCurrentVersionResponse> _currentlyExecutingAsyncTask;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
    }

    @Override
    public void onPause()
    {
        super.onPause();

        if (_currentlyExecutingAsyncTask != null && _currentlyExecutingAsyncTask.getStatus() == AsyncTask.Status.RUNNING)
        {
            _currentlyExecutingAsyncTask.cancel(true);
        }

        if (_asyncTaskCallback != null)
        {
            _asyncTaskCallback.safeDismissProgressDialog();
        }
    }

    public void startQuickMatch(View view)
    {
        AsyncTaskCallback callback = new AsyncTaskCallback(this)
        {
            @Override
            protected void onSuccess(IsCurrentVersionResponse onSuccessObject)
            {
                if (onSuccessObject.isCurrentVersion)
                {
                    final EditText usernameEditText = new EditText(MainActivity.this);
                    usernameEditText.setInputType(InputType.TYPE_TEXT_VARIATION_NORMAL);
                    usernameEditText.setHint(R.string.enter_username_hint);

                    new AlertDialog.Builder(MainActivity.this).setTitle(R.string.enter_username_title).setView(usernameEditText).setPositiveButton(R.string.enter_username_button, new DialogInterface.OnClickListener()
                    {
                        public void onClick(DialogInterface dialog, int whichButton)
                        {
                            dialog.dismiss();

                            ViewUtils.hideKeyboardForView(usernameEditText);

                            String username = ViewUtils.getTrimmedString(usernameEditText);
                            if (username.length() >= 3 && username.length() <= 15)
                            {
                                GameActivity.startActivity(MainActivity.this, username);
                            }
                            else
                            {
                                new AlertDialog.Builder(MainActivity.this).setTitle(R.string.invalid_username_title).setMessage(R.string.invalid_username_message).setPositiveButton(R.string.invalid_username_cancel, new DialogInterface.OnClickListener()
                                {
                                    public void onClick(DialogInterface dialog, int whichButton)
                                    {
                                        dialog.dismiss();
                                    }
                                }).show();
                            }
                        }
                    }).setNegativeButton(R.string.enter_username_cancel, new DefaultActionOnClickListener()).show();
                }
                else
                {
                    new AlertDialog.Builder(MainActivity.this).setTitle(R.string.update_required_title).setMessage(R.string.update_required_message).setPositiveButton(R.string.update_required_cancel, new DialogInterface.OnClickListener()
                    {
                        public void onClick(DialogInterface dialog, int whichButton)
                        {
                            dialog.dismiss();
                        }
                    }).show();
                }
            }

            @Override
            protected void onFailure()
            {
                new AlertDialog.Builder(MainActivity.this).setTitle(R.string.connection_error_title).setMessage(R.string.connection_error_message).setPositiveButton(R.string.connection_error_cancel, new DialogInterface.OnClickListener()
                {
                    public void onClick(DialogInterface dialog, int whichButton)
                    {
                        dialog.dismiss();
                    }
                }).show();
            }
        };

        _asyncTaskCallback = callback;
        _currentlyExecutingAsyncTask = new ApiCallAsyncTask(callback);
        _currentlyExecutingAsyncTask.execute();
    }

    public void startQuickOfflineMatch(View view)
    {
        final EditText usernameEditText = new EditText(this);
        usernameEditText.setInputType(InputType.TYPE_TEXT_VARIATION_NORMAL);
        usernameEditText.setHint(R.string.enter_username_hint);

        new AlertDialog.Builder(this).setTitle(R.string.enter_username_title).setView(usernameEditText).setPositiveButton(R.string.enter_username_play, new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int whichButton)
            {
                dialog.dismiss();

                ViewUtils.hideKeyboardForView(usernameEditText);

                String username = ViewUtils.getTrimmedString(usernameEditText);
                if (username.length() >= 3 && username.length() <= 15)
                {
                    OfflineGameActivity.startActivity(MainActivity.this, username);
                }
                else
                {
                    new AlertDialog.Builder(MainActivity.this).setTitle(R.string.invalid_username_title).setMessage(R.string.invalid_username_message).setPositiveButton(R.string.invalid_username_cancel, new DialogInterface.OnClickListener()
                    {
                        public void onClick(DialogInterface dialog, int whichButton)
                        {
                            dialog.dismiss();
                        }
                    }).show();
                }
            }
        }).setNegativeButton(R.string.enter_username_cancel, new DefaultActionOnClickListener()).show();
    }

    private final class DefaultActionOnClickListener implements DialogInterface.OnClickListener
    {
        @Override
        public void onClick(DialogInterface dialog, int which)
        {
            dialog.dismiss();
        }
    }
}