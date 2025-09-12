#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"
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

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

SDL_Vertex reference_triangle[3];

int main() {
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

	if (!SDL_CreateWindowAndRenderer("shooty triangle", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		printf("sdl window and renderer creation failed, error: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	} else {
		printf("hello window and renderer\n");
	}

	printf("creating ship\n");
	SDL_Vertex ship[3];
	create_player(ship);

	create_player(reference_triangle);

	SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
	SDL_RenderClear(renderer);

	printf("starting game loop\n");
	float mouse_pos_x;
	float mouse_pos_y;
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				printf("quit event hit\n");
				quit = true;
			} else {
				switch (event.type) {
					case SDL_EVENT_MOUSE_MOTION:
						//printf("getting mouse state...\n");
						SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
						//printf("turning the thing\n");
						turn_the_thing(&mouse_pos_x, &mouse_pos_y, ship);
						break;
					default:
						printf("unhandled event");
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderGeometry(renderer, NULL, ship, 3, NULL, 0);

		SDL_RenderPresent(renderer);
	}

	printf("ending program\n");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
