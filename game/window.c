#include "window.h"
#include "crash.h"

SDL_Window *window;

void window_create(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("window, window_create(), произошла попытка повторного создания окна.");
    }

    // Создадим окно.
    window = SDL_CreateWindow(u8"Сумеречный замок",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640,
                              480,
                              SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        crash("window, window_create(), не удалось создать окно.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}
