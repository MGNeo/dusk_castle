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
    float fps,
          t;

    size_t first_frame,
           last_frame,
           current_frame;
} animation_state;

#endif
