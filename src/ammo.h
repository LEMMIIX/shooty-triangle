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

/** initilizes the bullets_manager
 */
int bullets_manager_init();

/**
 * creates new bullet from Bullet_Type
 * !
 * creates 1 heap allocated pointer, that MUST be free'd:
 * new_bullet
 */
void create_bullet(float x, float y, Bullet_Type type);

/**
 * updates bullet movement of all active bullets
 * also frees all inactive bullets, as mentioned in create_bullet
 */
void update_bullets();

int free_all_bullets();

#endif
