#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "enemy.h"
#include "S_shapes.h"
#include "common.h"

#define MAX_ENEMIES 128
#define ENEMY_HEALTH 200
#define ENEMY_SPEED 70


S_Hex Asteroid = {
	.radians = 50,
	.x = 0.0f,
	.y = 0.0f
};

struct enemy_manager* EM = NULL;

int enemy_manager_init() {
	srand(time(NULL));

	EM = malloc(sizeof(struct enemy_manager) + sizeof(Enemy*) * MAX_ENEMIES);
	EM->size = 0;
	for (unsigned int i = 0; i < MAX_ENEMIES; ++i) {
		EM->live_enemies[i] = NULL;
	}
	
	return 0;
}

void create_enemy() {
	if (EM == NULL) {
		printf("NO ENEMY MANAGER. No enemy was created\n");
		return;
	}

	Enemy* new_enemy = malloc(sizeof(Enemy));
	new_enemy->shape.hex.radians = Asteroid.radians;
	new_enemy->health = ENEMY_HEALTH;

	/* determine the spawn position at
	 * the window border at random
	 * 0: top
	 * 1: right
	 * 2: bottom
	 * 3: left
	 */
	float border = (rand() % 361) % 4;

	if (border == 0) {
		new_enemy->center_x = (rand() % WINDOW_WIDTH + 1);
		new_enemy->center_y = 0;
	} else if (border == 1) {
		new_enemy->center_x = WINDOW_WIDTH;
		new_enemy->center_y = (rand() % WINDOW_HEIGHT + 1);
	} else if (border == 2) {
		new_enemy->center_x = (rand() % WINDOW_WIDTH + 1);
		new_enemy->center_y = WINDOW_HEIGHT;
	} else {
		new_enemy->center_x = 0;
		new_enemy->center_y = (rand() % WINDOW_HEIGHT + 1);
	}
	
	new_enemy->shape.hex.x = new_enemy->center_x;
	new_enemy->shape.hex.y = new_enemy->center_y;

	SDL_FPoint direction_vec = calc_enemy_turn_angle(new_enemy->center_x, new_enemy->center_y);
	new_enemy->direction_vec_x = direction_vec.x;
	new_enemy->direction_vec_y = direction_vec.y;

	for (unsigned int i = 0; i < MAX_ENEMIES; ++i) {
		if (EM->live_enemies[i] == NULL) {
			EM->live_enemies[i] = new_enemy;
			break;
		}
	}
}

SDL_FPoint calc_enemy_turn_angle(float x, float y) {
	SDL_Point player_pos = get_player_pos();
	
	float vector_x = player_pos.x - x;
	float vector_y = player_pos.y - y;

	float length = sqrtf(vector_x * vector_x + vector_y * vector_y);

	SDL_FPoint direction;
	if (length > 0) {
		direction.x = vector_x / length;
		direction.y = vector_y / length;
	}

	return direction;
}

void update_enemies() {
	unsigned int es = 0;
	for (unsigned int i = 0; i < MAX_ENEMIES; ++i) {
		if (EM->live_enemies[i] == NULL) {
			continue;
		}

		Enemy* enemy = EM->live_enemies[i];
		if (enemy->shape.hex.x < -200 ||
				enemy->shape.hex.x > WINDOW_WIDTH + 200 ||
				enemy->shape.hex.y < -200 ||
				enemy->shape.hex.y >= WINDOW_HEIGHT + 200) {
			free(EM->live_enemies[i]);
			EM->live_enemies[i] = NULL;
		} else {
			enemy->shape.hex.x += enemy->direction_vec_x * ENEMY_SPEED * delta_time;
			enemy->shape.hex.y += enemy->direction_vec_y * ENEMY_SPEED * delta_time;

			SDL_SetRenderDrawColor(renderer, 255, 255, 17, 255);
			S_RenderHex(renderer, &(enemy->shape.hex));
		}
	}
	
	for (unsigned int i = 0; i < MAX_ENEMIES; ++i) {
		es += EM->live_enemies[i] != NULL 
			? 1
			: 0;
	}
	printf("\33[2K\renemies: %u", es);
}

int free_all_enemies() {
	unsigned int es = 0;
	for (unsigned int i = 0; i < MAX_ENEMIES; ++i) {
		free(EM->live_enemies[i]);
		EM->live_enemies[i] = NULL;
	}
	for (unsigned int i = 0; i < MAX_ENEMIES; ++i) {
		es += EM->live_enemies[i] != NULL 
			? 1
			: 0;
	}
	printf("remaining dangling enemies: %u\n", es);
	return 0;
}
