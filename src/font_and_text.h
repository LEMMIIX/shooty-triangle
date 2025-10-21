#ifndef FONT_AND_TEXT_H
#define FONT_AND_TEXT_H

#include <SDL3/SDL_render.h>

bool font_init();
void print_text_to_screen_with_color(const char* string, int x, int y, SDL_Renderer* renderer, SDL_Color color);
void print_text_to_screen(const char* str, int x, int y, SDL_Renderer* renderer);
void kill_font();

#endif
