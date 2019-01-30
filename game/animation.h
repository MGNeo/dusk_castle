/*
 * Модуль анимации.
 * Предоставляет ресурс - состояние анимации.
 *
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdlib.h>

typedef struct s_animation_state
{
    float fps;

    size_t first_frame,
           last_frame;

    float current_frame;
} animation_state;

#endif
