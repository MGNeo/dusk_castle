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

// Вычисляет dt итерации главного цикла.
static float dt_calculate(void);

// Реализация функциональности редактора.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void editor(void)
{
    srand(time(NULL));

    size_t scene = 0;
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

            switch (scene)
            {
                // Первое меню Создать / Открыть.
                case (0):
                {
                    // Если пункт меню был активирован, и активация
                    // прошла успешно, переходим к редактированию уровня.
                    if (menu_1_processing(&event) == 1)
                    {
                        scene = 1;
                    }
                    break;
                }
                // Редактирование уровня.
                case (1):
                {
                    // Обработка курсора.
                    cursor_processing(&event);
                    // Обработка меню №2.
                    menu_2_processing(&event);
                    // Обработка нажатия F8 (сохранения).
                    f8_processing(&event);
                    // Обработка Escape (возврат в первое меню с возможным сохранением).
                    // Переход к первому меню происходит только в том случае,
                    // если пользователь решил не сохранять результат работы,
                    // либо если решил сохранить результат, и это сохранение
                    // было выполнено удачно.
                    if (escape_processing(&event) == 1)
                    {
                        scene = 0;
                    }
                    break;
                }
            }
        }

        // Задаем цвет очистки всех сцен.
        if (SDL_SetRenderDrawColor(render, 0, 0, 0, 255) != 0)
        {
            crash("editor(), SDL_SetRenderDrawColor() != 0\nSDL_GetError() : %s",
                  SDL_GetError());
        }

        // Очищаем рендер.
        if (SDL_RenderClear(render) != 0)
        {
            crash("editor(), SDL_RenderClear() != 0\nSDL_GetError() : %s",
                  SDL_GetError());
        }

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
                // Обработка автоматического сохранения.
                auto_save_processing(dt);

                // Обработка точки рендеринга.
                render_point_processing();

                // Рисуем карту.
                map_draw();

                // Рисуем сетку.
                grid_draw();

                // Рисуем курсор.
                cursor_draw();

                // Рисуем меню №2.
                menu_2_draw();

                // Рисуем информацию.
                info_draw();

                break;
            }
        }

        // Представляем рендер после обработки всех сцен.
        SDL_RenderPresent(render);
    }
}

// Вычисляет dt игрового цикла.
// За одну итерацию главного цикла функция должна вызываться один раз.
float dt_calculate(void)
{
    static Uint64 t1, t2;
    t2 = t1;
    t1 = SDL_GetPerformanceCounter();
    // dt первого кадра определить невозможно.
    static size_t first;
    if (first == 0)
    {
        ++first;
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
