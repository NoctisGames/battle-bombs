package com.technegames.battlebombs;

/**
 *
 * @author sgowen
 */
public final class UserSessionData
{
    public long _timeSinceLastChat;
    public short _playerIndex;
    public int _platform;

    public UserSessionData(long timeSinceLastChat, short playerIndex, int platform)
    {
        _timeSinceLastChat = timeSinceLastChat;
        _playerIndex = playerIndex;
        _platform = platform;
    }
}