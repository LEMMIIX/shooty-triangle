#ifndef AMMO_H
#define AMMO_H

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
typedef enum bullet_type{
	BASIC,
	EXPLOSIVE,
} Bullet_Type;

typedef struct bullet{
	Bullet_Type type;
	
	float center_x;
	float center_y;

	float direction_vec_x;
	float direction_vec_y;
	
	union {
		SDL_Vertex vert;
		SDL_FRect rect;
	} shape;
} Bullet;

struct bullets_manager {
	unsigned int size;
	unsigned int last_freed_index;
	Bullet* live_bullets[];
};

int bullets_manager_init();
SDL_FPoint calc_bullet_turn_angle();

/**
 * creates new bullet from Bullet_Type
 * !
 * creates 1 heap allocated pointer, that MUST be free'd:
 * new_bullet
 */
void create_bullet(Bullet_Type type);

/**
 * updates bullet movement of all active bullets
 * also frees all inactive bullets, as mentioned in create_bullet
 */
void update_bullets();

int free_all_bullets();

#endif
