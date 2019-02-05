#include "dt.h"

#include <SDL.h>
#include <stdlib.h>

static Uint64 begin;

// Установка новой точки отсчета времени.
extern void dt_reset(void)
{
    begin = SDL_GetPerformanceCounter();
}

// Подсчет количества секунд, которое прошло от точки отсчета до текущего момента.
extern float dt_get(void)
{
    const Uint64 end = SDL_GetPerformanceCounter();
    // Учитываем теоретическую возможность переполнения Uint64 (но только однократного переполнения).
    if (end >= begin)
    {
        return (end - begin) / ((float)SDL_GetPerformanceFrequency());
    } else {
        return (begin + (UINT64_MAX - end)) / ((float)SDL_GetPerformanceFrequency());
    }
}
