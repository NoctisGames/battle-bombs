package com.gowengamedev.battlebombsserver;

import com.shephertz.app42.server.idomain.BaseRoomAdaptor;
import com.shephertz.app42.server.idomain.HandlingResult;
import com.shephertz.app42.server.idomain.IRoom;
import com.shephertz.app42.server.idomain.IUser;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;

/**
 *
 * @author sgowen
 */
public final class Bb131RoomAdaptor extends BaseRoomAdaptor
{
    static
    {
        System.load(Main.NATIVE_LIBRARY_LOCATION);
    }

    private static final String PRE_GAME_TICK_LOG = "~~~~~~~~~~~~~~ PRE GAME TICK ~~~~~~~~~~~~~~~~~~";
    private static final String GAME_BEGIN_LOG = "~~~~~~~~~~~~~~~~~~~ GAME BEGIN ~~~~~~~~~~~~~~~~~~~";
    private static final String GAME_OVER_LOG = "~~~~~~~~~~~~~~~~~~~~ GAME--OVER ~~~~~~~~~~~~~~~~~~~";
    private static final String SERVER = "Server";
    private static final String EVENT_TYPE = "eventType";
    private static final String NUM_PLAYERS = "numPlayers";
    private static final String HAS_WINNER = "hasWinner";
    private static final String WINNING_PLAYER_INDEX = "winningPlayerIndex";
    private static final String TIME_TO_NEXT_ROUND = "timeToNextRound";
    private static final String MAP_TYPE = "mapType";
    private static final String IS_SUDDEN_DEATH_MODE = "isSuddenDeathMode";
    private static final String TIME_SINCE_SUDDEN_DEATH_MODE_BEGAN = "timeSinceSuddenDeathModeBegan";
    private static final String NUM_SECONDS_LEFT_FOR_ROUND = "numSecondsLeftForRound";
    private static final String EVENTS = "events";
    private static final String NUM_BREAKABLE_BLOCKS = "numBreakableBlocks";
    private static final String BREAKABLE_BLOCK_X_VALUES = "breakableBlockXValues";
    private static final String BREAKABLE_BLOCK_Y_VALUES = "breakableBlockYValues";
    private static final String BREAKABLE_BLOCK_POWER_UP_FLAGS = "breakableBlockPowerUpFlags";
    private static final String NUM_DELETED_BREAKABLE_BLOCKS = "numDeletedBreakableBlocks";
    private static final String DELETED_BREAKABLE_BLOCK_X_VALUES = "deletedBreakableBlockXValues";
    private static final String DELETED_BREAKABLE_BLOCK_Y_VALUES = "deletedBreakableBlockYValues";
    private static final String NUM_DELETED_POWER_UPS = "numDeletedPowerUps";
    private static final String DELETED_POWER_UP_X_VALUES = "deletedPowerUpsXValues";
    private static final String DELETED_POWER_UP_Y_VALUES = "deletedPowerUpsYValues";
    private static final String NUM_BREAKABLE_BLOCKS_AT_SPAWN_TIME = "numBreakableBlocksAtSpawnTime";
    private static final int TIME_BETWEEN_ROUNDS = 12;
    private static final int PLATFORM_UNKNOWN = 0;
    private static final int NUM_MAPS = 4;
    private static final short PRE_GAME_SERVER_UPDATE = 1335;
    private static final short BEGIN_SPECTATE = 1336;
    private static final short BEGIN_GAME = 1337;
    private static final short CLIENT_UPDATE = 1338;
    private static final short SUDDEN_DEATH = 1339;
    private static final short GAME_OVER = 1340;
    private static final short HARD_UPDATE = 1341;
    private static final int PLAYER_DEATH = 11;
    private static final int PLAYER_EVENT_BASE = 100000000;
    private static final int PLAYER_EVENT_DIRECTION_BASE = 10000000;

    // smooth constant elements to play with
    private static final float movAveragePeriod = 40; // #frames involved in average calc (suggested values 5-100)
    private static final float smoothFactor = 0.1f; // adjusting ratio (suggested values 0.01-0.5)

