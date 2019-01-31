#include "game.h"
#include "crash.h"
#include "render.h"
#include "scenes.h"

#include "processing.h"

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

            // Обрабатываем события текущей сцены.
            switch (scene)
            {
                case (SCENE_MENU):
                {
                    // Обрабатываем меню.
                    switch (menu_processing(&event))
                    {
                        case (-1):
                        {
                            exit(0);
                        }
                        case (1):
                        {
                            scene = SCENE_LEVEL;
                            break;
                        }
                    }
                    break;
                }
                case (SCENE_LEVEL):
                {

                    break;
                }
                case (SCENE_LOSE):
                {

                    break;
                }
                case (SCENE_WIN):
                {

                    break;
                }
                default:
                {
                    crash("game(), неизвестная сцена.");
                }
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

        // Просчитываем и рисуем текущую сцену.
        switch (scene)
        {
            case (SCENE_MENU):
            {
                menu_draw();
                break;
            }
            case (SCENE_LEVEL):
            {
                // Обрабатываем все анимации.
                animations_processing(dt);
                // Рисуем игрока.
                player_draw();
                break;
            }
            case (SCENE_LOSE):
            {

                break;
            }
            case (SCENE_WIN):
            {

                break;
            }
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
