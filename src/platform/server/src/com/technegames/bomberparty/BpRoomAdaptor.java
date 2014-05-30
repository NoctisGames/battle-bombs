package com.technegames.bomberparty;

import com.shephertz.app42.server.idomain.BaseRoomAdaptor;
import com.shephertz.app42.server.idomain.HandlingResult;
import com.shephertz.app42.server.idomain.IRoom;
import com.shephertz.app42.server.idomain.IUser;
import java.util.HashMap;
import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;

/**
 *
 * @author sgowen
 */
public final class BpRoomAdaptor extends BaseRoomAdaptor
{
    static
    {
        System.load("C:\\Users\\sgowen\\Documents\\GitHub\\bomber-party\\src\\platform\\server\\bp-native\\dist\\libBpNative.dll");
    }

    private static final String GAME_BEGIN_LOG = "~~~~~~~~~~~~~~~~~~~ GAME BEGIN ~~~~~~~~~~~~~~~~~~~";
    private static final String SERVER = "Server";
    private static final String EVENT_TYPE = "eventType";
    private static final String NUM_PLAYERS = "numPlayers";
    private static final String EVENTS = "events";
    private static final String NUM_BREAKABLE_BLOCKS = "numBreakableBlocks";
    private static final String BREAKABLE_BLOCK_X_VALUES = "breakableBlockXValues";
    private static final String BREAKABLE_BLOCK_Y_VALUES = "breakableBlockYValues";
    private static final String BREAKABLE_BLOCK_POWER_UP_FLAGS = "breakableBlockPowerUpFlags";
    private static final short BEGIN_SPECTATE = 1336;
    private static final short BEGIN_GAME = 1337;
    private static final short CLIENT_UPDATE = 1338;
    private static final short PLAYER_DEATH = 9;
    private static final short PLAYER_EVENT_BASE = 100;

    private final IRoom _room;
    private final Map<IUser, UserSessionData> _inRoomUserSessionDataMap = new HashMap<>();
    private final Map<IUser, UserSessionData> _inGameUserSessionDataMap = new HashMap<>();
    private final boolean[] _playerSpotsOccupied =
    {
        false, false, false, false, false, false, false, false
    };
    private final boolean[] _playerSpotsReceivedBeginGameCommand =
    {
        false, false, false, false, false, false, false, false
    };

    private float _stateTime;
    private long _lastFrameTime;
    private boolean _isGameRunning;

    public BpRoomAdaptor(IRoom room)
    {
        _room = room;
        _stateTime = 0;
        _lastFrameTime = 0L;
        _isGameRunning = false;

        System.out.println("Creating Adapter for Room: " + _room.getName() + ", with MAX USERS: " + _room.getMaxUsers());

        start(_room.getId());
    }

    @Override
    public void onUserLeaveRequest(IUser user)
    {
        removeUser(user, true);
    }

    @Override
    public void handleUserJoinRequest(IUser user, HandlingResult result)
    {
        if (_inRoomUserSessionDataMap.containsKey(user))
        {
            System.out.println(user.getName() + " rejoined the room");

            logRoom();

            return;
        }
        else
        {
            for (short i = 0; i < _playerSpotsOccupied.length; i++)
            {
                if (!_playerSpotsOccupied[i])
                {
                    _stateTime = 0;

                    _playerSpotsOccupied[i] = true;
                    _inRoomUserSessionDataMap.put(user, new UserSessionData(0f, i));

                    System.out.println(user.getName() + " joined the room");

                    logRoom();

                    return;
                }
            }
        }

        result.code = 1; // Not Successful
        result.sendNotification = false;
        result.sendResponse = true;
        result.description = "Room is Full";
    }

