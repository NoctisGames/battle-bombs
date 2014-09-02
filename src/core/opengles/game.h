//
//  game.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

void on_surface_created(int pixelWidth, int pixelHeight);

void on_surface_changed(int pixelWidth, int pixelHeight, int dpWidth, int dpHeight);

void init(const char *username, bool isOffline = false);

void on_resume();

void on_pause();

void update(float deltaTime);

void present();

void on_chat_received(const char *message);

void on_touch_down(float raw_touch_x, float raw_touch_y);

void on_touch_dragged(float raw_touch_x, float raw_touch_y);

void on_touch_up(float raw_touch_x, float raw_touch_y);

short get_current_music_id();

short get_current_sound_id();

int get_oldest_event_id();

bool is_time_to_send_keep_alive();

void reset_time_since_last_client_event();

int get_state();

void clear_state();

short get_player_index();

float get_player_x();

float get_player_y();

int get_player_direction();

bool handle_on_back_pressed();

#endif /* GAME_H */