    private final IRoom _room;
    private final Map<IUser, UserSessionData> _inRoomUserSessionDataMap = new HashMap<>();
    private final Map<IUser, UserSessionData> _inGameUserSessionDataMap = new HashMap<>();
    private final boolean[] _playerSpotsOccupied =
    {
        false, false, false, false, false, false, false, false
    };
    private final boolean[] _playerSpotsReceivedGameStateCommand =
    {
        false, false, false, false, false, false, false, false
    };
    private final String[] _activePlayerNames =
    {
        "", "", "", "", "", "", "", ""
    };

    private int _numSecondsLeftForRound;
    private float _stateTime;
    private float _timeSinceLastHardUpdate;
    private float _timeSinceSuddenDeathModeBegan;
    private float _oneOrLessPlayersAliveTimer;
    private float _countdownTime;
    private boolean _isGameRunning;
    private boolean _isGameCountingDown;
    private boolean _isSuddenDeathMode;
    private boolean _hasSentGameOverMessage;
    private float smoothedDeltaRealTime_ms = 17.5f; // initial value, Optionally you can save the new computed value (will change with each hardware) in Preferences to optimize the first drawing frames
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms; // mov Average start with default value
    private long lastRealTimeMeasurement_ms; // temporal storage for last time measurement
    private int mapType = 0; // Increment this by 1 every time we start a new game

