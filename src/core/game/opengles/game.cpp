//
//  game.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "game.h"
#include "OpenGLESGameScreen.h"
#include "TouchEvent.h"
#include "Assets.h"

OpenGLESGameScreen *gameScreen;
std::vector<TouchEvent> touchEvents;
std::vector<TouchEvent> touchEventsPool;
std::vector<TouchEvent> touchEventsBuffer;

TouchEvent newTouchEvent()
{
	if(touchEventsPool.size() == 0)
	{
		return TouchEvent(0, 0, Touch_Type::DOWN);
	}
	else
	{
		TouchEvent touchEvent = touchEventsPool.back();
		touchEventsPool.pop_back();
		return touchEvent;
	}
}

void addTouchEventForType(Touch_Type touchType, float x, float y)
{
	TouchEvent touchEvent = newTouchEvent();
	touchEvent.setTouchType(touchType);
	touchEvent.setX(x);
	touchEvent.setY(y);
    
	touchEventsBuffer.push_back(touchEvent);
}

void init(const char *username, bool isOffline, bool isRunningIOS8)
{
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    
    gameScreen = new OpenGLESGameScreen(username, isOffline, isRunningIOS8);
}

void on_surface_created(int pixelWidth, int pixelHeight)
{
    gameScreen->onSurfaceCreated(pixelWidth, pixelHeight);
}

void on_surface_changed(int pixelWidth, int pixelHeight, int dpWidth, int dpHeight)
{
    gameScreen->onSurfaceChanged(pixelWidth, pixelHeight, dpWidth, dpHeight);
}

void on_resume()
{
    gameScreen->onResume();
}

void on_pause()
{
    gameScreen->onPause();
}

void update(float deltaTime)
{
    for (std::vector<TouchEvent>::iterator itr = touchEvents.begin(); itr != touchEvents.end(); itr++)
	{
		if(touchEventsPool.size() < 50)
		{
			touchEventsPool.push_back(*itr);
		}
	}
    
	touchEvents.clear();
	touchEvents.swap(touchEventsBuffer);
	touchEventsBuffer.clear();
    
	gameScreen->update(deltaTime, touchEvents);
}

void present()
{
    gameScreen->present();
}

void on_chat_received(const char *message)
{
    gameScreen->handleServerUpdate(message);
}

void on_touch_down(float raw_touch_x, float raw_touch_y)
{
    addTouchEventForType(DOWN, raw_touch_x, raw_touch_y);
}

void on_touch_dragged(float raw_touch_x, float raw_touch_y)
{
    if(touchEventsBuffer.size() < 3)
	{
        addTouchEventForType(DRAGGED, raw_touch_x, raw_touch_y);
	}
}

void on_touch_up(float raw_touch_x, float raw_touch_y)
{
    addTouchEventForType(UP, raw_touch_x, raw_touch_y);
}

short get_current_music_id()
{
    short musicId = Assets::getInstance()->getMusicId();
    Assets::getInstance()->setMusicId(0);
    return musicId;
}

short get_current_sound_id()
{
    short playThisSound = Assets::getInstance()->getFirstSoundId();
    Assets::getInstance()->eraseFirstSoundId();
    return playThisSound;
}

int get_oldest_event_id()
{
    return gameScreen->popOldestEventId();
}

bool is_time_to_send_keep_alive()
{
    return gameScreen->isTimeToSendKeepAlive();
}

void reset_time_since_last_client_event()
{
    gameScreen->resetTimeSinceLastClientEvent();
}

int get_state()
{
    return gameScreen->getScreenState();
}

void clear_state()
{
    gameScreen->clearState();
}

short get_player_index()
{
    return gameScreen->getPlayerIndex();
}

float get_player_x()
{
    return gameScreen->getPlayerX();
}

float get_player_y()
{
    return gameScreen->getPlayerY();
}

int get_player_direction()
{
    return gameScreen->getPlayerDirection();
}

bool handle_on_back_pressed()
{
    return gameScreen->handleOnBackPressed();
}

#pragma mark <Offline Mode>

int get_num_seconds_left()
{
    return gameScreen->getNumSecondsLeft();
}