    @Override
    public void handleChatRequest(IUser sender, String message, HandlingResult result)
    {
        if (_inRoomUserSessionDataMap.containsKey(sender))
        {
            System.out.println(sender.getName() + " says " + message);

            // Sending just a blank message serves as a keep alive
            _inRoomUserSessionDataMap.get(sender)._timeSinceLastChat = 0f;

            if (_inGameUserSessionDataMap.containsKey(sender) && message != null && message.contains("{") && message.contains(","))
            {
                updateRoomWithMessage(message);
            }
        }

        result.sendNotification = false;
    }

    /*
     * Timer
     * Called after particular interval defined in AppConfig.json
     */
    @Override
    public void onTimerTick(long time)
    {
        float deltaTime = (time - _lastFrameTime) / 1000.0f;
        _lastFrameTime = time;

        for (Map.Entry entry : _inRoomUserSessionDataMap.entrySet())
        {
            IUser user = (IUser) entry.getKey();
            _inRoomUserSessionDataMap.get(user)._timeSinceLastChat += deltaTime;

            if (_inRoomUserSessionDataMap.get(user)._timeSinceLastChat > 7)
            {
                removeUser(user, false);
            }
            else if (_isGameRunning && _inRoomUserSessionDataMap.get(user)._timeSinceLastChat > 2)
            {
                short playerIndex = _inRoomUserSessionDataMap.get(user)._playerIndex;
                if (!_playerSpotsReceivedBeginGameCommand[playerIndex])
                {
                    System.out.println("SENDING BEGIN_GAME event to player so that he/she can spectate!");

                    _playerSpotsReceivedBeginGameCommand[playerIndex] = true;

                    String beginGameCommand = getGameStateCommand(BEGIN_SPECTATE);
                    if (beginGameCommand != null)
                    {
                        updateRoomWithMessage(beginGameCommand);
                    }

                    logRoom();
                }
            }
        }

        if (_isGameRunning)
        {
            update(_room.getId(), deltaTime);

            short eventId = get_oldest_event_id(_room.getId());

            if (eventId > 0)
            {
                String eventsMessage = eventId + ",";
                while ((eventId = get_oldest_event_id(_room.getId())) > 0)
                {
                    eventsMessage += eventId + ",";
                }

                eventsMessage += "0"; // Terminate with 0

                try
                {
                    JSONObject tobeSent = new JSONObject();
                    tobeSent.put(EVENT_TYPE, CLIENT_UPDATE);
                    tobeSent.put(EVENTS, eventsMessage);

                    appendBotData(tobeSent);

                    _room.BroadcastChat(SERVER, tobeSent.toString());
                }
                catch (JSONException e)
                {
                    System.err.println(e.toString());
                }
            }

            if (_inGameUserSessionDataMap.isEmpty())
            {
                _isGameRunning = false;
                _stateTime = 0;
            }
            else
            {
                short numAlive = 0;
                for (short playerIndex = 0; playerIndex < get_num_players(_room.getId()); playerIndex++)
                {
                    if (is_player_alive(_room.getId(), playerIndex))
                    {
                        System.out.println("Player " + playerIndex + " is alive");
                        numAlive++;
                    }
                    else
                    {
                        System.out.println("Player " + playerIndex + " is NOT alive");
                    }
                }

                if (numAlive <= 1)
                {
                    // Game has ended
                    // TODO, send out a player won event

                    _isGameRunning = false;
                    _stateTime = 0;
                }
            }
        }
        else if (_inRoomUserSessionDataMap.size() >= 2)
        {
            _stateTime += deltaTime;

            if (_stateTime > 10)
            {
                // We are re-initialzing the list, starting from 0,
                // so that we don't end up with a game with only 2 players
                // that indexed at 3 and 7 or something weird like that.
                for (short i = 0; i < _playerSpotsOccupied.length; i++)
                {
                    _playerSpotsOccupied[i] = false;
                    _playerSpotsReceivedBeginGameCommand[i] = false;
                }

                _inGameUserSessionDataMap.clear();

                short i = 0;
                for (Map.Entry entry : _inRoomUserSessionDataMap.entrySet())
                {
                    IUser user = (IUser) entry.getKey();
                    UserSessionData userSessionData = (UserSessionData) entry.getValue();

                    userSessionData._playerIndex = i;
                    _playerSpotsOccupied[i] = true;
                    _playerSpotsReceivedBeginGameCommand[i] = true;

                    // Lock in users for this game session, more users can join the room,
                    // but they won't be included in the game until the next round.
                    _inGameUserSessionDataMap.put(user, userSessionData);

                    i++;
                }

                init(_room.getId(), _inGameUserSessionDataMap.size());

                String beginGameCommand = getGameStateCommand(BEGIN_GAME);
                if (beginGameCommand != null)
                {
                    updateRoomWithMessage(beginGameCommand);

                    _isGameRunning = true;

                    System.out.println(GAME_BEGIN_LOG);
                }
            }
        }
        else
        {
            _stateTime = 0;
        }
    }

