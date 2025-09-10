#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"

extern SDL_Vertex reference_triangle[];

void turn_the_thing(float* x, float* y, SDL_Vertex obj[]);
float move_the_thing(SDL_Event event, SDL_Vertex obj[]);

#endif
