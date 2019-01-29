#include "draw.h"
#include "glyphs.h"
#include "render.h"
#include "crash.h"
#include "text.h"
#include "menu_1.h"
#include "menu_2.h"
#include "window.h"
#include "sprite.h"
#include "cursor.h"
#include "render_point.h"
#include "map.h"
#include "textures.h"
#include "units.h"
#include "statistics.h"

#include <string.h>
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Отрисовка текста.
static void text_draw(const char *const _text,
                      const size_t _size,
                      const int _x,
                      const int _y,
                      const text_align _align);

// Отрисовка спрайта.
static void sprite_draw(SDL_Texture *const _texture,
                        const int _x,
                        const int _y);

// Рисует нуль-терминированную строку в виде однострочного текста.
// Нультерминатор ресуется, если вдруг для него существует текстура.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void text_draw(const char *const _text,
               const size_t _size,
               const int _x,
               const int _y,
               const text_align _align)
{
    // Текст не задан.
    if (_text == NULL)
    {
        crash("text_draw(), _text == NULL");
    }

    // Задан слишком маленький размер текста.
    if (_size < MIN_FONT_SIZE)
    {
        crash("text_draw(), попытка отрисовать слишком мелкий текст.\n_size: %Iu",
              _size);
    }

    // Задан слишком большой размер текста.
    if (_size > MAX_FONT_SIZE)
    {
        crash("text_draw(), попытка отрисовать слишком крупный текст.\n_size: %Iu",
              _size);
    }

    const size_t text_length = strlen(_text);

    if (text_length > MAX_TEXT_LENGTH)
    {
        crash("text_draw(), длина текста превышает максимальную.\ntext_length: %Iu",
              text_length);
    }

    const size_t s = _size - MIN_FONT_SIZE;

    switch (_align)
    {
        // Вывод текста с левым выравниванием.
        case (TEXT_ALIGN_LEFT):
        {
            SDL_Rect rect;

            rect.x = _x;
            rect.y = _y;

            // Обходим символы строки слева направо (включая нультерминатор).
            for (size_t i = 0; i <= text_length; ++i)
            {
                const unsigned char g = _text[i];

                if (glyphs[s][g].texture != NULL)
                {
                    rect.h = glyphs[s][g].h;
                    rect.w = glyphs[s][g].w;

                    if (SDL_RenderCopy(render, glyphs[s][g].texture, NULL, &rect) != 0)
                    {
                        crash("text_draw(), не удалось отрисовать глиф при выводе текста с левым выравниванием\nSDL_GetError() : %s",
                              SDL_GetError());
                    }

                    rect.x += glyphs[s][g].w;
                }
            }
            break;
        }
        // Вывод текста с правым выравниванием.
        case (TEXT_ALIGN_RIGHT):
        {
            SDL_Rect rect;

            rect.x = _x;
            rect.y = _y;

            // Обходим символы строки справа налево (включая нультерминатор).
            for (size_t i = text_length; i <= text_length; --i)
            {
                const unsigned char g = _text[i];

                if (glyphs[s][g].texture != NULL)
                {
                    rect.h = glyphs[s][g].h;
                    rect.w = glyphs[s][g].w;

                    rect.x -= glyphs[s][g].w;

                    if (SDL_RenderCopy(render, glyphs[s][g].texture, NULL, &rect) != 0)
                    {
                        crash("text_draw(), не удалось отрисовать глиф при выводе текста с правым выравниванием\nSDL_GetError() : %s",
                              SDL_GetError());
                    }
                }
            }

            break;
        }

        case (TEXT_ALIGN_CENTER):
        {
            // Вычислим, сколько пикселей на экране займет строка (включая нультерминатор).
            int total_text_width = 0;
            for (size_t i = 0; i <= text_length; ++i)
            {
                const unsigned char g = _text[i];
                if (glyphs[s][g].texture != NULL)
                {
                    total_text_width += glyphs[s][g].w;
                }
            }

            const int offset = total_text_width / 2;

            SDL_Rect rect;

            rect.x = _x - offset;
            rect.y = _y;

            // Обходим символы строки слева направо (включая нультерминатор).
            for (size_t i = 0; i <= text_length; ++i)
            {
                const unsigned char g = _text[i];

                if (glyphs[s][g].texture != NULL)
                {
                    rect.h = glyphs[s][g].h;
                    rect.w = glyphs[s][g].w;
                    if (SDL_RenderCopy(render, glyphs[s][g].texture, NULL, &rect) != 0)
                    {
                        crash("text_draw(), не удалось отрисовать глиф при выводе текста с центральным выравниванием\nSDL_GetError() : %s",
                              SDL_GetError());
                    }

                    rect.x += glyphs[s][g].w;
                }
            }

            break;
        }
        default:
        {
            crash("text_draw(), для отрисовки текста задано неизвестное выравнивание");
            break;
        }
    }
}

