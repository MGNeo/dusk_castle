#include "scene_menu.h"
#include "units.h"
#include "scene_menu.h"
#include "dt.h"
#include "window.h"
#include "text.h"
#include "render.h"
#include "crash.h"
#include "fps.h"

#include <SDL.h>

// Выделенный юнит.
static size_t selected = 0;
// Количество объектов, по которым возможно получить помощь.
#define COUNT UNITS_COUNT

// Контроль.
static size_t control(void);
// Формирование команды перехода к следующей сцене.
static scene_return_value next_scene(void);
// Отрисовка.
static void draw(void);

// Сцена-помощь.
// Игнорирует значение _param.
extern scene_return_value scene_help(const size_t _param)
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

        // Рисуем.
        if (dt_get() > SPF)
        {
            draw();
            dt_reset();
        }
    }

    scene_return_value srv = {0, 0};
    return srv;
}

// Контроль.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
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
            // Возвращаемся в меню.
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return 1;
            }
            // Листаем влево.
            if ( (event.key.keysym.sym == SDLK_a) ||
                 (event.key.keysym.sym == SDLK_LEFT) )
            {
                --selected;
                selected %= COUNT;
                return 0;
            }
            // Листаем вправо.
            if ( (event.key.keysym.sym == SDLK_d) ||
                 (event.key.keysym.sym == SDLK_RIGHT) )
            {
                ++selected;
                selected %= COUNT;
                return 0;
            }
        }
    }
    return 0;
}
// Формирование команды перехода к следующей сцене.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
static scene_return_value next_scene(void)
{
    scene_return_value srv = {scene_menu, SCENE_MENU_PARAM};
    return srv;
}
// Отрисовка.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
static void draw(void)
{
    // Получаем размеры окна клиентской области.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Задаем рендеру цвет очистки.
    if (SDL_SetRenderDrawColor(render, 0, 0, 0, 255) != 0)
    {
        crash("scene_help, draw(), не удалось задать рендеру цвет рисования.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Очищаем рендер.
    if (SDL_RenderClear(render) != 0)
    {
        crash("scene_help, draw(), не удалось очистить рендер.\n");
    }

    // Рисуем подсказку.
    text_draw("[Escape] - возврат в меню",
              20,
              w / 2,
              h - 25,
              TEXT_ALIGN_CENTER);

    // Пока так.
    text_draw("Здесь будет помощь по игре...", 20, w / 2, h / 2, TEXT_ALIGN_CENTER);

    // Представляем рендер.
    SDL_RenderPresent(render);
}
