#include "resources_medium.h"
#include "resources_low.h"
#include "crash.h"
#include <stdlib.h>
#include <SDL.h>
#include <locale.h>
#include <windows.h>

// Инициализирует систему.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
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
        crash("system_init(), не удалось инициализировать SDL\nSDL_GetError() : %s",
              SDL_GetError());
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
        crash("system_init(), не удалось создать окно\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Создание рендера.
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (render == NULL)
    {
        crash("system_init(), не удалось создать рендер\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Задание рендеру режима смешивания.
    if (SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND) != 0)
    {
        crash("system_init(), не удалось задать рендеру режим смешивания\nSDL_GetError() : %s",
              SDL_GetError());
    }
}

// Загружает шрифт.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void font_load(void)
{

}

// Загружает текстуры.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void textures_load(void)
{

}
