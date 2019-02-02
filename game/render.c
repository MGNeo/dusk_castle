#include "render.h"
#include "window.h"
#include "crash.h"

SDL_Renderer *render;

void render_create(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("render_create(), произошла попытка повторного создания окна.");
    }

    // Создание рендера.
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (render == NULL)
    {
        crash("render_create(), не удалось создать рендер.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    // Задание рендеру режима смешивания.
    if (SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND) != 0)
    {
        crash("render_create(), не удалось задать рендеру режим смешивания.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}
