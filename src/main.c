#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#ifdef _WIN32
	#include <windows.h>
	#include <io.h>
	#include <fcntl.h>
#endif

#include "common.h"
#include "controller.h"
#include "player.h"
#include "font_and_text.h"
#include "ammo.h"
#include "enemy.h"

static const char* signature = R"(
                                                  
                               ___      _⎽⎽⎼⎼⎼⎺⎺‾‾‾‾‾‾‾⎺⎺⎻⎻⎼⎼⎽⎽_
                           _⎼⎺‾LCM‾\  /‾########################⎺⎺⎻⎻⎼⎼⎽⎽_
                         /##########\|######## |    #####################‾\
                        /######\#####\#########\_           ###############\
                       |#######|######\___########\          #/#############|
                        \##### | ##################\ _⎽⎽⎼⎼⎺⎺‾############### \
          _-_-_         /####  \   ##############   |#####################   |
          // // ‾⎺⎻(|)⎽⎽####  /\      ##########     /###################    |
          \| \|  ‾⎺⎻⎻⎼⎼⎽     /   \                   /##################     |
           _⎽⎽⎼⎼⎻    (O/    /     \                /################        /
           ⎺⎻__ ⎽ _⎽⎽=⎻⎻⎺⎺‾        ⎺⎻⎻⎼⎼⎽⎽_⎽⎽⎼⎼⎼⎺⎺##############  _⎽⎽⎼⎼⎻⎻⎺⎺‾
                                         /###############⎼⎼⎻⎻⎺⎺‾##‾⎺⎺⎻⎻⎼⎼_
                                        |#################################‾‾⎺⎺⎻⎻⎼_
                                         \######################################  \
                                           \###############____⎽⎽⎼_     ########   |
                                             ‾⎺⎺⎻⎻⎻⎻⎺⎺‾‾‾‾‾⎼⎼⎻⎻⎺⎺‾‾‾###########   /
                                                         ⎺⎺⎺⎺⎺⎺⎺⎺‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾


)";

float delta_time = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

static bool vsync_enabled = 1;

struct bullets_manager* BM = NULL;
struct enemy_manager* EM = NULL;

SDL_Vertex reference_triangle[3];

#define ENEMY_SPAWN_INTERVAL 1.0f
float enemy_timer = 0.0f;

Game_state game_state;

