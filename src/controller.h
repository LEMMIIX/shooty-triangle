#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "c_map.h"

extern struct map_uint_bool key_state[];
extern float Center_x;
extern float Center_y;

void turn_the_thing(float* x, float* y, SDL_Vertex obj[]);
float move_the_thing(SDL_Event event, SDL_Vertex obj[]);

void move_up(SDL_Vertex* obj);
void move_left(SDL_Vertex* obj);
void move_down(SDL_Vertex* obj);
void move_right(SDL_Vertex* obj);

void set_key_active(unsigned int);
void set_key_inactive(unsigned int);
bool is_key_active(unsigned int);


#endif
