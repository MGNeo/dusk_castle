/*
 * Модуль игрока.
 * Предоставляет ресурс-игрока и его описание.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "animation.h"

typedef struct s_player_unit
{
<<<<<<< HEAD
    animation anim;
=======
    float x, y, vx, vy;

    animation_state anim;
>>>>>>> 9af60c69fe74b8746afa6cb674ef69b85eae1608
} player_unit;

#endif
