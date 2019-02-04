#include "scene_authors.h"
#include "dt.h"

// Сцена-авторы.
// Игнорирует значение _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern scene_return_value scene_authors(const size_t _param)
{
    while (1)
    {
        dt_calculate();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type = SDL_QUIT)
            {
                exit(0);
            }
        }
    }
}
