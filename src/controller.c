#include <math.h>
#include <stdio.h>
#include "SDL3/SDL_rect.h"

#include "SDL3/SDL_render.h"
#include "c_map.h"
#include "common.h"
#include "controller.h"

struct map_uint_bool key_state[] = {
	{SDLK_W, 0},	// move up
	{SDLK_A, 0},	// move down
	{SDLK_S, 0},	// move left
	{SDLK_D, 0},	// move right
	{SDLK_Q, 0},	// quit
};

float Center_x = WINDOW_WIDTH / 2.0f;
float Center_y = WINDOW_HEIGHT / 2.0f;

float Mouse_x = WINDOW_WIDTH / 2.0f;
float Mouse_y = WINDOW_HEIGHT / 2.0f;

float Player_rotation_angle = 0.0f;

void turn_the_thing(float* mouse_x, float* mouse_y, SDL_Vertex *obj) {
	mouse_x = mouse_x;
	mouse_y = mouse_y;

	float vector_x = *mouse_x - Center_x;
	float vector_y = *mouse_y - Center_y;

	Player_rotation_angle = atan2f(vector_y, vector_x);
	//printf("\rrotation_angle = %f radians (%f degrees)", Player_rotation_angle, Player_rotation_angle * 180.0f / M_PI);
	//fflush(stdout);
	for (int i = 0; i < 3; ++i) {
		float x = reference_triangle[i].position.x - Center_x;
		float y = reference_triangle[i].position.y - Center_y;

		float x_rotated = x * cos(Player_rotation_angle) - y * sin(Player_rotation_angle);
		float y_rotated = x * sin(Player_rotation_angle) + y * cos(Player_rotation_angle);

		obj[i].position.x = x_rotated + Center_x;
		obj[i].position.y = y_rotated + Center_y;
	}
	//printf("mouse_x position = %f, ", *mouse_x);
	//printf("mouse_y position = %f\n", *mouse_y);
}

void move_up(SDL_Vertex* obj) {
	Center_y = (reference_triangle[0].position.y +reference_triangle[1].position.y +reference_triangle[2].position.y) / 3;
	
	if (Center_y - MOVEMENT_SPEED * delta_time > 0) {
		obj[0].position.y -= MOVEMENT_SPEED * delta_time;
		obj[1].position.y -= MOVEMENT_SPEED * delta_time;
		obj[2].position.y -= MOVEMENT_SPEED * delta_time;
	}
}
void move_left(SDL_Vertex* obj) {
	Center_x = (reference_triangle[0].position.x +reference_triangle[1].position.x +reference_triangle[2].position.x) / 3;

	if (Center_x - MOVEMENT_SPEED * delta_time > 0) {
		obj[0].position.x -= MOVEMENT_SPEED * delta_time;
		obj[1].position.x -= MOVEMENT_SPEED * delta_time;
		obj[2].position.x -= MOVEMENT_SPEED * delta_time;
	}
}
void move_down(SDL_Vertex* obj) {
	Center_y = (reference_triangle[0].position.y +reference_triangle[1].position.y +reference_triangle[2].position.y) / 3;

	if (Center_y + MOVEMENT_SPEED * delta_time < WINDOW_HEIGHT) {
		obj[0].position.y += MOVEMENT_SPEED * delta_time;
		obj[1].position.y += MOVEMENT_SPEED * delta_time;
		obj[2].position.y += MOVEMENT_SPEED * delta_time;
	}
}
void move_right(SDL_Vertex* obj) {
	Center_x = (reference_triangle[0].position.x +reference_triangle[1].position.x +reference_triangle[2].position.x) / 3;

	if (Center_x + MOVEMENT_SPEED * delta_time < WINDOW_WIDTH) {
		obj[0].position.x += MOVEMENT_SPEED * delta_time;
		obj[1].position.x += MOVEMENT_SPEED * delta_time;
		obj[2].position.x += MOVEMENT_SPEED * delta_time;
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
