#include "scene_authors.h"
#include "scene_menu.h"
#include "dt.h"
#include "text.h"
#include "window.h"
#include "render.h"
#include "crash.h"
#include "fps.h"

#include <SDL.h>
#include <math.h>
#include <time.h>

// Контроль.
static size_t control(void);
// Формирование команды перехода к следующей сцене.
static scene_return_value next_scene(void);
// Отрисовка.
static void draw(void);

// Сцена-авторы.
// Игнорирует значение _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern scene_return_value scene_authors(const size_t _param)
{
    (void)_param;

    dt_reset();

    while (1)
    {
        // Контроль.
        const size_t a = control();

        // Переход к другой сцене.
        if (a != 0)
        {
            return next_scene();
        }

        // Обработка отрисовки.
        if (dt_get() > SPF)
        {
            draw();
            dt_reset();
        }
    }
}

// Управление экраном авторов.
// Если пользователь хочет вернуться в меню, возвращает 1, иначе 0.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static size_t control(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return 1;
            }
        }
    }
    return 0;
}

// Формирование команды перехода к следующей сцене.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static scene_return_value next_scene(void)
{
    scene_return_value srv = {scene_menu, SCENE_MENU_PARAM};
    return srv;
}

// Отрисовка экрана авторов.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void draw(void)
{
    // Получаем размеры клиентской области окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Задаем рендеру цвет рисования.
    if (SDL_SetRenderDrawColor(render, 0, 0, 0, 255) != 0)
    {
        crash("scene_authors::draw(), не удалось задать рендеру цвет рисования.\nSDL_GetError() : %s",
                SDL_GetError());
    }

    // Очищаем рендер.
    if (SDL_RenderClear(render) != 0)
    {
        crash("scene_authors::draw(), не удалось очистить рендер.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Текст должен бежать вверх, как в звездных войнах.
    // ...

    // Рисуем подсказку.
    text_draw("[Escape] - возврат в меню",
              20,
              w / 2,
              h - 25,
              TEXT_ALIGN_CENTER);

    // Информация об игре.
    const char *const strings[] =
    {"Название игры: Сумеречный замок",
     " ",
     "- - -",
     " ",
     "Автор: Глухманюк Максим Александрович",
     "  Место и дата начала проекта: РФ, город Сызрань, 28.12.2018",
     " ",
     "- - -",
     " ",
     "Цели проекта:",
     "1. Демонстрация того факта, что даже при разработке игр вполне",
     "можно обойтись без наследования, классов, шаблонов, исключений,",
     "лямбд и всего остального, включая сам C++ =);",
     "2. Исследования в области организации модульности в языке C;",
     "3. Анализ возможностей и удобства использования библиотеки SDL и",
     "ее расширений.",
     " ",
     "- - -",
     " ",
     "PS. Я сделяль ^.^"};

     // Количество строк.
     const size_t strings_count = sizeof(strings) / sizeof(char*);
     // Размер текста строк.
     size_t size = 19;

     for (size_t s = 0; s < strings_count; ++s)
     {
         const float y = 50 + s * 20 + 3 * sin(clock() / 200.f + s);

         // Чтобы текст не налезал на подсказку.
         if ((y + 60) > h) break;

         // Отрисовываем строку.
         text_draw(strings[s],
                   size,
                   w / 2,
                   y,
                   TEXT_ALIGN_CENTER);
     }

     // Представляем рендер.
    SDL_RenderPresent(render);
}
