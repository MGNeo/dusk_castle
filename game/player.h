/*
 * Модуль игрока.
 * Предоставляет ресурс-игрока и его описание.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "animation.h"

// Единицы измерения - спрайты.

#define PLAYER_GRAVITY_ACCELERATION        10.f
#define PLAYER_Y_MAX_SPEED                 10.f

#define PLAYER_WALK_ACCELERATION           10.f
#define PLAYER_X_MAX_SPEED                 3.f

#define PLAYER_JUMP_SPEED                  5.f
#define PLAYER_CLIMB_SPEED                 2.f

#define PLAYER_BORDER                      0.05f

#define PLAYER_COLLISION_DETECT_MAX_STEP   0.01f

#define PLAYER_PASSIVE_BRAKING             0.25f

typedef struct s_player_unit
{

    float x, y,
          vx, vy;

    animation a;
} player_unit;

#endif