    public Bb131RoomAdaptor(IRoom room)
    {
        _room = room;
        _stateTime = 0;
        _timeSinceLastHardUpdate = 0;
        _timeSinceSuddenDeathModeBegan = 0;
        _oneOrLessPlayersAliveTimer = 0;
        _countdownTime = 0;
        _isGameRunning = false;
        _hasSentGameOverMessage = false;
        _isGameCountingDown = false;
        _isSuddenDeathMode = false;

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
        if (!_inRoomUserSessionDataMap.containsKey(user))
        {
            for (short i = 0; i < _playerSpotsOccupied.length; i++)
            {
                if (!_playerSpotsOccupied[i])
                {
                    if (!_isGameRunning)
                    {
                        _stateTime = 0;
                        _timeSinceLastHardUpdate = 0;
                        _timeSinceSuddenDeathModeBegan = 0;
                        _countdownTime = 0;
                        _oneOrLessPlayersAliveTimer = 0;
                    }

                    _playerSpotsOccupied[i] = true;
                    _inRoomUserSessionDataMap.put(user, new UserSessionData(user.getName(), 0, i, PLATFORM_UNKNOWN));

                    System.out.println(user.getName() + " joined the room");

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
// Sending just a blank message serves as a keep alive
            _inRoomUserSessionDataMap.get(sender)._timeSinceLastChat = 0;

            if (_inGameUserSessionDataMap.containsKey(sender) && message != null && message.contains("{") && message.contains(","))
            {
                updateRoomWithMessage(message);
            }
            else
            {
                String platformString = message.substring(message.length() - 1);
                int platform = Integer.parseInt(platformString);
                _inRoomUserSessionDataMap.get(sender)._platform = platform;
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
        float deltaTime = smoothedDeltaRealTime_ms / 1000;

        if (_isGameRunning)
        {
            if (_inRoomUserSessionDataMap.isEmpty())
            {
                endGame();
            }
            else
            {
                _stateTime += deltaTime;

                if (_isGameCountingDown)
                {
                    if (_stateTime >= 3)
                    {
                        _isGameCountingDown = false;
                        _stateTime -= 3;
                    }

                    update(_room.getId(), deltaTime);
                }
                else
                {
                    while (_stateTime >= 1)
                    {
                        _numSecondsLeftForRound--;
                        _stateTime -= 1;
                    }

                    short numAlive = 0;
                    short numHumansAlive = 0;
                    short winningPlayerIndex = -1;
                    for (short playerIndex = 0; playerIndex < get_num_players(_room.getId()); playerIndex++)
                    {
                        if (is_player_alive(_room.getId(), playerIndex))
                        {
                            winningPlayerIndex = playerIndex;
                            numAlive++;

                            if (!is_player_bot(_room.getId(), playerIndex))
                            {
                                numHumansAlive++;
                            }
                        }
                    }

                    boolean isGameStillOn = _numSecondsLeftForRound > 0 && numAlive > 1;
                    for (Map.Entry entry : _inRoomUserSessionDataMap.entrySet())
                    {
                        IUser user = (IUser) entry.getKey();
                        if (_inRoomUserSessionDataMap.get(user)._timeSinceLastChat > 1000)
                        {
                            short playerIndex = _inRoomUserSessionDataMap.get(user)._playerIndex;
                            if (!_playerSpotsReceivedGameStateCommand[playerIndex] && _numSecondsLeftForRound >= 6)
                            {
                                String beginGameCommand = getGameStateCommand(BEGIN_SPECTATE);
                                if (beginGameCommand != null)
                                {
                                    updateRoomWithMessage(beginGameCommand);
                                }

                                _playerSpotsReceivedGameStateCommand[playerIndex] = true;
                            }
                        }
                    }

                    if (_isSuddenDeathMode)
                    {
                        _timeSinceSuddenDeathModeBegan += deltaTime;
                    }
                    else if (_numSecondsLeftForRound <= 60 || numHumansAlive == 0)
                    {
                        try
                        {
                            JSONObject tobeSent = new JSONObject();
                            tobeSent.put(EVENT_TYPE, SUDDEN_DEATH);

                            updateRoomWithMessage(tobeSent.toString());

                            _isSuddenDeathMode = true;
                        }
                        catch (JSONException e)
                        {
                            System.err.println(e.toString());
                        }
                    }

                    update(_room.getId(), deltaTime);

                    int eventId = get_oldest_event_id(_room.getId());

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

                    _timeSinceLastHardUpdate += deltaTime;
                    if (_timeSinceLastHardUpdate > 4)
                    {
                        _timeSinceLastHardUpdate = 0;

                        sendHardUpdateToClients();
                    }

                    if (!isGameStillOn)
                    {
                        _oneOrLessPlayersAliveTimer += deltaTime;

                        if (_oneOrLessPlayersAliveTimer > 8)
                        {
                            endGame();
                        }
                        else if (!_hasSentGameOverMessage && _oneOrLessPlayersAliveTimer > 0.5f)
                        {
                            boolean hasWinner = numAlive == 1;
                            _oneOrLessPlayersAliveTimer -= 0.5f;

                            try
                            {
                                JSONObject tobeSent = new JSONObject();
                                tobeSent.put(EVENT_TYPE, GAME_OVER);
                                tobeSent.put(HAS_WINNER, hasWinner);
                                tobeSent.put(WINNING_PLAYER_INDEX, winningPlayerIndex);

                                updateRoomWithMessage(tobeSent.toString());

                                _hasSentGameOverMessage = true;

                                System.out.println(GAME_OVER_LOG);
                            }
                            catch (JSONException e)
                            {
                                System.err.println(e.toString());
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (_inRoomUserSessionDataMap.size() > 0)
            {
                _stateTime += deltaTime;
                _countdownTime += deltaTime;
                if (_countdownTime > 1)
                {
                    _countdownTime -= 1;

                    try
                    {
                        JSONObject tobeSent = new JSONObject();
                        tobeSent.put(EVENT_TYPE, PRE_GAME_SERVER_UPDATE);
                        tobeSent.put(TIME_TO_NEXT_ROUND, TIME_BETWEEN_ROUNDS - (int) _stateTime);
                        tobeSent.put(NUM_PLAYERS, _inRoomUserSessionDataMap.size());
                        for (Map.Entry entry : _inRoomUserSessionDataMap.entrySet())
                        {
                            IUser user = (IUser) entry.getKey();
                            UserSessionData userSessionData = (UserSessionData) entry.getValue();

                            short playerIndex = userSessionData._playerIndex;

                            tobeSent.put("playerIndex" + playerIndex, user.getName());
                            tobeSent.put("playerIndex" + playerIndex + "Platform", userSessionData._platform);
                        }

                        updateRoomWithMessage(tobeSent.toString());
                    }
                    catch (JSONException e)
                    {
                        System.err.println(e.toString());
                    }
                }

                if (_stateTime > TIME_BETWEEN_ROUNDS)
                {
                    // We are re-initialzing the list, starting from 0,
                    // so that we don't end up with a game with only 2 players
                    // that indexed at 3 and 7 or something weird like that.
                    for (short i = 0; i < _playerSpotsOccupied.length; i++)
                    {
                        _playerSpotsOccupied[i] = false;
                        _playerSpotsReceivedGameStateCommand[i] = false;
                        _activePlayerNames[i] = "";
                    }

                    _inGameUserSessionDataMap.clear();

                    short i = 0;
                    for (Map.Entry entry : _inRoomUserSessionDataMap.entrySet())
                    {
                        IUser user = (IUser) entry.getKey();
                        UserSessionData userSessionData = (UserSessionData) entry.getValue();

                        userSessionData._playerIndex = i;
                        _playerSpotsOccupied[i] = true;
                        _playerSpotsReceivedGameStateCommand[i] = true;
                        _activePlayerNames[i] = user.getName();

                        // Lock in users for this game session, more users can join the room,
                        // but they won't be included in the game until the next round.
                        _inGameUserSessionDataMap.put(user, userSessionData);

                        i++;
                    }

                    init(_room.getId(), _inGameUserSessionDataMap.size(), mapType);

                    for (short playerIndex = 0; playerIndex < get_num_players(_room.getId()); playerIndex++)
                    {
                        if (is_player_bot(_room.getId(), playerIndex))
                        {
                            String botName = new String(get_player_name_bytes(_room.getId(), playerIndex), Charset.forName("UTF-8"));
                            _activePlayerNames[playerIndex] = botName;
                        }
                    }

                    _numSecondsLeftForRound = 180;

                    String beginGameCommand = getGameStateCommand(BEGIN_GAME);
                    if (beginGameCommand != null)
                    {
                        updateRoomWithMessage(beginGameCommand);

                        _isGameRunning = true;
                        _isGameCountingDown = true;
                        _isSuddenDeathMode = false;
                        _hasSentGameOverMessage = false;
                        _stateTime = 0;
                        _timeSinceLastHardUpdate = 0;
                        _timeSinceSuddenDeathModeBegan = 0;
                        _oneOrLessPlayersAliveTimer = 0;
                        _countdownTime = 0;

                        System.out.println(GAME_BEGIN_LOG);
                        System.out.println("Number of Players: " + _inGameUserSessionDataMap.size());
                    }
                }
            }
            else
            {
                _stateTime = 0;
                _oneOrLessPlayersAliveTimer = 0;
                _countdownTime = 0;
            }
        }

        // Moving average calc
        long currTimePick_ms = System.nanoTime() / 1000000;
        float realTimeElapsed_ms;
        if (lastRealTimeMeasurement_ms > 0)
        {
            realTimeElapsed_ms = (currTimePick_ms - lastRealTimeMeasurement_ms);
        }
        else
        {
            realTimeElapsed_ms = smoothedDeltaRealTime_ms; // just the first time
        }

        movAverageDeltaTime_ms = (realTimeElapsed_ms + movAverageDeltaTime_ms * (movAveragePeriod - 1)) / movAveragePeriod;

        // Calc a better aproximation for smooth stepTime
        smoothedDeltaRealTime_ms = smoothedDeltaRealTime_ms + (movAverageDeltaTime_ms - smoothedDeltaRealTime_ms) * smoothFactor;

        lastRealTimeMeasurement_ms = currTimePick_ms;

        for (Map.Entry entry : _inRoomUserSessionDataMap.entrySet())
        {
            IUser user = (IUser) entry.getKey();
            _inRoomUserSessionDataMap.get(user)._timeSinceLastChat += realTimeElapsed_ms;
            if (_inRoomUserSessionDataMap.get(user)._timeSinceLastChat > 7000)
            {
                removeUser(user, false);
            }
        }
    }

    private String getGameStateCommand(short eventType)
    {
        try
        {
            JSONObject tobeSent = new JSONObject();
            tobeSent.put(EVENT_TYPE, eventType);
            tobeSent.put(MAP_TYPE, mapType);
            tobeSent.put(IS_SUDDEN_DEATH_MODE, _isSuddenDeathMode);
            tobeSent.put(NUM_BREAKABLE_BLOCKS_AT_SPAWN_TIME, get_num_breakable_blocks_at_spawn_time(_room.getId()));
            tobeSent.put(TIME_SINCE_SUDDEN_DEATH_MODE_BEGAN, _timeSinceSuddenDeathModeBegan);
            tobeSent.put(NUM_SECONDS_LEFT_FOR_ROUND, _numSecondsLeftForRound);
            tobeSent.put(NUM_PLAYERS, get_num_players(_room.getId()));

            for (Map.Entry entry : _inGameUserSessionDataMap.entrySet())
            {
                IUser user = (IUser) entry.getKey();
                UserSessionData userSessionData = (UserSessionData) entry.getValue();

                short playerIndex = userSessionData._playerIndex;

                tobeSent.put("playerIndex" + playerIndex, user.getName());
                tobeSent.put("playerIndex" + playerIndex + "IsBot", false);
                tobeSent.put("playerIndex" + playerIndex + "X", get_player_x(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Y", get_player_y(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Direction", get_player_direction(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Alive", is_player_alive(_room.getId(), playerIndex));
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
                else
                {
                    breakableBlockXValuesStringBuilder.append(",-1");
                    breakableBlockYValuesStringBuilder.append(",-1");
                    breakableBlockPowerUpFlagsStringBuilder.append(",-1");
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

    private void sendHardUpdateToClients()
    {
        try
        {
            JSONObject tobeSent = new JSONObject();
            tobeSent.put(EVENT_TYPE, HARD_UPDATE);

            int numDeletedBreakableBlocks = get_num_deleted_breakable_blocks(_room.getId());
            final StringBuilder deletedBreakableBlockXValuesStringBuilder = new StringBuilder();
            final StringBuilder deletedBreakableBlockYValuesStringBuilder = new StringBuilder();
            for (short j = 0; j < numDeletedBreakableBlocks; j++)
            {
                deletedBreakableBlockXValuesStringBuilder.append(get_deleted_breakable_block_grid_x(_room.getId(), j));
                deletedBreakableBlockYValuesStringBuilder.append(get_deleted_breakable_block_grid_y(_room.getId(), j));

                if (j < numDeletedBreakableBlocks - 1)
                {
                    deletedBreakableBlockXValuesStringBuilder.append(',');
                    deletedBreakableBlockYValuesStringBuilder.append(',');
                }
                else
                {
                    deletedBreakableBlockXValuesStringBuilder.append(",-1");
                    deletedBreakableBlockYValuesStringBuilder.append(",-1");
                }
            }

            tobeSent.put(NUM_DELETED_BREAKABLE_BLOCKS, numDeletedBreakableBlocks);
            tobeSent.put(DELETED_BREAKABLE_BLOCK_X_VALUES, deletedBreakableBlockXValuesStringBuilder.toString());
            tobeSent.put(DELETED_BREAKABLE_BLOCK_Y_VALUES, deletedBreakableBlockYValuesStringBuilder.toString());

            int numDeletedPowerUps = get_num_deleted_power_ups(_room.getId());
            final StringBuilder deletedPowerUpXValuesStringBuilder = new StringBuilder();
            final StringBuilder deletedPowerUpYValuesStringBuilder = new StringBuilder();
            for (short j = 0; j < numDeletedPowerUps; j++)
            {
                deletedPowerUpXValuesStringBuilder.append(get_deleted_power_up_grid_x(_room.getId(), j));
                deletedPowerUpYValuesStringBuilder.append(get_deleted_power_up_grid_y(_room.getId(), j));

                if (j < numDeletedPowerUps - 1)
                {
                    deletedPowerUpXValuesStringBuilder.append(',');
                    deletedPowerUpYValuesStringBuilder.append(',');
                }
                else
                {
                    deletedPowerUpXValuesStringBuilder.append(",-1");
                    deletedPowerUpYValuesStringBuilder.append(",-1");
                }
            }

            tobeSent.put(NUM_DELETED_POWER_UPS, numDeletedPowerUps);
            tobeSent.put(DELETED_POWER_UP_X_VALUES, deletedPowerUpXValuesStringBuilder.toString());
            tobeSent.put(DELETED_POWER_UP_Y_VALUES, deletedPowerUpYValuesStringBuilder.toString());

            _room.BroadcastChat(SERVER, tobeSent.toString());
        }
        catch (JSONException e)
        {
            System.err.println(e.toString());
        }
    }

    private void appendBotData(JSONObject tobeSent) throws JSONException
    {
        for (short playerIndex = 0; playerIndex < get_num_players(_room.getId()); playerIndex++)
        {
            if (is_player_bot(_room.getId(), playerIndex))
            {
                tobeSent.put("playerIndex" + playerIndex, _activePlayerNames[playerIndex]);
                tobeSent.put("playerIndex" + playerIndex + "IsBot", true);
                tobeSent.put("playerIndex" + playerIndex + "X", get_player_x(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Y", get_player_y(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Direction", get_player_direction(_room.getId(), playerIndex));
                tobeSent.put("playerIndex" + playerIndex + "Alive", is_player_alive(_room.getId(), playerIndex));
            }
        }
    }

    private void updateRoomWithMessage(final String message)
    {
        handle_server_update(_room.getId(), message);

        _room.BroadcastChat(SERVER, message);
    }

    private void endGame()
    {
        _isGameRunning = false;
        _isGameCountingDown = false;
        _isSuddenDeathMode = false;
        _stateTime = 0;
        _oneOrLessPlayersAliveTimer = 0;
        _countdownTime = 0;

        mapType++;
        if (mapType == NUM_MAPS)
        {
            mapType = 0;
        }
    }

    private void removeUser(IUser user, boolean onUserLeaveRequest)
    {
        if (_inGameUserSessionDataMap.containsKey(user))
        {
            UserSessionData userSessionData = _inGameUserSessionDataMap.get(user);
            short playerIndex = userSessionData._playerIndex;

            if (is_player_alive(_room.getId(), playerIndex))
            {
                int eventId = PLAYER_DEATH;
                eventId += PLAYER_EVENT_BASE * playerIndex;
                eventId += PLAYER_EVENT_DIRECTION_BASE * get_player_direction(_room.getId(), playerIndex);

                String playerDeathEventString = String.valueOf(eventId);
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
            _playerSpotsReceivedGameStateCommand[_inRoomUserSessionDataMap.get(user)._playerIndex] = false;

            _inRoomUserSessionDataMap.remove(user);
        }

        _room.removeUser(user, true);

        System.out.println(user.getName() + " left room (" + (onUserLeaveRequest ? "exited" : "timed out") + ")");
    }

    private final class UserSessionData
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

    private static native void start(String roomId);

    private static native void init(String roomId, int numHumanPlayers, int mapType);

    private static native byte[] get_player_name_bytes(String roomId, short playerIndex);

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

    private static native int get_num_deleted_breakable_blocks(String roomId);

    private static native int get_deleted_breakable_block_grid_x(String roomId, int breakable_block_index);

    private static native int get_deleted_breakable_block_grid_y(String roomId, int breakable_block_index);

    private static native int get_num_deleted_power_ups(String roomId);

    private static native int get_deleted_power_up_grid_x(String roomId, int power_up_index);

    private static native int get_deleted_power_up_grid_y(String roomId, int power_up_index);

    private static native int get_num_breakable_blocks_at_spawn_time(String roomId);

    private static native int get_oldest_event_id(String roomId);
}