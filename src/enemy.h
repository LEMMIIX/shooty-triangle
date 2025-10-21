#ifndef ENEMY_H
#define ENEMY_H

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "S_shapes.h"
#include "ammo.h"

#define MAX_ENEMIES 128

extern struct bullets_manager* get_bullets_manager();

typedef struct enemy {
	float center_x;
	float center_y;

	float direction_vec_x;
	float direction_vec_y;

	float health;
	
	union {
		S_Hex hex;
	} shape;
} Enemy;

struct enemy_manager {
	unsigned int size;
	Enemy* live_enemies[];
};

int enemy_manager_init();
SDL_FPoint calc_enemy_turn_angle(float x, float y);

/**
 * !
 * creates 1 heap allocated pointer, that MUST be free'd:
 * new_enemy
 */
void create_enemy();

/**
 * updates enemy movement of all active enemies
 * also frees all inactive enemies, as mentioned in create_enemy
 */
void update_enemies(struct bullets_manager* bullets_manager);

int free_all_enemies();

struct enemy_manager* get_enemy_manager();

#endif