    private String getGameStateCommand(short eventType)
    {
        try
        {
            JSONObject tobeSent = new JSONObject();
            tobeSent.put(EVENT_TYPE, eventType);
            tobeSent.put(NUM_PLAYERS, get_num_players(_room.getId()));

            for (Map.Entry entry : _inGameUserSessionDataMap.entrySet())
            {
                IUser user = (IUser) entry.getKey();
                UserSessionData userSessionData = (UserSessionData) entry.getValue();

                short playerIndex = userSessionData._playerIndex;

                tobeSent.put("playerIndex" + playerIndex, user.getName());
                tobeSent.put("playerIndex" + playerIndex + "X", get_player_x(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Y", get_player_y(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Direction", get_player_direction(_room.getId(), playerIndex));
            }

            appendBotData(tobeSent);

            int numBreakableBlocks = get_num_breakable_blocks(_room.getId());
            final StringBuilder breakableBlockXValuesStringBuilder = new StringBuilder();
            final StringBuilder breakableBlockYValuesStringBuilder = new StringBuilder();
            final StringBuilder breakableBlockPowerUpFlagsStringBuilder = new StringBuilder();
            for (short j = 0; j < numBreakableBlocks; j++)
            {
                breakableBlockXValuesStringBuilder.append(get_breakable_block_grid_x(_room.getId(), j));
                breakableBlockYValuesStringBuilder.append(get_breakable_block_grid_y(_room.getId(), j));
                breakableBlockPowerUpFlagsStringBuilder.append(get_breakable_block_power_up_flag(_room.getId(), j));

                if (j < numBreakableBlocks - 1)
                {
                    breakableBlockXValuesStringBuilder.append(',');
                    breakableBlockYValuesStringBuilder.append(',');
                    breakableBlockPowerUpFlagsStringBuilder.append(',');
                }
            }

            tobeSent.put(NUM_BREAKABLE_BLOCKS, numBreakableBlocks);
            tobeSent.put(BREAKABLE_BLOCK_X_VALUES, breakableBlockXValuesStringBuilder.toString());
            tobeSent.put(BREAKABLE_BLOCK_Y_VALUES, breakableBlockYValuesStringBuilder.toString());
            tobeSent.put(BREAKABLE_BLOCK_POWER_UP_FLAGS, breakableBlockPowerUpFlagsStringBuilder.toString());

            return tobeSent.toString();
        }
        catch (JSONException e)
        {
            System.err.println(e.toString());
        }

        return null;
    }

    private void appendBotData(JSONObject tobeSent) throws JSONException
    {
        for (short playerIndex = 0; playerIndex < get_num_players(_room.getId()); playerIndex++)
        {
            if (is_player_bot(_room.getId(), playerIndex))
            {
                tobeSent.put("playerIndex" + playerIndex, "Bot " + playerIndex);
                tobeSent.put("playerIndex" + playerIndex + "X", get_player_x(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Y", get_player_y(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Direction", get_player_direction(_room.getId(), playerIndex));
            }
        }
    }

    private void updateRoomWithMessage(final String message)
    {
        handle_server_update(_room.getId(), message);

        _room.BroadcastChat(SERVER, message);
    }

    private void removeUser(IUser user, boolean onUserLeaveRequest)
    {
        if (_inGameUserSessionDataMap.containsKey(user))
        {
            UserSessionData userSessionData = _inGameUserSessionDataMap.get(user);
            short playerIndex = userSessionData._playerIndex;

            if (is_player_alive(_room.getId(), playerIndex))
            {
                short playerDeathEvent = (short) (playerIndex * PLAYER_EVENT_BASE + PLAYER_DEATH);
                String playerDeathEventString = String.valueOf(playerDeathEvent);
                playerDeathEventString = playerDeathEventString.concat(",0");

                try
                {
                    JSONObject tobeSent = new JSONObject();
                    tobeSent.put(EVENT_TYPE, CLIENT_UPDATE);
                    tobeSent.put(EVENTS, playerDeathEventString);

                    updateRoomWithMessage(tobeSent.toString());
                }
                catch (JSONException e)
                {
                    System.err.println(e.toString());
                }
            }

            _inGameUserSessionDataMap.remove(user);
        }

        if (_inRoomUserSessionDataMap.containsKey(user))
        {
            _playerSpotsOccupied[_inRoomUserSessionDataMap.get(user)._playerIndex] = false;
            _playerSpotsReceivedBeginGameCommand[_inRoomUserSessionDataMap.get(user)._playerIndex] = false;

            _inRoomUserSessionDataMap.remove(user);
        }

        _room.removeUser(user, true);

        System.out.println(user.getName() + " left room (" + (onUserLeaveRequest ? "exited" : "timed out") + ")");

        logRoom();
    }

    private void logRoom()
    {
        System.out.println("_inRoomUserSessionDataMap size: " + _inRoomUserSessionDataMap.size());
        System.out.println("_inGameUserSessionDataMap size: " + _inGameUserSessionDataMap.size());
        System.out.println("_playerSpotsOccupied: { "
                + _playerSpotsOccupied[0] + ", "
                + _playerSpotsOccupied[1] + ", "
                + _playerSpotsOccupied[2] + ", "
                + _playerSpotsOccupied[3] + ", "
                + _playerSpotsOccupied[4] + ", "
                + _playerSpotsOccupied[5] + ", "
                + _playerSpotsOccupied[6] + ", "
                + _playerSpotsOccupied[7] + " }");
        System.out.println("_playerSpotsReceivedBeginGameCommand: { "
                + _playerSpotsReceivedBeginGameCommand[0] + ", "
                + _playerSpotsReceivedBeginGameCommand[1] + ", "
                + _playerSpotsReceivedBeginGameCommand[2] + ", "
                + _playerSpotsReceivedBeginGameCommand[3] + ", "
                + _playerSpotsReceivedBeginGameCommand[4] + ", "
                + _playerSpotsReceivedBeginGameCommand[5] + ", "
                + _playerSpotsReceivedBeginGameCommand[6] + ", "
                + _playerSpotsReceivedBeginGameCommand[7] + " }");
    }

    private static native void start(String roomId);

    private static native void init(String roomId, int numHumanPlayers);

    private static native void handle_server_update(String roomId, String message);

    private static native void update(String roomId, float deltaTime);

    private static native int get_num_players(String roomId);

    private static native boolean is_player_bot(String roomId, short playerIndex);

    private static native float get_player_x(String roomId, short playerIndex);

    private static native float get_player_y(String roomId, short playerIndex);

    private static native int get_player_direction(String roomId, short playerIndex);

    private static native boolean is_player_alive(String roomId, short playerIndex);

    private static native int get_num_breakable_blocks(String roomId);

    private static native int get_breakable_block_grid_x(String roomId, int breakable_block_index);

    private static native int get_breakable_block_grid_y(String roomId, int breakable_block_index);

    private static native int get_breakable_block_power_up_flag(String roomId, int breakable_block_index);

    private static native short get_oldest_event_id(String roomId);
}
