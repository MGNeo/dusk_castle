#include "glyphs.h"
#include "crash.h"
#include "render.h"

#include <SDL_ttf.h>
#include <stdlib.h>
#include <windows.h>

glyph glyphs[FONTS_COUNT][256];

// Загружает глифы.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void glyphs_load(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("glyphs_load(), попытка повторной загрузки глифов.");
    }

    // Цвет шрифта.
    SDL_Color h_color = {255, 255, 255, 255};

    // Загружаем шрифты нужного размера.
    for (size_t s = 0; s < FONTS_COUNT; ++s)
    {
        TTF_Font *h_font = TTF_OpenFont("font/font.ttf", MIN_FONT_SIZE + s);
        if (h_font == NULL)
        {
            crash("glyphs_load(), не удалось загрузить шрифт размера: %Iu\nTTF_GetError() : %s",
                  MIN_FONT_SIZE + s,
                  TTF_GetError());
        }

        // Заполним глифы.
        for (size_t g = 0; g < 256; ++g)
        {
            const char ansi_char = (char)g;
            // Защита от переполнения, если символ представлен суррогатной парой.
            Uint16 unicode_char[2] = {0};
            // Преобразуем символ из текущей кодировки (cp1251) в юникод.
            const int r_code = MultiByteToWideChar(1251,
                                                   0,
                                                   &ansi_char,
                                                   1,
                                                   unicode_char,
                                                   1);

            if (r_code != 1)
            {
                crash("glyphs_load(), не удалось конвертировать символ из cp1251 в unicode.\nGetLastError() : %lu",
                      GetLastError());
            }

            if (unicode_char[1] != 0)
            {
                crash("glyphs_load(), при конвертации ANSI символа [%c] получилась суррогатная пара UTF16-LE, а не единственный 16-ти битный символ.",
                      ansi_char);
            }

            // Отрендерим символ в поверхность.
            SDL_Surface *h_surface = TTF_RenderGlyph_Blended(h_font, unicode_char[0], h_color);

            // Если для символа есть графическое представление.
            // Важно, для символа может быть графическое представление со всеми прозрачными пикселями.
            if (h_surface != NULL)
            {
                // Сформируем текстуру на основе поверхности.
                SDL_Texture *h_texture = SDL_CreateTextureFromSurface(render, h_surface);
                if (h_texture == NULL)
                {
                    crash("glyphs_load(), не удалось создать текстуру на основе поверхности с символом.\nSDL_GetError() : %s",
                          SDL_GetError());
                }

                // Инициализируем глиф.
                glyphs[s][g].w = h_surface->w;
                glyphs[s][g].h = h_surface->h;
                glyphs[s][g].texture = h_texture;

                // Освобождаем вспомогательную поверхность.
                SDL_FreeSurface(h_surface);
            }
        }

        TTF_CloseFont(h_font);
    }
}
