#include "editor.h"
#include "crash.h"
#include "render.h"
#include "windows.h"
#include "draw.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Вычисляет dt итерации игрового цикла.
static float dt_calculate(void);

void editor(void)
{
    srand(time(NULL));

    size_t scene = 0;
    while (1)
    {
        const float dt = dt_calculate();

        // Обрабатываем все события.
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

        }

        SDL_RenderClear(render);// ТЕСТ.

        // Обрабатываем сцены.
        switch (scene)
        {
            // Выбор Открыть/Создать.
            case(0):
            {
                break;
            }
            // Редактирование уровня.
            case (1):
            {
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
