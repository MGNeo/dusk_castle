#include "systems.h"

#include <stdlib.h>
#include <locale.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <windows.h>
#include <SDL_ttf.h>

// Иницализирует все систем.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern void systems_init(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("systems_init(), произошла попытка повторной инициализации всех систем.");
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
}
