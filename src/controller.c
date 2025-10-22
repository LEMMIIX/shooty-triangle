#include <math.h>
#include <stdbool.h>
#include "SDL3/SDL_rect.h"

#include "SDL3/SDL_render.h"
#include "c_map.h"
#include "common.h"
#include "player.h"
#include "controller.h"

struct map_uint_bool key_state[] = {
	{SDLK_W, 0},	// move up
	{SDLK_A, 0},	// move down
	{SDLK_S, 0},	// move left
	{SDLK_D, 0},	// move right
	{SDLK_Q, 0},	// quit
};

extern Player_ship ship;

float Center_x = WINDOW_WIDTH / 2.0f;
float Center_y = WINDOW_HEIGHT / 2.0f;

float Mouse_x = WINDOW_WIDTH / 2.0f;
float Mouse_y = WINDOW_HEIGHT / 2.0f;

float Player_rotation_angle = 0.0f;

void turn_the_thing(float* mouse_x, float* mouse_y) {
	Mouse_x = *mouse_x;
	Mouse_y = *mouse_y;

	float vector_x = Mouse_x - Center_x;
	float vector_y = Mouse_y - Center_y;

	Player_rotation_angle = atan2f(vector_y, vector_x);
	for (int i = 0; i < 3; ++i) {
		float x = ship.reference_shape[i].position.x - Center_x;
		float y = ship.reference_shape[i].position.y - Center_y;

		float x_rotated = x * cos(Player_rotation_angle) - y * sin(Player_rotation_angle);
		float y_rotated = x * sin(Player_rotation_angle) + y * cos(Player_rotation_angle);

		ship.shape[i].position.x = x_rotated + Center_x;
		ship.shape[i].position.y = y_rotated + Center_y;
	}
}

void move_ship_up() {
	Center_y = (ship.reference_shape[0].position.y +ship.reference_shape[1].position.y +ship.reference_shape[2].position.y) / 3;
	
	if (Center_y - MOVEMENT_SPEED * delta_time > 0) {
		ship.shape[0].position.y -= MOVEMENT_SPEED * delta_time;
		ship.shape[1].position.y -= MOVEMENT_SPEED * delta_time;
		ship.shape[2].position.y -= MOVEMENT_SPEED * delta_time;

		ship.reference_shape[0].position.y -= MOVEMENT_SPEED * delta_time;
		ship.reference_shape[1].position.y -= MOVEMENT_SPEED * delta_time;
		ship.reference_shape[2].position.y -= MOVEMENT_SPEED * delta_time;
	}
}
void move_ship_left() {
	Center_x = (ship.reference_shape[0].position.x +ship.reference_shape[1].position.x +ship.reference_shape[2].position.x) / 3;

	if (Center_x - MOVEMENT_SPEED * delta_time > 0) {
		ship.shape[0].position.x -= MOVEMENT_SPEED * delta_time;
		ship.shape[1].position.x -= MOVEMENT_SPEED * delta_time;
		ship.shape[2].position.x -= MOVEMENT_SPEED * delta_time;

		ship.reference_shape[0].position.x -= MOVEMENT_SPEED * delta_time;
		ship.reference_shape[1].position.x -= MOVEMENT_SPEED * delta_time;
		ship.reference_shape[2].position.x -= MOVEMENT_SPEED * delta_time;
	}
}
void move_ship_down() {
	Center_y = (ship.reference_shape[0].position.y +ship.reference_shape[1].position.y +ship.reference_shape[2].position.y) / 3;

	if (Center_y + MOVEMENT_SPEED * delta_time < WINDOW_HEIGHT) {
		ship.shape[0].position.y += MOVEMENT_SPEED * delta_time;
		ship.shape[1].position.y += MOVEMENT_SPEED * delta_time;
		ship.shape[2].position.y += MOVEMENT_SPEED * delta_time;

		ship.reference_shape[0].position.y += MOVEMENT_SPEED * delta_time;
		ship.reference_shape[1].position.y += MOVEMENT_SPEED * delta_time;
		ship.reference_shape[2].position.y += MOVEMENT_SPEED * delta_time;
	}
}
void move_ship_right() {
	Center_x = (ship.reference_shape[0].position.x +ship.reference_shape[1].position.x +ship.reference_shape[2].position.x) / 3;

	if (Center_x + MOVEMENT_SPEED * delta_time < WINDOW_WIDTH) {
		ship.shape[0].position.x += MOVEMENT_SPEED * delta_time;
		ship.shape[1].position.x += MOVEMENT_SPEED * delta_time;
		ship.shape[2].position.x += MOVEMENT_SPEED * delta_time;
		
		ship.reference_shape[0].position.x += MOVEMENT_SPEED * delta_time;
		ship.reference_shape[1].position.x += MOVEMENT_SPEED * delta_time;
		ship.reference_shape[2].position.x += MOVEMENT_SPEED * delta_time;
	}
}

void set_key_active(unsigned int key) {
	const unsigned int size = sizeof(key_state) / sizeof(key_state[0]);
	for (unsigned int i = 0; i < size; ++i) {
		if (key == key_state[i].key) {
			key_state[i].active = 1;
		}
	}
}

void set_key_inactive(unsigned int key) {
	const unsigned int size = sizeof(key_state) / sizeof(key_state[0]);
	for (unsigned int i = 0; i < size; ++i) {
		if (key == key_state[i].key) {
			key_state[i].active = 0;
		}
	}
}

bool is_key_active(unsigned int key) {
	bool is_active = false;
	
	const unsigned int size = sizeof(key_state) / sizeof(key_state[0]);
	for (unsigned int i = 0; i < size; ++i) {
		if (key == key_state[i].key) {
			is_active = key_state[i].active;
		}
	}

	return is_active;
}

SDL_Point get_player_pos() {
	SDL_Point player_pos = {
		.x = Center_x,
		.y = Center_y
	};
	return player_pos;
}

SDL_Point get_mouse_pos() {
	SDL_Point mouse_pos = {
		.x = Mouse_x,
		.y = Mouse_y
	};
	return mouse_pos;
}

float get_player_rot() {
	return Player_rotation_angle;
}

void flush_keystate() {
	for (unsigned int i = 0; i < 5; ++i) {
		key_state[i].active = false;
	}
}

void flush_mousepos() {
	Mouse_x = WINDOW_WIDTH / 2.0f;
	Mouse_y = WINDOW_HEIGHT / 2.0f;
}
 void flush_playerpos() {
	Center_x = WINDOW_WIDTH / 2.0f;;
	Center_y = WINDOW_HEIGHT / 2.0f;;
 }
