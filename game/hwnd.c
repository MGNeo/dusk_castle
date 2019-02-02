#include "hwnd.h"
#include "SDL_syswm.h"
#include "window.h"
#include "crash.h"

HANDLE hwnd;

void hwnd_create(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("hwnd_create(), произошла попытка повторного создания hwnd.");
    }

     // Для того, чтобы диалоговые окна правильно блокировали родительское окно,
    // им необходим хэндл родительского окна.
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(window, &info) != 1)
    {
        crash("hwnd_create(), не удалось получить hwnd окна игры.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}
