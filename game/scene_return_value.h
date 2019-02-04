/*
 * Модуль возвращаемого значения сцены.
 * Предоставляет описание ресурса - возвращаемого значения сцены.
 *
 */
#ifndef SCENE_RETURN_VALUE_H
#define SCENE_RETURN_VALUE_H

#include <stdlib.h>

typedef struct s_scene_return_value scene_return_value;

struct s_scene_return_value
{
    scene_return_value (*scene)(const size_t _param);
    size_t param;
};

#endif
