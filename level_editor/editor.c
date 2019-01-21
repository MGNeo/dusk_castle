#include "editor.h"
#include "crash.h"
#include "render.h"
#include "windows.h"
#include "draw.h"
#include "processing.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "window.h"// ТЕСТ

// Вычисляет dt итерации игрового цикла.
static float dt_calculate(void);

void editor(void)
{
    srand(time(NULL));

    size_t scene = 0;
    while (1)
    {
        const float dt = dt_calculate();

        // Обработка всех накопившихся событий.
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

            switch (scene)
            {
                // Первое меню Открыть/Создать.
                case (0):
                {
                    // Если какой-либо пункт меню был активирован, переходим редактированию.
                    if (menu_1_processing(&event) != 0)
                    {
                        scene = 1;
                    }
                    break;
                }

                case (1):
                {

                    break;
                }
            }
        }

        // Цвет очистки всех сцен - черный.
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);// Тест.

        SDL_RenderClear(render);// ТЕСТ.

        // Обрабатываем сцены.
        switch (scene)
        {
            // Первое меню Открыть/Создать.
            case(0):
            {
                // Рисуем первое меню.
                menu_1_draw();
                break;
            }
            // Редактирование уровня.
            case (1):
            {
                draw_grid();
                break;
            }
        }

        // Представляем рендер после обработки всех сцен.
        SDL_RenderPresent(render);
    }
}

// Вычисляет dt игрового цикла.
// За одну итерацию игрового цикла функция должна вызываться один раз.
float dt_calculate(void)
{
    static Uint64 t1, t2;
    t2 = t1;
    t1 = SDL_GetPerformanceCounter();
    // dt первого кадра определить невозможно.
    static size_t first = 1;
    if (first == 1)
    {
        first = 0;
        return 0;
    }

    // Контроль теоретически возможного переполнения Uint64
    if (t2 > t1)
    {
        return (t1 + (SDL_MAX_UINT64 - t2)) / (float) SDL_GetPerformanceFrequency();
    } else {
        return (t1 - t2) / (float) SDL_GetPerformanceFrequency();
    }
}
