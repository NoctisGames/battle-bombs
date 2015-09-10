package com.technegames.battlebombs;

public final class StringUtils
{
    public static boolean isStringEmpty(String string)
    {
        return string == null || string.length() == 0;
    }

    private StringUtils()
    {
        // Hide Constructor
    }
}