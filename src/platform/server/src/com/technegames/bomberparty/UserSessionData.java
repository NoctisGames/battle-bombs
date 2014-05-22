package com.technegames.bomberparty;

/**
 *
 * @author sgowen
 */
public final class UserSessionData
{
    public float _timeSinceLastChat;
    public short _playerIndex;
    
    public UserSessionData(float timeSinceLastChat, short playerIndex)
    {
        _timeSinceLastChat = timeSinceLastChat;
        _playerIndex = playerIndex;
    }
}