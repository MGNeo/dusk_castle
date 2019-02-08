/*
 * Модуль анимации.
 * Предоставляет ресурс - анимацию и функции работы с анимацией.
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
extern void animation_player_walk_init(animation *const _animation);
extern void animation_player_climb_init(animation *const _animation);
extern void animation_player_fall_init(animation *const _animation);
extern void animation_player_die_init(animation *const _animation);

extern void animation_ghost_init(animation *const _animation);
extern void animation_bat_init(animation *const _animation);
extern void animation_toxic_init(animation *const _animation);

extern void animation_silver_coin_init(animation *const _animation);
extern void animation_gold_coin_init(animation *const _animation);
extern void animation_toxic_init(animation *const _animation);

#endif
