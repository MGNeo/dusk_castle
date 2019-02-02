/*
 * Модуль анимации.
 * Предоставляет ресурс - состояние анимации и функцию обработки анимации.
 *
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdlib.h>

#define ANIMATIONS_FPS 3

typedef struct s_animation
{
    float t;

    size_t first_frame,
           last_frame,
           current_frame;
} animation;

extern void animation_processing(animation *const _animation,
                                 const float _dt);

#endif
