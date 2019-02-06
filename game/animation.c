#include "animation.h"
#include "crash.h"
#include "frames.h"

// Обработка анимации.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_processing(animation *const _animation,
                          const float _dt)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_processing(), _animation == NULL");
    }
    _animation->t += _dt * ANIMATIONS_FPS;
    if (_animation->t > 1.f)
    {
        _animation->current_frame += _animation->t + 0.5f;
        _animation->t = 0.f;
        if (_animation->current_frame > _animation->last_frame)
        {
            _animation->current_frame -=
            (_animation->last_frame - _animation->first_frame) + 1;
        }
    }
}

// Инициализация анимации игрока "идти".
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_player_walk_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_player_walk_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = PLAYER_WALK_FIRST;
    _animation->current_frame = PLAYER_WALK_FIRST;
    _animation->last_frame = PLAYER_WALK_LAST;
}

// Инициализация анимации игрока "карабкаться".
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_player_climb_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_player_climb_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = PLAYER_CLIMB_FIRST;
    _animation->current_frame = PLAYER_CLIMB_FIRST;
    _animation->last_frame = PLAYER_CLIMB_LAST;
}

// Инициализация анимации игрока "падать".
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_player_fall_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_player_fall_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = PLAYER_FALL_FIRST;
    _animation->current_frame = PLAYER_FALL_FIRST;
    _animation->last_frame = PLAYER_FALL_LAST;
}

// Инициализация анимации игрока "умирать".
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_player_die_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_player_die_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = PLAYER_DIE_FIRST;
    _animation->current_frame = PLAYER_DIE_FIRST;
    _animation->last_frame = PLAYER_DIE_LAST;
}

// Инициализация анимации призрака.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_ghost_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_ghost_init(), _animation == NULL");
    }

    _animation->t = (rand() % 100) / 100.f;
    _animation->first_frame = 0;
    _animation->current_frame = 0;
    _animation->last_frame = GHOST_FRAMES_COUNT - 1;
}

// Инициализация анимации мыши.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_bat_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_but_init(), _animation == NULL");
    }

    _animation->t = (rand() % 100) / 100.f;
    _animation->first_frame = 0;
    _animation->current_frame = rand() % BAT_FRAMES_COUNT;
    _animation->last_frame = BAT_FRAMES_COUNT - 1;
}

// Инициализация анимации серебряной монетки.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void animation_silver_coin_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_silver_coin_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = 0;
    _animation->current_frame = 0;
    _animation->last_frame = SILVER_COIN_FRAMES_COUNT - 1;
}

// Инициализация анимации золотой монеты.
extern void animation_gold_coin_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_gold_coin_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = 0;
    _animation->current_frame = 0;
    _animation->last_frame = GOLD_COIN_FRAMES_COUNT - 1;
}

extern void animation_toxic_init(animation *const _animation)
{
    if (_animation == NULL)
    {
        crash("animation.c, animation_toxic_init(), _animation == NULL");
    }

    _animation->t = 0.f;
    _animation->first_frame = 0;
    _animation->current_frame = 0;
    _animation->last_frame = TOXIC_FRAMES_COUNT - 1;
}

// TODO +_init для имен функций инициализации анимации.
