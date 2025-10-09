#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "ammo.h"
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

float delta_time = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

static bool vsync_enabled = 1;

SDL_Vertex reference_triangle[3];


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

	printf("creating ship\n");
	SDL_Vertex ship[3];
	create_player(ship);

	create_player(reference_triangle);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	printf("starting game loop\n");
	float mouse_pos_x;
	float mouse_pos_y;

	float movement_speed = 200.0f;

	const char* controls ="Q - quit\nW - move UP\nA - move LEFT\nS - move DOWN\nD - move RIGHT\nMOUSE - rotate\nMOUSE [left] - small bullet\nMOUSE [right] - big bullet\n";
	
	Uint64 last_tick = SDL_GetTicks();
	Uint64 frame_count = 0;
	Uint64 current_tick;
	float frames_per_second = 0.0f;
	char frames_string[100] = "0.000";

	Uint64 last_delta_tick = SDL_GetTicks();

	bool quit = false;
	SDL_Event event;
	while(!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				printf("quit event hit\n");
				quit = true;

			} else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
				turn_the_thing(&mouse_pos_x, &mouse_pos_y, ship);
			
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				set_key_active(event.key.key);
			
			} else if (event.type == SDL_EVENT_KEY_UP) {
				set_key_inactive(event.key.key);
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
				create_bullet(mouse_pos_x, mouse_pos_y, BASIC);
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT) {
				SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
				create_bullet(mouse_pos_x, mouse_pos_y, EXPLOSIVE);
			}
		}

		if (is_key_active(SDLK_W)) {
			move_up(ship, movement_speed * delta_time);
			move_up(reference_triangle, movement_speed * delta_time);
		}
		if (is_key_active(SDLK_A)) {
			move_left(ship, movement_speed * delta_time);
			move_left(reference_triangle, movement_speed * delta_time);
		}
		if (is_key_active(SDLK_S)) {
			move_down(ship, movement_speed * delta_time);
			move_down(reference_triangle, movement_speed * delta_time);
		}
		if (is_key_active(SDLK_D)) {
			move_right(ship, movement_speed * delta_time);
			move_right(reference_triangle, movement_speed * delta_time);
		}
		if (is_key_active(SDLK_Q)) {
			quit = true;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderGeometry(renderer, NULL, ship, 3, NULL, 0);

		print_text_to_screen(controls, 0, 0, renderer);

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

		update_bullets();

		SDL_RenderPresent(renderer);


		// delta calculation
		delta_time = (current_tick - last_delta_tick) / 1000.0f;
		last_delta_tick = current_tick;
	}

	printf("ending program\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	kill_font();
	SDL_Quit();

	free_all_bullets();

	return 0;
}
