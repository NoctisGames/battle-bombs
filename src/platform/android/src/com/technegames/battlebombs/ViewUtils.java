package com.technegames.battlebombs;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Point;
import android.os.Build;
import android.os.Build.VERSION;
import android.view.Display;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;

public final class ViewUtils
{
    public static void hideKeyboardForView(View view)
    {
        Context context = view.getContext();
        InputMethodManager inputMethodManager = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
    }

    @SuppressLint("NewApi")
    @SuppressWarnings("deprecation")
    public static Point getScreenSize(Activity activity)
    {
        Display display = activity.getWindowManager().getDefaultDisplay();
        Point size = new Point();

        if (VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2)
        {
            display.getSize(size);
        }
        else
        {
            size.x = display.getWidth();
            size.y = display.getHeight();
        }

        return size;
    }

    public static String getTrimmedString(TextView tv)
    {
        return tv.getText().toString().trim();
    }

    private ViewUtils()
    {
        // Hide Constructor
    }
}