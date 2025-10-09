#include <SDL3/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h>

#include "ammo.h"
#include "SDL3/SDL_render.h"
#include "common.h"

#define MAX_BULLETS 128

SDL_Rect basic_shape = {
		.h = 15.0f,
		.w = 15.0f,
		.x = 0.0f,
		.y = 0.0f,
};

SDL_Rect expl_shape = {
		.h = 40.0f,
		.w = 40.0f,
		.x = 0.0f,
		.y = 0.0f,
};

struct bullets_manager* bm = NULL;

int bullets_manager_init() {
	bm = malloc(sizeof(struct bullets_manager) + sizeof(Bullet*) * MAX_BULLETS);
	bm->size = 0;
	bm->last_freed_index = 0;
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		bm->live_bullets[i] = NULL;
	}

	return 0;
}

void create_bullet(float x, float y, Bullet_Type type) {
	if (bm == NULL) {
		printf("NO BULLET MANAGER. No bullet was created.\n");
		return;
	}


	Bullet* new_bullet = malloc(sizeof(Bullet));
	new_bullet->type = type;

	if (type == BASIC) {
		new_bullet->shape.rect.h = basic_shape.h;	
		new_bullet->shape.rect.w = basic_shape.w;	
		new_bullet->shape.rect.x = x - (basic_shape.w / 2.0f);	
		new_bullet->shape.rect.y = y - (basic_shape.y / 2.0f);	

	} else if (type == EXPLOSIVE) {
		new_bullet->shape.rect.h = expl_shape.h;	
		new_bullet->shape.rect.w = expl_shape.w;	
		new_bullet->shape.rect.x = x - (expl_shape.w / 2.0f);	
		new_bullet->shape.rect.y = y - (expl_shape.y / 2.0f);	
	}

	// eventually use bm->last_freed_index to maybe optimize the free-space check
	// could also use a random num gen to randomly suggest an index between [0,127] on first iteration lol
	for (unsigned int i = 0; i < /*bm->size*/ MAX_BULLETS; ++i) {
		if (bm->live_bullets[i] == NULL) {
			bm->live_bullets[i] = new_bullet;
			break;
		} 
	}
	
}

void update_bullets() {
	unsigned int bs = 0;
	for (unsigned int i = 0; i < /*bm->size*/ MAX_BULLETS; ++i) {
		if (bm->live_bullets[i] == NULL)  {
			continue;

		} else if (bm->live_bullets[i]->type == BASIC) {
			if (bm->live_bullets[i]->shape.rect.x < WINDOW_WIDTH) {
				bm->live_bullets[i]->shape.rect.x += 100 * delta_time;
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_RenderRect(renderer, &(bm->live_bullets[i]->shape.rect));

			} else {
				free(bm->live_bullets[i]);
				bm->live_bullets[i] = NULL;
			}
		} else if (bm->live_bullets[i]->type == EXPLOSIVE) {
			if (bm->live_bullets[i]->shape.rect.x < WINDOW_WIDTH) {
				bm->live_bullets[i]->shape.rect.x += 70 * delta_time;
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				SDL_RenderRect(renderer, &(bm->live_bullets[i]->shape.rect));

			} else {
				free(bm->live_bullets[i]);
				bm->live_bullets[i] = NULL;
			}
		} 
	}
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		bs += bm->live_bullets[i] != NULL 
			? 1
			: 0;
	}
	printf("\33[2K\rbullets: %u", bs);
}

int free_all_bullets() {
	unsigned int bs = 0;
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		free(bm->live_bullets[i]);
		bm->live_bullets[i] = NULL;
	}
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		bs += bm->live_bullets[i] != NULL 
			? 1
			: 0;
	}
	printf("remaining dangling bullets: %u", bs);
	return 0;
}
