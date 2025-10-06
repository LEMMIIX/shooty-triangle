#include <math.h>
#include <stdio.h>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_rect.h"

#include "SDL3/SDL_render.h"
#include "c_map.h"
#include "common.h"
#include "player.h"
#include "controller.h"

struct map_uint_bool key_state[] = {
	{SDLK_W, 0},
	{SDLK_A, 0},
	{SDLK_S, 0},
	{SDLK_D, 0},
};

float move_the_thing(SDL_Event event, SDL_Vertex obj[]) { return 0; }

void turn_the_thing(float* mouse_x, float* mouse_y, SDL_Vertex *obj) {
	float center_x = (reference_triangle[0].position.x +reference_triangle[1].position.x +reference_triangle[2].position.x) / 3;
	float center_y = (reference_triangle[0].position.y +reference_triangle[1].position.y +reference_triangle[2].position.y) / 3;
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

		//printf("obj[%d].position.x = %f\n", i, obj[i].position.x);
		//printf("obj[%d].position.y = %f\n", i, obj[i].position.y);
	}
	//printf("mouse_x position = %f, ", *mouse_x);
	//printf("mouse_y position = %f\n", *mouse_y);
}

void move_up(SDL_Vertex* obj, float movement_speed) {
	obj[0].position.y -= movement_speed;
	obj[1].position.y -= movement_speed;
	obj[2].position.y -= movement_speed;
}
void move_left(SDL_Vertex* obj, float movement_speed) {
	obj[0].position.x -= movement_speed;
	obj[1].position.x -= movement_speed;
	obj[2].position.x -= movement_speed;
}
void move_down(SDL_Vertex* obj, float movement_speed) {
	obj[0].position.y += movement_speed;
	obj[1].position.y += movement_speed;
	obj[2].position.y += movement_speed;
}
void move_right(SDL_Vertex* obj, float movement_speed) {
	obj[0].position.x += movement_speed;
	obj[1].position.x += movement_speed;
	obj[2].position.x += movement_speed;
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
