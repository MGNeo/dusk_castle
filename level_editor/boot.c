#include "boot.h"
#include "window.h"
#include "render.h"
#include "glyphs.h"
#include "textures.h"
#include "crash.h"

#include <windows.h>
#include <locale.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

// Загружает текстуру.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture);

// Инициализирует систему.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void system_init(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("system_init(), попытка повторной инициализации системы.");
    }

    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Инициализация SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        crash("system_init(), не удалось инициализировать SDL.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Инициализация SDL_ttf.
    if (TTF_Init() != 0)
    {
        crash("system_init(), не удалось инициализировать SDL_ttf.\nTTF_Init() != 0\nTTF_GetError() : %s",
              TTF_GetError());
    }

    // Создание окна.
    window = SDL_CreateWindow(u8"Редактор уровней Сумеречного замка",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640,// Вынести в константы...
                              480,
                              SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        crash("system_init(), не удалось создать окно.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Создание рендера.
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (render == NULL)
    {
        crash("system_init(), не удалось создать рендер.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Задание рендеру режима смешивания.
    if (SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND) != 0)
    {
        crash("system_init(), не удалось задать рендеру режим смешивания.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}

// Загружает глифы.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void glyphs_load(void)
{
    // Защита от повторного вызова.
    static size_t again = 0;
    if (again++ != 0)
    {
        crash("glyphs_load(), попытка повторной загрузки глифов.");
    }

    // Цвет шрифта.
    SDL_Color h_color = {255, 255, 255, 255};

    // Загружаем все нужные размеры шрифтов.
    for (size_t s = 0; s < FONTS_COUNT; ++s)
    {
        TTF_Font *h_font = TTF_OpenFont("font/font.ttf", MIN_FONT_SIZE + s);
        if (h_font == NULL)
        {
            crash("glyphs_load(), не удалось загрузить шрифт размера %Iu.\nTTF_GetError() : %s",
                  MIN_FONT_SIZE + s,
                  TTF_GetError());
        }
        // Заполним глифы.
        for (size_t g = 0; g < 256; ++g)
        {
            const char ansi_char = (char)g;
            // Защита от переполнения, если символ предстален суррогатной парой.
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
                crash("glyphs_load(), при конвертации ANSI символа [%c] получилась суррогатная пара UTF16-LE, а не единственный 16-ти символ.",
                      ansi_char);
            }

            // Отрендерим символ в поверхность.
            SDL_Surface *h_surface = TTF_RenderGlyph_Blended(h_font, unicode_char[0], h_color);

            // Если для символа есть графическое представление.
            // Важно, для символа может быть графическое представление со всеми прозрачными пикселями.
            if (h_surface != NULL)
            {
                // Сформируем на основе поверхности текстуру.
                SDL_Texture *h_texture = SDL_CreateTextureFromSurface(render, h_surface);
                if (h_texture == NULL)
                {
                    crash("font_load(), не удалось создать текстуру на основе поверхности с символом\nSDL_GetError() : %s",
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

// Загружает текстуру.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture)
{
    if (_file_name == NULL)
    {
        crash("texture_load(), _file_name == NULL");
    }

    if (strlen(_file_name) == 0)
    {
        crash("texture_load(), strlen(_file_name) == 0");
    }

    if (_texture == NULL)
    {
        crash("texure_load(), _texure == NULL");
    }

    // Пытаемся создать поверхность, загрузив данные из файла.
    SDL_Surface *h_surface = SDL_LoadBMP(_file_name);
    if (h_surface == NULL)
    {
        crash("texture_load(), не удалось создать поверхность на основе файла: %s\nSDL_GetError() : %s",
              _file_name,
              SDL_GetError());
    }

    // Пытаемся создать текстуру на основе поверхности.
    SDL_Texture *h_texture = SDL_CreateTextureFromSurface(render, h_surface);
    if (h_texture == NULL)
    {
        crash("texture_load(), не удалось создать текстуру на основе поверхности.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    SDL_FreeSurface(h_surface);

    *_texture = h_texture;
}

// Загружает текстуры.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void textures_load(void)
{
    // Защита от повторного вызова.
    static size_t again = 0;
    if (again++ != 0)
    {
        crash("textures_load(), попытка повторной загрузки текстур.");
    }
    // Загружаем все текстуры в один массив.
    for (size_t t = 0; t < TEXTURES_COUNT; ++t)
    {
        char file_name[] = "textures/texture****.bmp";
        sprintf(file_name, "textures/texture%04Iu.bmp", t);// Желателен контроль отказа.
        texture_load(file_name, &textures[t]);
    }

}
