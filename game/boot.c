#include "boot.h"
#include "window.h"
#include "render.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <locale.h>
#include <windows.h>
#include <SDL_syswm.h>

// Загружает текстуру.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture);

// Инициализирует систему.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void system_init(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("system_init(), попытка повторной инициализации системы.\n");
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
    // ...
}

// Загружает текстуры.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void textures_load(void)
{
    // ...
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
                         SDL_Texture **const _texture)
{

}
