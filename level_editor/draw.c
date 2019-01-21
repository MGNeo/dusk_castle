#include "draw.h"
#include "glyphs.h"
#include "render.h"
#include "crash.h"
#include "text.h"
#include "menu_1.h"
#include "window.h"
#include "sprite.h"

#include <string.h>
#include <SDL.h>

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
    SDL_GetWindowSize(window, &w, &h);// Нужен размер клиентской области.

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
void draw_grid(void)
{
    // Задаем цвет сетки.
    const int r_code = SDL_SetRenderDrawColor(render, 155, 155, 155, 255);
    if (r_code != 0)
    {
        crash("draw_grid(), не удалось задать цвет сетки.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // ТЕСТ.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    for (size_t x = 0; x < w; x += SPRITE_WIDTH)
    {
        SDL_RenderDrawLine(render, x, 0, x, h);
    }
}
