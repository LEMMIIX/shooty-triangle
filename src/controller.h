#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "c_map.h"
#include "player.h"

extern struct map_uint_bool key_state[];
extern float Center_x;
extern float Center_y;

void turn_the_thing(float* x, float* y);

void move_ship_up();
void move_ship_left();
void move_ship_down();
void move_ship_right();

void set_key_active(unsigned int);
void set_key_inactive(unsigned int);
bool is_key_active(unsigned int);
void flush_keystate();
void flush_mousepos();
void flush_playerpos();


#endif
