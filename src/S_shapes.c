#include <math.h>

#include "S_shapes.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

#define DEG_TO_RAD (M_PI / 180)

bool S_RenderHex(SDL_Renderer *renderer, S_Hex *hex) {
	SDL_FPoint points[7];

	for (int i = 0; i < 6; ++i) {
		float angle = (i * 60.0f) * DEG_TO_RAD;
		points[i].x = hex->x + hex->radians * cosf(angle);
		points[i].y = hex->y + hex->radians * sinf(angle);
	}
	points[6] = points[0];

	return SDL_RenderLines(renderer, points, 7);
}
