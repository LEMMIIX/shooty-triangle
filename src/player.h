#ifndef PLAYER_H
#define PLAYER_H

#include "SDL3/SDL_render.h"

extern struct enemy_manager* get_enemy_manager();

void create_player(SDL_Vertex triangle[]);
bool got_hit_by_enemy(struct enemy_manager* EM);

#endif
