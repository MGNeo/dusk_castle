/*
 * Модуль сцен.
 * Предоставляет ресурсы-сцены игры.
 *
 */

#ifndef SCENES_H
#define SCENES_H

#include "scene_return_value.h"

// Массив с указателями на функции сцен.
extern scene_return_value (*const scenes[])(const size_t _param);
// Количество сцен.
extern const size_t scenes_count;

#endif
