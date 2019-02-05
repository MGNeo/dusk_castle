#include "editor.h"
#include "crash.h"
#include "render.h"
#include "windows.h"
#include "draw.h"
#include "processing.h"
#include "dt.h"

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Реализация функциональности редактора.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void editor(void)
{
    srand(time(NULL));

    const float fps = 60.f;
    const float spf = 1 / fps;

    size_t scene = 0;

    // Сбрасываем точку отсчета времени.
    dt_reset();

    while (1)
    {
        // Если имеется необработанное событие, то обрабатываем его.
        SDL_Event event;
        if (SDL_PollEvent(&event) != 0)
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

        // Если пришло время, то просчитываем и отрисовываем.
        const float dt = dt_get();
        if (dt > spf)
        {
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

            // Сброс.
            dt_reset();
        }
    }
}
