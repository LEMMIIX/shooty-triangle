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

float center_x = WINDOW_WIDTH / 2.0f;
float center_y = WINDOW_HEIGHT / 2.0f;

float move_the_thing(SDL_Event event, SDL_Vertex obj[]) { return 0; }

void turn_the_thing(float* mouse_x, float* mouse_y, SDL_Vertex *obj) {
	float vector_x = *mouse_x - center_x;
	float vector_y = *mouse_y - center_y;

	float rotation_angle = atan2f(vector_y, vector_x);
	//printf("\rrotation_angle = %f radians (%f degrees)", rotation_angle, rotation_angle * 180.0f / M_PI);
	fflush(stdout);
	for (int i = 0; i < 3; ++i) {
		float x = reference_triangle[i].position.x - center_x;
		float y = reference_triangle[i].position.y - center_y;

		float x_rotated = x * cos(rotation_angle) - y * sin(rotation_angle);
		float y_rotated = x * sin(rotation_angle) + y * cos(rotation_angle);

		obj[i].position.x = x_rotated + center_x;
		obj[i].position.y = y_rotated + center_y;
	}
	//printf("mouse_x position = %f, ", *mouse_x);
	//printf("mouse_y position = %f\n", *mouse_y);
}

void move_up(SDL_Vertex* obj, float movement_speed) {
	center_y = (reference_triangle[0].position.y +reference_triangle[1].position.y +reference_triangle[2].position.y) / 3;
	
	if (center_y - movement_speed > 0) {
		obj[0].position.y -= movement_speed;
		obj[1].position.y -= movement_speed;
		obj[2].position.y -= movement_speed;
	}
}
void move_left(SDL_Vertex* obj, float movement_speed) {
	center_x = (reference_triangle[0].position.x +reference_triangle[1].position.x +reference_triangle[2].position.x) / 3;

	if (center_x - movement_speed > 0) {
		obj[0].position.x -= movement_speed;
		obj[1].position.x -= movement_speed;
		obj[2].position.x -= movement_speed;
	}
}
void move_down(SDL_Vertex* obj, float movement_speed) {
	center_y = (reference_triangle[0].position.y +reference_triangle[1].position.y +reference_triangle[2].position.y) / 3;

	if (center_y + movement_speed < WINDOW_HEIGHT) {
		obj[0].position.y += movement_speed;
		obj[1].position.y += movement_speed;
		obj[2].position.y += movement_speed;
	}
}
void move_right(SDL_Vertex* obj, float movement_speed) {
	center_x = (reference_triangle[0].position.x +reference_triangle[1].position.x +reference_triangle[2].position.x) / 3;

	if (center_x < WINDOW_WIDTH) {
		obj[0].position.x += movement_speed;
		obj[1].position.x += movement_speed;
		obj[2].position.x += movement_speed;
	}
}

void set_key_active(unsigned int key) {
	const unsigned int size = sizeof(key_state) / sizeof(key_state[0]);
	for (int i = 0; i < size; ++i) {
		if (key == key_state[i].key) {
			key_state[i].active = 1;
		}
	}
}

void set_key_inactive(unsigned int key) {
	const unsigned int size = sizeof(key_state) / sizeof(key_state[0]);
	for (int i = 0; i < size; ++i) {
		if (key == key_state[i].key) {
			key_state[i].active = 0;
		}
	}
}

bool is_key_active(unsigned int key) {
	bool is_active = false;
	
	const unsigned int size = sizeof(key_state) / sizeof(key_state[0]);
	for (int i = 0; i < size; ++i) {
		if (key == key_state[i].key) {
			is_active = key_state[i].active;
		}
	}

	return is_active;
}
