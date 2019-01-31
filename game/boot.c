#include "boot.h"
#include "window.h"
#include "render.h"
#include "glyphs.h"
#include "textures.h"
#include "crash.h"
#include "sprite.h"
#include "frames.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <locale.h>
#include <windows.h>
#include <SDL_syswm.h>

// Загружает текстуру.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture,
                         const size_t _frames_count);

// Инициализирует систему.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
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

    //Инициализация SDL_ttf.
    if (TTF_Init() != 0)
    {
        crash("system_init(), не удалось инициализировать SDL_ttf.\nTTF_GetError() : %s",
              TTF_GetError());
    }

    // Инициализация SDL_mixer.
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 128) != 0)
    {
        crash("system_init(), не удалось инициализировать SDL_mixer.\nMix_GetError() : %s",
              Mix_GetError());
    }

    // Установим количество смешиваемых каналов.
    Mix_AllocateChannels(128);

    // Создадим окно.
    window = SDL_CreateWindow(u8"Сумеречный замок",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640,
                              480,
                              SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        crash("system_init(), не удалось создать окно.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Для того, чтобы диалоговые окна правильно блокировали родительское окно,
    // им необходим хэндл родительского окна.
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(window, &info) != 1)
    {
        crash("system_init(), не удалось получить hwnd окна игры.\nSDL_GetError() : %s",
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
        for (size_t g = 0; g < 255; ++g)
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
                    crash("font_load(), не удалось создать текстуру на основе поверхности с символом.\nSDL_GetError() : %s",
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

// Загружает текстуры.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void textures_load(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("textures_load(), попытка повторной загрузки текстур.\n");
    }

    // Игрок.
    texture_load("textures/player.bmp", &texture_player, PLAYER_FRAMES_COUNT);

    // Старт.
    texture_load("textures/start.bmp", &texture_start, START_FRAMES_COUNT);

    // Финиш.
    texture_load("textures/finish.bmp", &texture_finish, FINISH_FRAMES_COUNT);

    // Стена.
    texture_load("textures/wall.bmp", &texture_wall, WALL_FRAMES_COUNT);

    // Лестница.
    texture_load("textures/ladder.bmp", &texture_ladder, LADDER_FRAMES_COUNT);

    // Шипы.
    texture_load("textures/stakes.bmp", &texture_stakes, STAKES_FRAMES_COUNT);

    // Токсичная слизь.
    texture_load("textures/toxic.bmp", &texture_toxic, TOXIC_FRAMES_COUNT);

    // Призрак.
    texture_load("textures/ghost.bmp", &texture_ghost, GHOST_FRAMES_COUNT);

    // Летучая мышь.
    texture_load("textures/bat.bmp", &texture_bat, BAT_FRAMES_COUNT);

    // Серебрянная монетка.
    texture_load("textures/silver coin.bmp", &texture_silver_coin, SILVER_COIN_FRAMES_COUNT);

    // Золотая монетка.
    texture_load("textures/gold coin.bmp", &texture_gold_coin, GOLD_COIN_FRAMES_COUNT);

}

// Загружает звуки.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void sounds_load(void)
{
    // ...
}

// Загружает текстуру.
// В случае критической ошибки показывает ионформацию о причине сбоя и крашит программу.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture,
                         const size_t _frames_count)
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
        crash("texture_load(), _texture == NULL");
    }

    if (_frames_count == 0)
    {
        crash("texture_load(), _frames_count == 0");
    }

    if (_frames_count > MAX_FRAMES_COUNT)
    {
        crash("texture_load(), _frames_count > MAX_FRAMES_COUNT");
    }

    // Пытаемся создать поверхность, загрузив данные из файла.
    SDL_Surface *h_surface = SDL_LoadBMP(_file_name);
    if (h_surface == NULL)
    {
        crash("texture_load(), не удалось создать поверхность на оснвое файла: %s\nSDL_GetError() : %s",
              _file_name,
              SDL_GetError());
    }

    // Проверяем высоту текстуры на соответствие требованиям.
    if (h_surface->h != SPRITE_SIZE)
    {
        crash("texture_load(), неверная высота текстуры.\nТекущая: %i\nТребуемая: %i",
              h_surface->h,
              SPRITE_SIZE);
    }
    // Вычисляем заданную ширину.
    // Целочисленное переполнение знакового возникнуть не может.
    const int required_w = SPRITE_SIZE * _frames_count;
    // Проверяем ширину текстуры на соответствие требованиям.
    if (h_surface->w != required_w)
    {
        crash("texture_load(), неверная ширина текстуры.\nТекущая: %i\nТребуемая: %i",
              h_surface->w,
              required_w);
    }

    // Пытаемся создать текстуру на основе поверхности.
    SDL_Texture *h_texture = SDL_CreateTextureFromSurface(render, h_surface);
    if (h_surface == NULL)
    {
        crash("texture_load(), не удалось создать текстуру на основе поверхности.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    SDL_FreeSurface(h_surface);

    *_texture = h_texture;
}