int main() {
	// to correctly render UNICODE chars in the terminal on windows, mainly for the awesome signature
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
	printf("%s\n", signature);

	printf("starting program\n");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("sdl ini failed, error: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	} else {
		printf("hello video\n");
	}

	// TODO split up into CreateWindow and CreateRenderer to provide SDL_RendererFlags
	if (!SDL_CreateWindowAndRenderer("shooty triangle", WINDOW_WIDTH, WINDOW_HEIGHT,
				SDL_WINDOW_ALWAYS_ON_TOP,
				&window, &renderer)) {
		printf("sdl window and renderer creation failed, error: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	} else {
		printf("hello window and renderer\n");
	}

	if (!SDL_SetRenderVSync(renderer, vsync_enabled)) {
		printf("VSync could not be enabled, error: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!font_init()) {
		printf("font %s not found\n", FONT);
		return SDL_APP_FAILURE;
	} else {
		printf("using font: %s\n", FONT);
	}

	if (bullets_manager_init() != 0) {
		printf("bullet manager not initialized\n");
		return SDL_APP_FAILURE;
	}
	printf("got BM\n");
	if (enemy_manager_init() != 0) {
		printf("enemy manager not initialized\n");
		return SDL_APP_FAILURE;
	}
	printf("got EM\n");

	printf("creating ship\n");
	SDL_Vertex ship[3];
	create_player(ship);

	create_player(reference_triangle);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	printf("starting game loop\n");
	float mouse_pos_x;
	float mouse_pos_y;

	const char* controls ="ESC - (un-)pause\nQ - quit\nW - move UP\nA - move LEFT\nS - move DOWN\nD - move RIGHT\nMOUSE - rotate\nMOUSE [left] - small bullet\nMOUSE [right] - big bullet\n";
	const char* controls_esc = "ESC - (un-)pause\n";
	const char* paused_text = "G A M E  P A U S E D";
	SDL_Color paused_color = {
		.r = 255,
		.g = 0,
		.b = 0,
		.a = 255
	};
	const char* gameover_text = "G A M E  O V E R";
	const char* gameover_controls = "R - restart\nQ - quit\n";

	Uint64 last_tick = SDL_GetTicks();
	Uint64 frame_count = 0;
	Uint64 current_tick;
	float frames_per_second = 0.0f;
	char frames_string[100] = "0.000";

	Uint64 last_delta_tick = SDL_GetTicks();

	// TODO refactor to return running only when init of screen etc ist successful
	game_state = RUNNING;

	SDL_Event event;
	while(game_state != QUIT) {
		if (game_state == RUNNING) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT) {
					printf("quit event hit\n");
					game_state = QUIT;

				} else if(event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE) {
					game_state = PAUSED;

				} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
					SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
					turn_the_thing(&mouse_pos_x, &mouse_pos_y, ship);

				} else if (event.type == SDL_EVENT_KEY_DOWN) {
					set_key_active(event.key.key);

				} else if (event.type == SDL_EVENT_KEY_UP) {
					set_key_inactive(event.key.key);
				} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
					SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
					create_bullet(BASIC);
				} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT) {
					SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
					create_bullet(EXPLOSIVE);
				}
			}

			if (is_key_active(SDLK_W)) {
				move_up(ship);
				move_up(reference_triangle);
			}
			if (is_key_active(SDLK_A)) {
				move_left(ship);
				move_left(reference_triangle);
			}
			if (is_key_active(SDLK_S)) {
				move_down(ship);
				move_down(reference_triangle);
			}
			if (is_key_active(SDLK_D)) {
				move_right(ship);
				move_right(reference_triangle);
			}
			if (is_key_active(SDLK_Q)) {
				game_state = QUIT;
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_RenderGeometry(renderer, NULL, ship, 3, NULL, 0);

			//fps count
			++frame_count;
			current_tick = SDL_GetTicks();
			if (current_tick - last_tick >= 1000) {
				frames_per_second = frame_count * 1000.0f / (current_tick - last_tick);
				snprintf(frames_string, 99, "vsync: %i, fps: %.3f", vsync_enabled, frames_per_second);

				frame_count = 0;
				last_tick = current_tick;
			}
			print_text_to_screen(frames_string, WINDOW_WIDTH / 2, 0, renderer);
			print_text_to_screen(controls_esc, 0, 0, renderer);

			if (enemy_timer >= ENEMY_SPAWN_INTERVAL) {
				create_enemy();
				enemy_timer = 0;
			}

			update_bullets();
			update_enemies(BM);

			SDL_RenderPresent(renderer);


			// delta calculation
			delta_time = (current_tick - last_delta_tick) / 1000.0f;
			last_delta_tick = current_tick;

			enemy_timer += delta_time;

			if (got_hit_by_enemy(EM)) {
				game_state = GAME_OVER;
			}

		} else if (game_state == PAUSED) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT || event.key.key == SDLK_Q) {
					printf("quit event hit\n");
					game_state = QUIT;

				} else if(event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE) {
					game_state = RUNNING;
					last_delta_tick = SDL_GetTicks();
				}
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_RenderGeometry(renderer, NULL, ship, 3, NULL, 0);

			print_text_to_screen_with_color(paused_text, ((WINDOW_WIDTH / 2.0f) - 60), WINDOW_HEIGHT / 2.0f, renderer, paused_color);
			print_text_to_screen(controls, 0, 0, renderer);

			SDL_RenderPresent(renderer);

		} else if (game_state == GAME_OVER) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT || event.key.key == SDLK_Q) {
					printf("quit event hit\n");
					game_state = QUIT;

				} else if(event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_R) {
					free_all_bullets();
					free_all_enemies();

					if (bullets_manager_init() != 0) {
						printf("bullet manager not initialized\n");
						return SDL_APP_FAILURE;
					}
					printf("got BM\n");
					if (enemy_manager_init() != 0) {
						printf("enemy manager not initialized\n");
						return SDL_APP_FAILURE;
					}
					printf("got EM\n");

					create_player(ship);
					create_player(reference_triangle);

					flush_keystate();
					flush_mousepos();
					flush_playerpos();

					game_state = RUNNING;
					last_delta_tick = SDL_GetTicks();
				}
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			print_text_to_screen_with_color(gameover_text, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2, renderer, paused_color);
			print_text_to_screen(gameover_controls, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 + 20, renderer);

			SDL_RenderPresent(renderer);
		}
	}

	printf("ending program\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	kill_font();
	free_all_bullets();
	free_all_enemies();

	return 0;
}
