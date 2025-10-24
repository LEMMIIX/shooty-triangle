#ifndef PLAYER_H
#define PLAYER_H

#include "SDL3/SDL_render.h"

extern struct enemy_manager* get_enemy_manager();

typedef struct player_ship {
	SDL_Vertex shape[3];
	SDL_Vertex reference_shape[3];
	int health;
	int score;
} Player_ship;

void create_player(Player_ship* player);
bool got_hit_by_enemy(struct enemy_manager* EM);

#endif
