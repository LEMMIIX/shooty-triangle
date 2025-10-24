#include "player.h"
#include "SDL3/SDL_render.h"
#include <math.h>

#include "common.h"
#include "enemy.h"

void create_player(Player_ship* player) {
	//center
	player->shape[0].position.x = WINDOW_WIDTH / 2.0f + 30;
	player->shape[0].position.y = WINDOW_HEIGHT / 2.0;
	player->shape[0].color.r = 1.0;
	player->shape[0].color.g = 0.0;
	player->shape[0].color.b = 0.0;
	player->shape[0].color.a = 1.0;

	//top
	player->shape[1].position.x = WINDOW_WIDTH / 2.0f - 20;
	player->shape[1].position.y = WINDOW_HEIGHT / 2.0 - 20;
	player->shape[1].color.r = 1.0;
	player->shape[1].color.g = 0.0;
	player->shape[1].color.b = 0.0;
	player->shape[1].color.a = 1.0;

	//right
	player->shape[2].position.x = WINDOW_WIDTH / 2.0f - 20;
	player->shape[2].position.y = WINDOW_HEIGHT / 2.0 + 20;
	player->shape[2].color.r = 1.0;
	player->shape[2].color.g = 0.0;
	player->shape[2].color.b = 0.0;
	player->shape[2].color.a = 1.0;



	//center
	player->reference_shape[0].position.x = WINDOW_WIDTH / 2.0f + 30;
	player->reference_shape[0].position.y = WINDOW_HEIGHT / 2.0;
	player->reference_shape[0].color.r = 1.0;
	player->reference_shape[0].color.g = 0.0;
	player->reference_shape[0].color.b = 0.0;
	player->reference_shape[0].color.a = 1.0;

	//top
	player->reference_shape[1].position.x = WINDOW_WIDTH / 2.0f - 20;
	player->reference_shape[1].position.y = WINDOW_HEIGHT / 2.0 - 20;
	player->reference_shape[1].color.r = 1.0;
	player->reference_shape[1].color.g = 0.0;
	player->reference_shape[1].color.b = 0.0;
	player->reference_shape[1].color.a = 1.0;

	//rightreference_
	player->reference_shape[2].position.x = WINDOW_WIDTH / 2.0f - 20;
	player->reference_shape[2].position.y = WINDOW_HEIGHT / 2.0 + 20;
	player->reference_shape[2].color.r = 1.0;
	player->reference_shape[2].color.g = 0.0;
	player->reference_shape[2].color.b = 0.0;
	player->reference_shape[2].color.a = 1.0;



	player->health = 100;
	player->score = 0;
}

bool got_hit_by_enemy(struct enemy_manager* EM) {
	bool hit = false;
	for (unsigned int i= 0; i < MAX_ENEMIES && !hit; ++i) {
		if (EM->live_enemies[i] == NULL) {
			continue;
		}

		SDL_Point player_pos = get_player_pos();
		Enemy* enemy = EM->live_enemies[i];
		float distance = sqrtf(
				powf(player_pos.x - enemy->center_x, 2) +
				powf(player_pos.y - enemy->center_y, 2)
				);
		hit = distance <= enemy->shape.hex.radians + 15;
	}
	return hit;
}
