package com.technegames.battlebombs;

/**
 *
 * @author sgowen
 */
public final class UserSessionData
{
    public final String _username;
    public long _timeSinceLastChat;
    public short _playerIndex;
    public int _platform;

    public UserSessionData(String username, long timeSinceLastChat, short playerIndex, int platform)
    {
        _username = username;
        _timeSinceLastChat = timeSinceLastChat;
        _playerIndex = playerIndex;
        _platform = platform;
    }
}