/*
 * Модуль врага.
 * Предоставляет описание врага.
 *
 */
#ifndef ENEMY_H
#define ENEMY_H

#include "animation.h"
#include <SDL.h>

#define MAX_ENEMIES 1024

#define GHOST_SPEED 1
#define BAT_SPEED   1

typedef struct s_enemy_unit
{
    uint8_t active;

    float x, y,
          vx, vy;

    animation a;

    SDL_Texture *texture;
} enemy_unit;

#endif
