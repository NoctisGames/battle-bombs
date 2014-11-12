package com.technegames.battlebombs;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;

import com.technegames.battlebombs.api.ServerStatusResponse;
import com.technegames.battlebombs.async.ApiCallAsyncTask;
import com.technegames.battlebombs.callback.AsyncTaskCallback;

public final class MainActivity extends Activity
{
    private AsyncTaskCallback _asyncTaskCallback;
    private AsyncTask<Object, Void, ServerStatusResponse> _currentlyExecutingAsyncTask;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);

        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onPause()
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

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == GameActivity.REQUEST_CODE && resultCode == GameActivity.RESULT_CONNECTION_ERROR)
        {
            showConnectionErrorDialog();
        }
    }

    public void startQuickMatch(View view)
    {
        AsyncTaskCallback callback = new AsyncTaskCallback(this)
        {
            @Override
            protected void onSuccess(ServerStatusResponse onSuccessObject)
            {
                if (onSuccessObject.isCurrentVersion)
                {
                    if (onSuccessObject.isDownForMaintenance)
                    {
                        new AlertDialog.Builder(MainActivity.this).setTitle(R.string.down_for_maintenance_title).setMessage(R.string.down_for_maintenance_message).setPositiveButton(R.string.down_for_maintenance_cancel, new DialogInterface.OnClickListener()
                        {
                            public void onClick(DialogInterface dialog, int whichButton)
                            {
                                dialog.dismiss();
                            }
                        }).show();
                    }
                    else
                    {
                        final EditText usernameEditText = new EditText(MainActivity.this);
                        usernameEditText.setInputType(InputType.TYPE_CLASS_TEXT);
                        usernameEditText.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI);
                        usernameEditText.setHint(R.string.enter_username_hint);

                        String storedUsername = AppPrefs.getInstance(MainActivity.this).getPlayerName();
                        usernameEditText.setText(storedUsername);

                        new AlertDialog.Builder(MainActivity.this).setTitle(R.string.enter_username_title).setView(usernameEditText).setPositiveButton(R.string.enter_username_button, new DialogInterface.OnClickListener()
                        {
                            public void onClick(DialogInterface dialog, int whichButton)
                            {
                                dialog.dismiss();

                                ViewUtils.hideKeyboardForView(usernameEditText);

                                String username = ViewUtils.getTrimmedString(usernameEditText);
                                AppPrefs.getInstance(MainActivity.this).setPlayerName(username);
                                if (username.length() >= 3 && username.length() <= 12)
                                {
                                    GameActivity.startActivityForResult(MainActivity.this, username);
                                }
                                else
                                {
                                    showInvalidUsernameDialog();
                                }
                            }
                        }).setNegativeButton(R.string.enter_username_cancel, new DefaultActionOnClickListener()).show();
                    }
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
                showConnectionErrorDialog();
            }
        };

        _asyncTaskCallback = callback;
        _currentlyExecutingAsyncTask = new ApiCallAsyncTask(callback);
        _currentlyExecutingAsyncTask.execute();
    }

    public void startQuickOfflineMatch(View view)
    {
        final EditText usernameEditText = new EditText(this);
        usernameEditText.setInputType(InputType.TYPE_CLASS_TEXT);
        usernameEditText.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI);
        usernameEditText.setHint(R.string.enter_username_hint);

        String storedUsername = AppPrefs.getInstance(MainActivity.this).getPlayerName();
        usernameEditText.setText(storedUsername);

        new AlertDialog.Builder(this).setTitle(R.string.enter_username_title).setView(usernameEditText).setPositiveButton(R.string.enter_username_play, new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int whichButton)
            {
                dialog.dismiss();

                ViewUtils.hideKeyboardForView(usernameEditText);

                String username = ViewUtils.getTrimmedString(usernameEditText);
                AppPrefs.getInstance(MainActivity.this).setPlayerName(username);
                if (username.length() >= 3 && username.length() <= 12)
                {
                    OfflineGameActivity.startActivity(MainActivity.this, username);
                }
                else
                {
                    showInvalidUsernameDialog();
                }
            }
        }).setNegativeButton(R.string.enter_username_cancel, new DefaultActionOnClickListener()).show();
    }

    private void showInvalidUsernameDialog()
    {
        new AlertDialog.Builder(MainActivity.this).setTitle(R.string.invalid_username_title).setMessage(R.string.invalid_username_message).setPositiveButton(R.string.invalid_username_cancel, new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int whichButton)
            {
                dialog.dismiss();
            }
        }).show();
    }

    private void showConnectionErrorDialog()
    {
        new AlertDialog.Builder(MainActivity.this).setTitle(R.string.connection_error_title).setMessage(R.string.connection_error_message).setPositiveButton(R.string.connection_error_cancel, new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int whichButton)
            {
                dialog.dismiss();
            }
        }).show();
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