#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_rect.h>
#include <stdbool.h>

#include "font_and_text.h"
#include "common.h"

TTF_Font* font;
SDL_Color font_color;
SDL_Surface* font_surface;
SDL_FRect font_rect;

#define FONT_SIZE 12
#define PATH_FONT "../resources/fonts/" FONT

bool font_init() {
	const char* font_path = PATH_FONT;
	TTF_Init();
	font = TTF_OpenFont(font_path, FONT_SIZE);
	if (font == 0) {
		return 0;
	} else {
		font_color.r = 255;
		font_color.g = 255;
		font_color.b = 255;
		font_color.a = 255;
		return 1;
	}
}

void print_text_to_screen(const char* string, int x, int y, SDL_Renderer* renderer) {
	font_surface = TTF_RenderText_Solid_Wrapped(font, string, 0, font_color, 0);

	SDL_Texture* font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);

	font_rect.x = x;
	font_rect.y = y;
	font_rect.h = font_surface->h;
	font_rect.w = font_surface->w;

	SDL_RenderTexture(renderer, font_texture, NULL, &font_rect);
	SDL_DestroySurface(font_surface);
	SDL_DestroyTexture(font_texture);
}

void kill_font() {
	TTF_CloseFont(font);
}
