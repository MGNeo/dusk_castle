#include "game.h"

#include "scene_menu.h"

// Реализация функциональности игры.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void game(void)
{
    scene_return_value srv = { scene_menu, SCENE_MENU_PARAM };
    while (1)
    {
        // Советский вертухан.
        srv = srv.scene(srv.param);
    }
}

