//
//  bp_room_adaptor.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 3/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include <jni.h>
#include "macros.h"
#include <random>
#include <cstring>
#include "ServerGameSession.h"

/* These functions are called from Java. */
extern "C"
{
    JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_start(JNIEnv* env, jclass cls, jstring room_id);

    JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_init(JNIEnv* env, jclass cls, jstring room_id);

    JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_handle_1server_1update(JNIEnv* env, jclass cls, jstring room_id, jstring message);

    JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_update(JNIEnv* env, jclass cls, jstring room_id, jfloat delta_time);

    JNIEXPORT float JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1player_1x(JNIEnv* env, jclass cls, jstring room_id, jshort player_index);

    JNIEXPORT float JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1player_1y(JNIEnv* env, jclass cls, jstring room_id, jshort player_index);

    JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1player_1direction(JNIEnv* env, jclass cls, jstring room_id, jshort player_index);
    
    JNIEXPORT bool JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_is_1player_1alive(JNIEnv* env, jclass cls, jstring room_id, jshort player_index);

    JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1num_1breakable_1blocks(JNIEnv* env, jclass cls, jstring room_id);

    JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1breakable_1block_1grid_1x(JNIEnv* env, jclass cls, jstring room_id, jshort breakable_block_index);

    JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1breakable_1block_1grid_1y(JNIEnv* env, jclass cls, jstring room_id, jshort breakable_block_index);

    JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1breakable_1block_1power_1up_1flag(JNIEnv* env, jclass cls, jstring room_id, jshort breakable_block_index);
};

struct RoomIdGameSessionPair
{
    const char *room_id;
    ServerGameSession *game_session;
};

std::vector<RoomIdGameSessionPair> roomIdGameSessionPairs;

ServerGameSession *getGameSessionForRoomId(JNIEnv* env, jstring room_id)
{
    const char *nativeString = (env)->GetStringUTFChars(room_id, NULL);
    for (int i = 0; i < roomIdGameSessionPairs.size(); i++)
    {
        if (std::strcmp(roomIdGameSessionPairs[i].room_id, nativeString) == 0)
        {
            // We know which session we are working with now.
            ServerGameSession *game_session = roomIdGameSessionPairs[i].game_session;
            return game_session;
        }
    }

    return nullptr;
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_start(JNIEnv* env, jclass cls, jstring room_id)
{
    UNUSED(env);
    UNUSED(cls);

    const char *str = env->GetStringUTFChars(room_id, NULL);
    RoomIdGameSessionPair newRoomIdGameSessionPair = {str, new ServerGameSession()};
    roomIdGameSessionPairs.push_back(newRoomIdGameSessionPair);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_init(JNIEnv* env, jclass cls, jstring room_id)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        gameSession->init();
    }
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_handle_1server_1update(JNIEnv* env, jclass cls, jstring room_id, jstring message)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        const char *messageCharArray = env->GetStringUTFChars(message, NULL);
        gameSession->handleServerUpdate(messageCharArray);
    }
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_update(JNIEnv* env, jclass cls, jstring room_id, jfloat delta_time)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        gameSession->update(delta_time);
    }
}

JNIEXPORT float JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1player_1x(JNIEnv* env, jclass cls, jstring room_id, jshort player_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getPlayerXAtIndex(player_index);
    }

    return 0;
}

JNIEXPORT float JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1player_1y(JNIEnv* env, jclass cls, jstring room_id, jshort player_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getPlayerYAtIndex(player_index);
    }

    return 0;
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1player_1direction(JNIEnv* env, jclass cls, jstring room_id, jshort player_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getPlayerDirectionAtIndex(player_index);
    }

    return 0;
}

JNIEXPORT bool JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_is_1player_1alive(JNIEnv* env, jclass cls, jstring room_id, jshort player_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->isPlayerAliveAtIndex(player_index);
    }
    
    return false;
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1num_1breakable_1blocks(JNIEnv* env, jclass cls, jstring room_id)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getNumBreakableBlocks();
    }
    
    return 0;
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1breakable_1block_1grid_1x(JNIEnv* env, jclass cls, jstring room_id, jshort breakable_block_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getBreakableBlockGridX(breakable_block_index);
    }
    
    return 0;
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1breakable_1block_1grid_1y(JNIEnv* env, jclass cls, jstring room_id, jshort breakable_block_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getBreakableBlockGridY(breakable_block_index);
    }
    
    return 0;
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_BpRoomAdaptor_get_1breakable_1block_1power_1up_1flag(JNIEnv* env, jclass cls, jstring room_id, jshort breakable_block_index)
{
    UNUSED(env);
    UNUSED(cls);

    ServerGameSession *gameSession = getGameSessionForRoomId(env, room_id);
    if (gameSession != nullptr)
    {
        return gameSession->getBreakableBlockPowerUpFlag(breakable_block_index);
    }
    
    return 0;
}