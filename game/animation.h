/*
 * Модуль анимации.
 * Предоставляет ресурс - состояние анимации и функцию обработки анимации.
 *
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
           current_frame,
           last_frame;
} animation;

extern void animation_processing(animation *const _animation,
                                 const float _dt);

extern void animation_player_walk(animation *const _animation);
extern void animation_player_climb(animation *const _animation);
extern void animation_player_fall(animation *const _animation);
extern void animation_player_die(animation *const _animation);

extern void animation_ghost(animation *const _animation);
extern void animation_bat(animation *const _animation);
extern void animation_toxic(animation *const _animation);

#endif
