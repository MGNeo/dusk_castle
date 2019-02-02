#include "game.h"
#include "crash.h"
#include "render.h"
#include "scenes.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Вычисляет dt итерации игрового цикла.
static float dt_calculate(void);

// Реализация функциональности игры.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void game(void)
{
    srand(time(NULL));

    scene_return_value (*scenes[2])(const size_t) = {scene_menu,
                                                     scene_game};

    scene_return_value srv = { SCENE_MENU, 0 };
    while (1)
    {
        srv = scenes[srv.scene](srv.param);
    }

    size_t scene = SCENE_MENU;
    while (1)
    {
        // Определяем время кадра.
        const float dt = dt_calculate();

        // Обработка всех накопившихся событий.
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }
        }

        // Обрабатываем сцены.

        // Задаем цвет очистки всех сцен.
        if (SDL_SetRenderDrawColor(render, 0, 0, 0, 255) != 0)
        {
            crash("game(), SDL_SetRenderDrawColor() != 0\nSDL_GetError() : %s",
                  SDL_GetError());
        }

        // Очищаем рендер.
        if (SDL_RenderClear(render) != 0)
        {
            crash("game(), SDL_RenderClear() != 0\nSDL_GetError() : %s",
                  SDL_GetError());
        }

        // Представляем рендер после обработки всех сцен.
        SDL_RenderPresent(render);
    }
}

// Вычисляет dt итерации игрового цикла.
// За одну итерацию игрового цикла функция должна вызываться один раз.
static float dt_calculate(void)
{
    static Uint64 t1, t2;
    t2 = t1;
    t1 = SDL_GetPerformanceCounter();
    // dt первого кадра определить невозможно.
    static size_t first;
    if (first == 0)
    {
        ++first;
        return 0.f;
    }

    // Контроль теоретически возможного переполнения Uint64.
    if (t2 > t1)
    {
        return (t1 + (SDL_MAX_UINT64 - t2)) / (float) SDL_GetPerformanceFrequency();
    } else {
        return (t1 - t2) / (float) SDL_GetPerformanceFrequency();
    }
}
