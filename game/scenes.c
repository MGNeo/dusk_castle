#include "scenes.h"
#include "crash.h"
#include "render.h"
#include "windows.h"
#include "text.h"
#include "textures.h"

// Сцена-меню.
// Игнорирует значение _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
scene_return_value scene_menu(const size_t _param)
{
    // ...
    scene_return_value srv = {0, 0};
    return srv;
}

// Сцена-игра.
// Обрабатывает значение _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
scene_return_value scene_game(const size_t _param)
{
    // ...
    scene_return_value srv = {0, 0};
    return srv;
}
