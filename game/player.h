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
    animation_state anim;
} player_unit;

extern player_unit player;

#endif
