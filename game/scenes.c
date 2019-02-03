#include "scenes.h"

#include "scene_menu.h"

// Массив с указателями на функции сцен.
scene_return_value (*const scenes[])(const size_t _param) = {scene_menu};
// Количество сцен.
const size_t scenes_count = sizeof(scenes) / sizeof(scene_return_value (*)(const size_t));
