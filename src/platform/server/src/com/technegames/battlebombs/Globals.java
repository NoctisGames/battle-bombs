package com.technegames.battlebombs;

import java.util.Random;

/**
 *
 * @author sgowen
 */
public final class Globals
{
    public static final String CURRENT_ZONE_KEY = "1bc691b6-8c0f-4247-9";
    public static final String[] BOT_NAMES =
    {
        "Tyler Bot",
        "Stephen Bot",
        "Mikel Bot",
        "Dan Bot",
        "Matt Bot",
        "Jay Bot",
        "Kyle Bot",
        "Kenny Bot",
        "Eric Bot",
        "Stan Bot",
        "Brad Bot",
        "Adam Bot",
        "Mary Bot",
        "Andre Bot",
        "Bill Bot",
        "Mike Bot",
        "Jason Bot",
        "Kurt Bot",
        "Travis Bot",
        "Joe Bot",
        "James Bot",
        "Heather Bot",
        "Glen Bot",
        "Don Bot",
        "Paul Bot",
        "Morgan Bot",
        "Sean Bot",
        "Dave Bot",
        "Pier Bot",
        "Roy Bot",
        "Aaron Bot",
        "Frank Bot",
        "Evan Bot",
        "Peter Bot",
        "Jeff Bot",
        "Ryan Bot",
        "Jenna Bot",
        "Todd Bot",
        "Wesley Bot"
    };

    private static final Random botNameRandom = new Random();

    public static String getRandomBotName()
    {
        return BOT_NAMES[botNameRandom.nextInt(BOT_NAMES.length)];
    }
}