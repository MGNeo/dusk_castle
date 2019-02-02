/*
 * Модуль сцен.
 * Предоставляет ресурсы-сцены игры.
 *
 */

#ifndef SCENES_H
#define SCENES_H

#include <stdlib.h>

#define SCENE_MENU  0
#define SCENE_LEVEL 1

typedef struct s_scene_return_value
{
    size_t scene,
           param;
} scene_return_value;

extern scene_return_value scene_menu(const size_t _param);
extern scene_return_value scene_game(const size_t _param);

#endif
