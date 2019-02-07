/*
 * Модуль сцены-игры.
 * Предоставляет ресурс-сцену игру.
 *
 */

#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "scene_return_value.h"

#define SCENE_GAME_PARAM_NEW_GAME      0
#define SCENE_GAME_PARAM_NEXT_LEVEL    1
#define SCENE_GAME_PARAM_CURRENT_LEVEL 2

extern scene_return_value scene_game(const size_t _param);

#endif
