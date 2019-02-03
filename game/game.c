#include "game.h"

#include "scenes.h"

// Реализация функциональности игры.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void game(void)
{
    scene_return_value srv = { 0, 0 };
    while (1)
    {
        srv = scenes[srv.scene](srv.param);
    }
}