// Отрисовывает первое меню.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void menu_1_draw(void)
{
    int w, h;
    // Пытаемся получить текущие размеры окна (клиентской области).
    SDL_GetWindowSize(window, &w, &h);

    const size_t size = 20;

    w = w / 2;
    h = h / 2;

    // Отрисовываем меню.
    if (menu_1_selected_item == MENU_1_NEW)
    {
        text_draw("[ Создать новый уровень ]", size, w, h - size, TEXT_ALIGN_CENTER);
        text_draw("Открыть существующий уровень", size, w, h + size, TEXT_ALIGN_CENTER);

        return;
    }
    if (menu_1_selected_item == MENU_1_OPEN)
    {
        text_draw("Создать новый уровень", size, w, h - size, TEXT_ALIGN_CENTER);
        text_draw("[ Открыть существующий уровень ]", size, w, h + size, TEXT_ALIGN_CENTER);

        return;
    }
}

// Рисует сетку.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void grid_draw(void)
{
    // Задаем цвет сетки.
    if (SDL_SetRenderDrawColor(render, 100, 100, 100, 200) != 0)
    {
        crash("grid_draw(), не удалось задать цвет линий.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    for (int x = 0; x < w; x += SPRITE_SIZE)
    {
        if (SDL_RenderDrawLine(render, x, 0, x, h) != 0)
        {
            crash("grid_draw(), не удалось отрисовать вертикальную линию сетки\nSDL_GetError() : %s",
                  SDL_GetError());
        }
    }
    for (int y = 0; y < h; y += SPRITE_SIZE)
    {
        if (SDL_RenderDrawLine(render, 0, y, w, y) != 0)
        {
            crash("grid_draw(), не удалось отрисовать горизонтальную линию сетки\nSDL_GetError() : %s",
                  SDL_GetError());
        }
    }

}

// Отрисовка курсора.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void cursor_draw(void)
{
    // Задаем цвет курсора.
    if (SDL_SetRenderDrawColor(render, 0, 255, 0, 255) != 0)
    {
        crash("cursor_draw(), не удалось задать цвет линий курсора навигации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    const int x = (cursor_x - render_point_x) * SPRITE_SIZE;
    const int y = (cursor_y - render_point_y) * SPRITE_SIZE;

    const int x_1 = x + 2;
    const int y_1 = y + 2;

    const int x_2 = x + SPRITE_SIZE - 2;
    const int y_2 = y + SPRITE_SIZE - 2;

    // Верх.
    if (SDL_RenderDrawLine(render, x_1, y_1, x_2, y_1) != 0)
    {
        crash("cursor_draw(), не удалось отрисовать верхнюю часть курсора навигации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Лево.
    if (SDL_RenderDrawLine(render, x_1, y_1, x_1, y_2) != 0)
    {
        crash("cursor_draw(), не удалось отрисовать левую часть курсора навигации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Низ.
    if (SDL_RenderDrawLine(render, x_1, y_2, x_2, y_2) != 0)
    {
        crash("cursor_draw(), не удалось отрисовать нижнюю часть курсора навигации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Право.
    if (SDL_RenderDrawLine(render, x_2, y_2, x_2, y_1) != 0)
    {
        crash("cursor_draw(), не удалось отрисовать правую часть курсора навигации.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}

// Отрисовка информации.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void info_draw(void)
{
    // Задаем цвет панели информации.
    if (SDL_SetRenderDrawColor(render, 100, 100, 100, 255) != 0)
    {
        crash("info_draw(), не удалось задать цвет панели информации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Получаем размеры окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Определяем размер и расположение панели информации.
    SDL_Rect rect;
    rect.w = w;
    rect.h = 40;
    rect.x = 0;
    rect.y = 0;

    // Рисуем панель информации.
    if (SDL_RenderFillRect(render, &rect) != 0)
    {
        crash("info_draw(), не удалось нарисовать панель информации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Буфер для формирования строк.
    char info[100];

    // Информация о курсоре.
    sprintf(info, "Курсор X: %i",
            cursor_x);
    text_draw(info, 12, 10, 4, TEXT_ALIGN_LEFT);
    sprintf(info, "Курсор Y: %i",
            cursor_y);
    text_draw(info, 12, 10, 20, TEXT_ALIGN_LEFT);


    // Информация.
    sprintf(info, "Навигация: W, S, A, D, SPACE");
    text_draw(info, 12, 176, 4, TEXT_ALIGN_LEFT);

    sprintf(info, "Выбор блока: Q, E");
    text_draw(info, 12, 176, 20, TEXT_ALIGN_LEFT);

    sprintf(info, "Сохранение результата: F8");
    text_draw(info, 12, 432, 4, TEXT_ALIGN_LEFT);

    sprintf(info, "Выход в первое меню: Escape");
    text_draw(info, 12, 432, 20, TEXT_ALIGN_LEFT);
}

// Отрисовка карты.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void map_draw(void)
{
    // Получаем размеры окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    // Определяем, сколько клеток видно на экране.
    const int count_x = w / SPRITE_SIZE + 1;
    const int count_y = h / SPRITE_SIZE + 1;

    // Отрисовываем только те клетки, которые находятся в пределах видимости.
    for (int x = render_point_x; x < render_point_x + count_x; ++x)
    {
        for (int y = render_point_y; y < render_point_y + count_y; ++y)
        {
            if ( (x < MAP_WIDTH) && (y < MAP_HEIGHT) )
            {
                sprite_draw(textures[map[x][y]],
                           (x - render_point_x) * SPRITE_SIZE,
                           (y - render_point_y) * SPRITE_SIZE);
            }
        }
    }
}

// Рисует спрайт.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void sprite_draw(SDL_Texture *const _texture,
                        const int _x,
                        const int _y)
{
    if (_texture == NULL)
    {
        crash("sprite_draw(), _texture == NULL");
    }

    SDL_Rect rect;
    rect.w = SPRITE_SIZE;
    rect.h = SPRITE_SIZE;
    rect.x = _x;
    rect.y = _y;

    // Рисуем спрайт.
    if (SDL_RenderCopy(render, _texture, NULL, &rect) != 0)
    {
        crash("sprite_draw(), не удалось отрисовать спрайт.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}

// Рисует меню №2.
// В случае критической ошибки показывает информацию о причине сбоя и кращит программу.
void menu_2_draw(void)
{
    // Задаем цвет панели выбора типа блока.
    if (SDL_SetRenderDrawColor(render, 100, 100, 100, 255) != 0)
    {
        crash("menu_2_draw(), не удалось задать цвет панели выбора типа блока.\nSDL_GetError() : %s",
              SDL_GetError());
    }
    // Получаем размеры окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Определяем размер и расположение панели выбора типа блока.
    SDL_Rect rect;
    rect.w = w;
    rect.h = SPRITE_SIZE + 16;
    rect.x = 0;
    rect.y = h - SPRITE_SIZE - 16;

    // Рисуем панель выбора типа блока.
    if (SDL_RenderFillRect(render, &rect) != 0)
    {
        crash("menu_2_draw(), не удалось нарисовать панель информации.\nSDL_GetError() : %s",
              SDL_GetError());
    }
    // Позиция элемента по y.
    const int y = h - SPRITE_SIZE;

    // ID центрального юнита.
    // Необходим для того, чтобы выделенный элемент располагался в центре списка.
    const uint8_t central_unit_id = (UNITS_COUNT - 1) / 2;

    // Сдвиг списка.
    const int list_shift = (w - UNITS_COUNT * SPRITE_SIZE) / 2 + SPRITE_SIZE / 2;
    // Рисуем элементы списка.
    for (uint8_t e = 0; e < UNITS_COUNT; ++e)
    {
        // Определяем положение элемента списка по x.
        const int x = e * SPRITE_SIZE + list_shift;
        // Если элемент списка находится за пределами экрана, то мы его не рисуем.
        if ( (x < -SPRITE_SIZE) || (x > w) )
        {
            continue;
        }

        // Определяем, какой тип имеет отображаемый элемент списка.
        // Выбранный элемент списка отображается в центре.
        // Зацикливание отображения осуществляется при помощи переполнения через верхнюю границу
        // диапазона юнитов и получения остатка от деления.
        // Переполнение через нижнюю границу диапазона (0) использовать нельзя,
        // т.к. UINT8_MAX не кратен UNITS_COUNT.

        // Тип элемента списка.
        const uint8_t type = (UNITS_COUNT - central_unit_id + menu_2_selected_item + e) % UNITS_COUNT;
        // Если тип элемента списка оказался идентичен выбранному типу,
        // рисуем под элементом выделение.
        if (type == menu_2_selected_item)
        {
            // Задаем цвет заливки.
            if (SDL_SetRenderDrawColor(render, 0, 200, 0, 255) != 0)
            {
                crash("menu_2_draw(), не удалось задать цвет заливки выделения выбранного типа блока.\nSDL_GetError() : %s",
                      SDL_GetError());
            }

            // Определяем позицию выделения.
            rect.h = SPRITE_SIZE + 16;
            rect.w = SPRITE_SIZE;
            rect.x = x;
            rect.y = y - 16;

            // Рисуем выделение.
            if (SDL_RenderFillRect(render, &rect) != 0)
            {
                crash("menu_2_draw(), не удалось нарисовать верхнюю линюю подсветки выделенного типа блока.\nSDL_GetError() : %s",
                      SDL_GetError());
            }

        }

        // Отрисуем спрайт.
        sprite_draw(textures[type], x, y);

        // Отрисуем счетчик количества блоков данного типа.
        char number[6];
        sprintf(number, "%Iu", (size_t)statistics[type]);
        text_draw(number, 10, x + SPRITE_SIZE / 2, y - 14, TEXT_ALIGN_CENTER);
    }
}
