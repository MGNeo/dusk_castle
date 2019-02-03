#include "scene_menu.h"

#include "SDL.h"
#include "window.h"
#include "text.h"
#include "render.h"
#include "crash.h"
#include "dt.h"
#include <stdio.h>

// Управление менюшкой.
static void menu_control(const SDL_Event *const _event,
                         size_t *const _selected_item,
                         const size_t _items_count);

// Рисование меню.
static void menu_draw(const size_t _selected_item,
                      const size_t _items_count,
                      const char *const _items[]);

// Сцена-меню.
// Игнорирует значение _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
scene_return_value scene_menu(const size_t _param)
{
    (void)_param;
    size_t selected_item = 0;
    const char *const items[] = {"Играть",
                                 "Помощь",
                                 "Авторы",
                                 "Выход"};
    const size_t items_count = sizeof(items) / sizeof(char*);

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
            menu_control(&event, &selected_item, items_count);
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
        menu_draw(selected_item, items_count, items);

        // Представляем рендер.
        SDL_RenderPresent(render);
    }

    scene_return_value srv = {0, 0};
    return srv;
}

// Управление меню.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void menu_control(const SDL_Event *const _event,
                  size_t *const _selected_item,
                  const size_t _items_count)
{
    if (_event == NULL)
    {
        crash("menu_control(), _event == NULL");
    }

    if (_selected_item == NULL)
    {
        crash("menu_control(), _selected_item == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        // Листаем вверх.
        if ( (_event->key.keysym.sym == SDLK_w) ||
             (_event->key.keysym.sym == SDLK_UP) )
        {
            _selected_item[0] -= 1;
        }
        // Листаем вниз.
        if ( (_event->key.keysym.sym == SDLK_s) ||
             (_event->key.keysym.sym == SDLK_DOWN) )
        {
            _selected_item[0] += 1;
        }
        _selected_item[0] %= _items_count;
    }
}

// Рисование меню.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void menu_draw(const size_t _selected_item,
                      const size_t _items_count,
                      const char *const _items[])
{
    if (_items == NULL)
    {
        crash("menu_draw(), _items == NULL");
    }

    for (size_t i = 0; i < _items_count; ++i)
    {
        if (_items[i] == NULL)
        {
            crash("menu_draw(), _items[%Iu] == NULL", i);
        }
    }

    // Получаем размеры окна клиентской области.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Размер шрифта.
    const size_t size = 20;
    // Положение текста по x.
    const int x = w / 2;
    // Положение текста по y.
    const int y = h / 2 - (_items_count / 2) * size;
    for (size_t i = 0; i < _items_count; ++i)
    {
        char text[100];
        int s_bias = 0;
        // Добавляем выделенному пункту меню скобочки и увеличение.
        if (_selected_item == i)
        {
            sprintf(text, "[%s]", _items[i]);
            s_bias = 2;
        } else {
            sprintf(text, "%s", _items[i]);
        }
        text_draw(text, size + s_bias, x, y + i * size, TEXT_ALIGN_CENTER);
    }
}
