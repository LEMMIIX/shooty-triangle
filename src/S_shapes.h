#ifndef S_SHAPES_H
#define S_SHAPES_H

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

typedef struct S_Hex {
	float x;
	float y;
	float radians;
} S_Hex;

bool S_RenderHex(SDL_Renderer* renderer, S_Hex* hex);


#endif
