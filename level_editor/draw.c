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

#include <string.h>
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

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
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
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
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void menu_1_draw(void)
{
    int w, h;
    // Пытаемся получить текущие размеры окна.
    SDL_GetWindowSize(window, &w, &h);// Нужен размер клиентской области...

    const size_t size = 20;

    w = w / 2;
    h = h / 2;

    // Возможно, стоит получать размеры окна один раз в начале цикла...

    // Отрисовываем меню.
    if (menu_1_selected_item == MENU_1_NEW)
    {
        text_draw("- Создать новый уровень -", size, w, h - size, TEXT_ALIGN_CENTER);
        text_draw("Открыть существующий уровень", size, w, h + size, TEXT_ALIGN_CENTER);

        return;
    }
    if (menu_1_selected_item == MENU_1_OPEN)
    {
        text_draw("Создать новый уровень", size, w, h - size, TEXT_ALIGN_CENTER);
        text_draw("- Открыть существующий уровень -", size, w, h + size, TEXT_ALIGN_CENTER);

        return;
    }
}

// Рисует сетку.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void grid_draw(void)
{
    // Задаем цвет сетки.
    if (SDL_SetRenderDrawColor(render, 100, 100, 100, 200) != 0)
    {
        crash("grid_draw(), не удалось задать цвет линий.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // ТЕСТ.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    for (size_t x = 0; x < w; x += SPRITE_SIZE)
    {
        if (SDL_RenderDrawLine(render, x, 0, x, h) != 0)
        {
            crash("grid_draw(), не удалось отрисовать вертикальную линию сетки\nSDL_GetError() : %s",
                  SDL_GetError());
        }
    }
    for (size_t y = 0; y < h; y += SPRITE_SIZE)
    {
        if (SDL_RenderDrawLine(render, 0, y, w, y) != 0)
        {
            crash("grid_draw(), не удалось отрисовать горизонтальную линию сетки\nSDL_GetError() : %s",
                  SDL_GetError());
        }
    }

}

// Отрисовка курсора.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
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
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
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
    rect.w = w - 8;
    rect.h = SPRITE_SIZE - 8;
    rect.x = 4;
    rect.y = 4;

    // Рисуем панель информации.
    if (SDL_RenderFillRect(render, &rect) != 0)
    {
        crash("info_draw(), не удалось нарисовать панель информации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Информация о курсоре.
    char info[100];

    // Информация о курсоре.
    sprintf(info, "Курсор    x: %i    y: %i",
            cursor_x,
            cursor_y);

    text_draw(info, 12, 10, 10, TEXT_ALIGN_LEFT);

    // Информация о точке рендеринга.
    sprintf(info, "Точка рендеринга    x: %i    y: %i",
            render_point_x,
            render_point_y);
    text_draw(info, 12, 10, 30, TEXT_ALIGN_LEFT);
}

// Отрисовка карты.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void map_draw(void)
{
    // Получаем размеры окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    // Определяем, сколько клеток видно на экране.
    const int count_x = w / SPRITE_SIZE + 1;
    const int count_y = h / SPRITE_SIZE + 1;

    // Сперва задаем всем текстурам стопроцентную непрозрачность, потому что
    // непрозрачность текстур могла быть изменена при отрисовке интерфейса.
    for (size_t t = 0; t < TEXTURES_COUNT; ++t)
    {
        if (SDL_SetTextureAlphaMod(textures[t], 255) != 0)
        {
            crash("map_draw(), не удалось задать текстурам уровня стопроцентную непрозрачность.\nSDL_GetError() : %s",
                  SDL_GetError());
        }
    }

    // Отрисовываем только те клетки, которые находятся в пределах видимости.
    for (int x = render_point_x; x < render_point_x + count_x; ++x)
    {
        for (size_t y = render_point_y; y < render_point_y + count_y; ++y)
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
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
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
// В случае ошибки показывает информацию о причине сбоя и кращит программу.
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
    rect.h = SPRITE_SIZE + 8;
    rect.x = 0;
    rect.y = h - SPRITE_SIZE - 8;

    // Рисуем панель выбора типа блока.
    if (SDL_RenderFillRect(render, &rect) != 0)
    {
        crash("menu_2_draw(), не удалось нарисовать панель информации.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Определяем, сколько спрайтов умещается по ширине экрана.
    const int count_x = w / SPRITE_SIZE + 1;

    // Рисуем список типов клеток в виде списка.
    for (uint8_t t = 0; t < count_x; ++t)
    {
        // Определяем положение элемента списка.
        const int x = t * SPRITE_SIZE;
        const int y = h - SPRITE_SIZE - 4;

        // Определяем тип клетки списка перед отрисовкой.
        // Клетка выбранного типа располагается в центре списка.
        const auto uint8_t type = menu_2_selected_item - (count_x / 2) + t;// Как это обозвать?

        // Определяем прозрачность элементов списка.
        uint8_t alpha = 255;
        // Индекс клетки, которая находится в центре списка.
        const int half = count_x / 2;

        // Защита от деления на ноль не нужна, так как
        // count_x в любом случае >= 1.
        //alpha = 255 * ( 1 - abs(half - t) / (float)(half) );

        // Зададим текстуре прозрачность.
        if (SDL_SetTextureAlphaMod(textures[type], alpha) != 0)
        {
            crash("menu_2_draw(), не удалось задать прозрачность текстуре.\nSDL_GetError() : %s",
                  SDL_GetError());
        }

        // Под центральным (выбранным) элементом списка рисуется выделение.
        if (t == half)
        {
            // Задаем цвет заливки.
            if (SDL_SetRenderDrawColor(render, 0, 255, 0, 255) != 0)
            {
                crash("menu_2_draw(), не удалось задать цвет заливки выделения выбранного типа блока.\nSDL_GetError() : %s",
                      SDL_GetError());
            }

            // Рисуем выделение.

            rect.h = SPRITE_SIZE + 8;
            rect.w = SPRITE_SIZE;
            rect.x = x;
            rect.y = y - 4;


            if (SDL_RenderFillRect(render, &rect) != 0)
            {
                crash("menu_2_draw(), не удалось нарисовать верхнюю линюю подсветки выделенного типа блока.\nSDL_GetError() : %s",
                      SDL_GetError());
            }

        }

        // Отрисуем спрайт.
        sprite_draw(textures[type], x, y);
    }
}
