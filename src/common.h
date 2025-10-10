#ifndef COMMON_H
#define COMMON_H

#include "SDL3/SDL_render.h"
#include "c_map.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FONT "terminal-f4.ttf"
#define MOVEMENT_SPEED 200.0f

extern float delta_time;
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern SDL_Vertex reference_triangle[];

SDL_Point get_player_pos();
SDL_Point get_mouse_pos();
float get_player_rot();

#endif
