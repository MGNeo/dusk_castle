/*
 * Модуль текстур.
 * Предоставляет ресурсы-текстуры.
 *
 */

#ifndef TEXTURES_H
#define TEXTURES_H

#include "SDL.h"
#include "units.h"

#define TEXTURES_COUNT UNITS_COUNT

extern SDL_Texture *textures[TEXTURES_COUNT];

#endif
