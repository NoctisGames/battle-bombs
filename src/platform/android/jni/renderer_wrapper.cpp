//
//  renderer_wrapper.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include <jni.h>
#include "game.h"
#include "macros.h"

/* These functions are called from Java. */
extern "C"
{
JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1surface_1created(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height, jint dp_width, jint dp_height);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_init(JNIEnv* env, jclass cls, jstring username);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1resume(JNIEnv* env, jclass cls);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1pause(JNIEnv* env, jclass cls);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_update(JNIEnv* env, jclass cls, jfloat delta_time);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_present(JNIEnv* env, jclass cls);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1chat_1received(JNIEnv* env, jclass cls, jstring message);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);

JNIEXPORT short JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1current_1music_1id(JNIEnv* env, jclass cls);

JNIEXPORT short JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1current_1sound_1id(JNIEnv* env, jclass cls);

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1oldest_1event_1id(JNIEnv* env, jclass cls);

JNIEXPORT bool JNICALL Java_com_technegames_bomberparty_RendererWrapper_is_1time_1to_1send_1keep_1alive(JNIEnv* env, jclass cls);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_reset_1time_1since_1last_1client_1event(JNIEnv* env, jclass cls);

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1state(JNIEnv* env, jclass cls);

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_clear_1state(JNIEnv* env, jclass cls);

JNIEXPORT short JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1index(JNIEnv* env, jclass cls);

JNIEXPORT float JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1x(JNIEnv* env, jclass cls);

JNIEXPORT float JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1y(JNIEnv* env, jclass cls);

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1direction(JNIEnv* env, jclass cls);

JNIEXPORT bool JNICALL Java_com_technegames_bomberparty_RendererWrapper_handle_1on_1back_1pressed(JNIEnv* env, jclass cls);
};

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1surface_1created(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height)
{
	UNUSED(env);
	UNUSED(cls);
	on_surface_created(pixel_width, pixel_height);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height, jint dp_width, jint dp_height)
{
	UNUSED(env);
	UNUSED(cls);
	on_surface_changed(pixel_width, pixel_height, dp_width, dp_height);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_init(JNIEnv* env, jclass cls, jstring username)
{
	UNUSED(env);
	UNUSED(cls);

	const char *nativeUsername = (env)->GetStringUTFChars(username, nullptr);
	init(nativeUsername);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1resume(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	on_resume();
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1pause(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	on_pause();
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_update(JNIEnv* env, jclass cls, jfloat delta_time)
{
	UNUSED(env);
	UNUSED(cls);
	update(delta_time);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_present(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	present();
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1chat_1received(JNIEnv* env, jclass cls, jstring message)
{
	UNUSED(env);
	UNUSED(cls);

	const char *nativeMessage = (env)->GetStringUTFChars(message, nullptr);
	on_chat_received(nativeMessage);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
	on_touch_down(raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
	on_touch_dragged(raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
	on_touch_up(raw_touch_x, raw_touch_y);
}

JNIEXPORT short JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1current_1music_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	return get_current_music_id();
}

JNIEXPORT short JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1current_1sound_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	return get_current_sound_id();
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1oldest_1event_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	return get_oldest_event_id();
}

JNIEXPORT bool JNICALL Java_com_technegames_bomberparty_RendererWrapper_is_1time_1to_1send_1keep_1alive(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	return is_time_to_send_keep_alive();
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_reset_1time_1since_1last_1client_1event(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	reset_time_since_last_client_event();
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1state(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	return get_state();
}

JNIEXPORT void JNICALL Java_com_technegames_bomberparty_RendererWrapper_clear_1state(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	clear_state();
}

JNIEXPORT short JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1index(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return get_player_index();
}

JNIEXPORT float JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1x(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return get_player_x();
}

JNIEXPORT float JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1y(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return get_player_y();
}

JNIEXPORT int JNICALL Java_com_technegames_bomberparty_RendererWrapper_get_1player_1direction(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return get_player_direction();
}

JNIEXPORT bool JNICALL Java_com_technegames_bomberparty_RendererWrapper_handle_1on_1back_1pressed(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
	return handle_on_back_pressed();
}
