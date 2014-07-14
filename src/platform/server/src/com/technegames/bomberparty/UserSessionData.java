package com.technegames.bomberparty;

/**
 *
 * @author sgowen
 */
public final class UserSessionData
{
    public long _timeSinceLastChat;
    public short _playerIndex;
    
    public UserSessionData(long timeSinceLastChat, short playerIndex)
    {
        _timeSinceLastChat = timeSinceLastChat;
        _playerIndex = playerIndex;
    }
}