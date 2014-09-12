package com.technegames.battlebombs;

import java.util.Random;

/**
 *
 * @author sgowen
 */
public final class Globals
{
    public static final String CURRENT_AUTH_DATA = "T3chn3G4m35_01";
    public static final boolean IS_BATTLE_BOMBS_BETA_CLOSED = false;
    public static final String[] BOT_NAMES =
    {
        "Tyler Bot",
        "Stephen Bot",
        "Kathy Bot",
        "Mikel Bot",
        "Dan Bot",
        "Matt Bot",
        "Jay Bot",
        "Bradlee Bot",
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
        "Peter Bot",
        "Jeff Bot",
        "Ryan Bot",
        "Todd Bot",
        "Wesley Bot",
        "Ted Bot",
        "Barney Bot",
        "Robin Bot",
        "Lily Bot",
        "Marshall Bot",
        "Kyle Bot",
        "Kenny Bot",
        "Eric Bot",
        "Stan Bot",
        "Randy Bot",
        "Norman Bot",
        "Norma Bot",
        "Dylan Bot",
        "Dexter Bot",
        "Debra Bot",
        "Sam Bot",
        "Dean Bot",
        "Bobby Bot",
        "Ruby Bot",
        "Brandi Bot",
        "Mariel Bot",
        "Bruce Bot",
        "Dick Bot",
        "Alfred Bot",
        "Dante Bot"
    };

    private static final Random botNameRandom = new Random();

    public static String getRandomBotName()
    {
        return BOT_NAMES[botNameRandom.nextInt(BOT_NAMES.length)];
    }
}