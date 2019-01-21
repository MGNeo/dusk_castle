/*
 * Модуль глифов.
 * Предоставляет ресурсы-глифы.
 *
 */

#ifndef GLYPHS_H
#define GLYPHS_H

#include "SDL.h"

#define MIN_FONT_SIZE 10
#define MAX_FONT_SIZE 30
#define FONTS_COUNT (MAX_FONT_SIZE - MIN_FONT_SIZE + 1)// ?

typedef struct s_glyph
{
    int w, h;
    SDL_Texture *texture;
} glyph;

extern glyph glyphs[FONTS_COUNT][256];

#endif
