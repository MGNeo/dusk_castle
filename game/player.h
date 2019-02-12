/*
 * Модуль игрока.
 * Предоставляет ресурс-игрока и его описание.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "animation.h"

#define PLAYER_GRAVITY         5
#define PLAYER_SPEED           4
#define PLAYER_ACCELERATION   10
#define PLAYER_BORDER       0.1f

typedef struct s_player_unit
{

    float x, y,
          vx, vy;

    animation a;
} player_unit;

#endif
