#include "SDL3/SDL_render.h"

#include "common.h"

void create_player(SDL_Vertex triangle[]) {
	//center
	triangle[0].position.x = WINDOW_WIDTH / 2.0f + 30;
	triangle[0].position.y = WINDOW_HEIGHT / 2.0;
	triangle[0].color.r = 1.0;
	triangle[0].color.g = 0.0;
	triangle[0].color.b = 0.0;
	triangle[0].color.a = 1.0;

	//top
	triangle[1].position.x = WINDOW_WIDTH / 2.0f - 20;
	triangle[1].position.y = WINDOW_HEIGHT / 2.0 - 20;
	triangle[1].color.r = 1.0;
	triangle[1].color.g = 0.0;
	triangle[1].color.b = 0.0;
	triangle[1].color.a = 1.0;

	//right
	triangle[2].position.x = WINDOW_WIDTH / 2.0f - 20;
	triangle[2].position.y = WINDOW_HEIGHT / 2.0 + 20;
	triangle[2].color.r = 1.0;
	triangle[2].color.g = 0.0;
	triangle[2].color.b = 0.0;
	triangle[2].color.a = 1.0;
}
