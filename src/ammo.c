#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ammo.h"
#include "common.h"
#include "player.h"


SDL_Rect Basic_shape = {
		.h = 15.0f,
		.w = 15.0f,
		.x = 0.0f,
		.y = 0.0f,
};

SDL_Rect Expl_shape = {
		.h = 40.0f,
		.w = 40.0f,
		.x = 0.0f,
		.y = 0.0f,
};

extern struct bullets_manager* BM;
extern  Player_ship ship;

int bullets_manager_init() {
	BM = malloc(sizeof(struct bullets_manager) + sizeof(Bullet*) * MAX_BULLETS);
	BM->size = 0;
	BM->last_freed_index = 0;
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		BM->live_bullets[i] = NULL;
	}

	return 0;
}

void create_bullet(Bullet_Type type) {
	if (BM == NULL) {
		printf("NO BULLET MANAGER. No bullet was created.\n");
		return;
	}

	SDL_Point player_pos = get_player_pos();
	SDL_FPoint direction_vec = calc_bullet_turn_angle();

	Bullet* new_bullet = malloc(sizeof(Bullet));
	new_bullet->type = type;
	new_bullet->direction_vec_x = direction_vec.x;
	new_bullet->direction_vec_y = direction_vec.y;

	if (type == BASIC) {
		new_bullet->shape.rect.h = Basic_shape.h;	
		new_bullet->shape.rect.w = Basic_shape.w;	
		new_bullet->shape.rect.x = player_pos.x - (Basic_shape.w / 2.0f);	
		new_bullet->shape.rect.y = player_pos.y - (Basic_shape.h / 2.0f);	
		new_bullet->center_x = player_pos.x;
		new_bullet->center_y = player_pos.y;

	} else if (type == EXPLOSIVE) {
		new_bullet->shape.rect.h = Expl_shape.h;	
		new_bullet->shape.rect.w = Expl_shape.w;	
		new_bullet->shape.rect.x = player_pos.x - (Expl_shape.w / 2.0f);	
		new_bullet->shape.rect.y = player_pos.y - (Expl_shape.h / 2.0f);	
		new_bullet->center_x = player_pos.x;
		new_bullet->center_y = player_pos.y;
	}

	// eventually use BM->last_freed_index to maybe optimize the free-space check
	// could also use a random num gen to randomly suggest an index between [0,127] on first iteration lol
	for (unsigned int i = 0; i < /*BM->size*/ MAX_BULLETS; ++i) {
		if (BM->live_bullets[i] == NULL) {
			BM->live_bullets[i] = new_bullet;
			break;
		} 
	}
	
}

SDL_FPoint calc_bullet_turn_angle() {
	float angle = get_player_rot();
	SDL_FPoint direction_vec = {
		.x = cosf(angle),
		.y = sinf(angle)
	};

	return direction_vec;
}

void update_bullets() {
	//unsigned int bs = 0;
	for (unsigned int i = 0; i < /*BM->size*/ MAX_BULLETS; ++i) {
		if (BM->live_bullets[i] == NULL)  {
			continue;
		}

		/*
		 * an offset is used on the border to prevent
		 * the player not being able to shoot because
		 * of how the projectile is created, it would
		 * be outside of the window sometimes. Might
		 * have to come up with a better solution.
		 * Maybe I could even use GetRenderViewportSize()
		 */
		Bullet* bullet = BM->live_bullets[i];
		if (bullet->shape.rect.x < -20 ||
				bullet->shape.rect.x > WINDOW_WIDTH + 20 ||
				bullet->shape.rect.y < -20 ||
				bullet->shape.rect.y >= WINDOW_HEIGHT + 20) {
			free(BM->live_bullets[i]);
			BM->live_bullets[i] = NULL;
			ship.score -= 50;

		} else {
			if (bullet->type == BASIC) {
				bullet->shape.rect.x += bullet->direction_vec_x * MOVEMENT_SPEED * delta_time * 2.0f;
				bullet->shape.rect.y += bullet->direction_vec_y * MOVEMENT_SPEED * delta_time * 2.0f;
				bullet->center_x = bullet->shape.rect.x + (bullet->shape.rect.w / 2);
				bullet->center_y = bullet->shape.rect.y - (bullet->shape.rect.h / 2);

				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_RenderRect(renderer, &(bullet->shape.rect));

			} else if (bullet->type == EXPLOSIVE) {
				bullet->shape.rect.x += bullet->direction_vec_x * MOVEMENT_SPEED * delta_time;
				bullet->shape.rect.y += bullet->direction_vec_y * MOVEMENT_SPEED * delta_time;
				bullet->center_x = bullet->shape.rect.x + (bullet->shape.rect.w / 2);
				bullet->center_y = bullet->shape.rect.y - (bullet->shape.rect.h / 2);

				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				SDL_RenderRect(renderer, &(bullet->shape.rect));
			}
		}
	}
	/*
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		bs += BM->live_bullets[i] != NULL;
	}
	printf("\33[2K\rbullets: %u", bs);
	*/
}

int free_all_bullets() {
	unsigned int bs = 0;
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		free(BM->live_bullets[i]);
		BM->live_bullets[i] = NULL;
	}
	for (unsigned int i = 0; i < MAX_BULLETS; ++i) {
		bs += BM->live_bullets[i] != NULL;
	}

	free(BM);
	BM = NULL;

	printf("remaining dangling bullets: %u\n", bs);
	return 0;
}

struct bullets_manager* get_bullets_manager() {
	return BM;
}

