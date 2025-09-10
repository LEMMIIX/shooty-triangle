#include <math.h>
#include <stdio.h>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_rect.h"

#include "SDL3/SDL_render.h"
#include "common.h"
#include "player.h"
#include "controller.h"

float move_the_thing(SDL_Event event, SDL_Vertex obj[]) { return 0; }

void turn_the_thing(float* mouse_x, float* mouse_y, SDL_Vertex *obj) {
	float center_x = (reference_triangle[0].position.x +reference_triangle[1].position.x +reference_triangle[2].position.x) / 3;
	float center_y = (reference_triangle[0].position.y +reference_triangle[1].position.y +reference_triangle[2].position.y) / 3;
	float vector_x = *mouse_x - center_x;
	float vector_y = *mouse_y - center_y;

	float rotation_angle = atan2f(vector_y, vector_x);
	printf("rotation_angle = %f radians (%f degrees)\n", rotation_angle, rotation_angle * 180.0f / M_PI);
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
