/*
 * Модуль возвращаемого значения сцены.
 * Предоставляет описание ресурса - возвращаемого значения сцены.
 *
 */
#ifndef SCENE_RETURN_VALUE_H
#define SCENE_RETURN_VALUE_H

#include <stdlib.h>

typedef struct s_scene_return_value
{
    size_t scene;
    size_t param;
} scene_return_value;

#endif
