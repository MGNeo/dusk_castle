#include "scene_menu.h"

#include "SDL.h"
#include "window.h"
#include "text.h"
#include "render.h"
#include "crash.h"
#include "dt.h"
#include "scene_game.h"
#include "scene_help.h"
#include "scene_authors.h"

#include <stdio.h>
#include <string.h>

// Элементы меню.
typedef enum e_elements
{
    GAME,
    HELP,
    AUTHORS,
    EXIT
} elements;

// Количество элементов меню.
#define COUNT (EXIT + 1)
// Выделенный элемент меню.
static elements selected = 0;

// Управление менюшкой.
static size_t menu_control(const SDL_Event *const _event);
// Формирование команды перехода к следующей сцене.
static scene_return_value menu_activate(void);
// Рисование меню.
static void menu_draw(void);

// Сцена-меню.
// Игнорирует значение _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern scene_return_value scene_menu(const size_t _param)
{
    (void)_param;

    // Выделяем первый элемент меню.
    selected = GAME;

    while (1)
    {
        // Если объявления функции не было, то стиралась первая буква первого пункта...
        // Проявление UB.
         dt_calculate();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }
            // Обрабатываем управление менюшкой.
            const size_t a = menu_control(&event);
            // Если пункт меню был активирован, переходим к следующей сцене.
            if (a != 0)
            {
                return menu_activate();
            }
        }

        // Задаем рендеру цвет рисования.
        if (SDL_SetRenderDrawColor(render, 0, 0, 0, 255) != 0)
        {
            crash("scene_menu(), не удалось задать рендеру цвет очистки.\nSDL_GetError() : %s",
                  SDL_GetError());
        }

        // Очищаем рендер.
        if (SDL_RenderClear(render) != 0)
        {
            crash("scene_menu(), не удалось очистить рендер.\nSDL_GetError() : %s",
                  SDL_GetError());
        }

        // Рисуем меню.
        menu_draw();

        // Представляем рендер.
        SDL_RenderPresent(render);
    }
}

// Управление меню.
// При активации элемента меню возвращает 1, иначе 0.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
size_t menu_control(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("menu_control(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        // Листаем вверх.
        if ( (_event->key.keysym.sym == SDLK_w) ||
             (_event->key.keysym.sym == SDLK_UP) )
        {
            --selected;
            selected %= COUNT;
            return 0;
        }
        // Листаем вниз.
        if ( (_event->key.keysym.sym == SDLK_s) ||
             (_event->key.keysym.sym == SDLK_DOWN) )
        {
            ++selected;
            selected %= COUNT;
            return 0;
        }
        // Активировали пункт меню.
        if ( (_event->key.keysym.sym == SDLK_SPACE) ||
             (_event->key.keysym.sym == SDLK_RETURN) )
        {
            return 1;
        }
    }
    return 0;
}

// Формирование команды перехода к следующей сцене.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static scene_return_value menu_activate(void)
{
    scene_return_value srv = {NULL, 0};
    switch (selected)
    {
        case (GAME):
        {
            srv.scene = scene_game;
            srv.param = 0;
            break;
        }
        case (HELP):
        {
            srv.scene = scene_help;
            srv.param = 0;
        }
        case (AUTHORS):
        {
            srv.scene = scene_authors;
            srv.param = 0;
        }
        case (EXIT):
        {
            exit(0);
        }
        default:
        {
            crash("menu_activate(), активирован неизвестный пункт меню.\nselected: %i",
                  selected);
        }
    }

    return srv;
}

// Рисование меню.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void menu_draw(void)
{
    // Получаем размеры окна клиентской области.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Размер шрифта.
    const size_t size = 20;
    // Положение текста по x.
    const int x = w / 2;
    // Положение текста по y.
    const int y = h / 2 - (COUNT / 2) * size;
    for (elements i = 0; i < COUNT; ++i)
    {
        // Формируем текстовое представление элемента меню.
        char text_a[100] = {0};
        switch (i)
        {
            case (GAME):
            {
                sprintf(text_a, "Играть");
                break;
            }
            case (HELP):
            {
                sprintf(text_a, "Помощь");
                break;
            }
            case (AUTHORS):
            {
                sprintf(text_a, "Авторы");
                break;
            }
            case (EXIT):
            {
                sprintf(text_a, "Выход");
                break;
            }
            default:
            {
                crash("menu_draw(), попытка отрисовать неизвестный элемент меню.\ni: %i", i);
            }
        }

        // Если отрисовываемый элемент - это выделенный элемент меню,
        // добавляем ему скобочки и придаем ему увеличение.
        char text_b[100];
        int s_bias = 0;

        if (selected == i)
        {
            sprintf(text_b, "[%s]", text_a);
            s_bias = 2;
        } else {
            sprintf(text_b, "%s", text_a);
        }

        // Рисуем текст.
        text_draw(text_b,
                  size + s_bias,
                  x,
                  y + i * size,
                  TEXT_ALIGN_CENTER);
    }
}
