#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_rect.h>
#include <stdbool.h>

#include "font_and_text.h"
#include "common.h"

TTF_Font* Font;
SDL_Color Font_color;
SDL_Surface* Font_surface;
SDL_FRect Font_rect;

#define FONT_SIZE 12
// eventually create a byte array to embedd the font into the binaries
#define PATH_FONT "../resources/fonts/" FONT

bool font_init() {
	const char* font_path = PATH_FONT;
	TTF_Init();
	Font = TTF_OpenFont(font_path, FONT_SIZE);
	if (Font == 0) {
		return 0;
	} else {
		Font_color.r = 255;
		Font_color.g = 255;
		Font_color.b = 255;
		Font_color.a = 255;
		return 1;
	}
}

void print_text_to_screen(const char* string, int x, int y, SDL_Renderer* renderer) {
	Font_surface = TTF_RenderText_Solid_Wrapped(Font, string, 0, Font_color, 0);

	SDL_Texture* Font_texture = SDL_CreateTextureFromSurface(renderer, Font_surface);

	Font_rect.x = x;
	Font_rect.y = y;
	Font_rect.h = Font_surface->h;
	Font_rect.w = Font_surface->w;

	SDL_RenderTexture(renderer, Font_texture, NULL, &Font_rect);
	SDL_DestroySurface(Font_surface);
	SDL_DestroyTexture(Font_texture);
}

void kill_font() {
	TTF_CloseFont(Font);
}
