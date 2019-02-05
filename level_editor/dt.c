#include "dt.h"
#include <SDL.h>

static Uint64 begin;

extern void  dt_reset(void)
{
    begin = SDL_GetPerformanceCounter();
}
extern float dt_get(void)
{
    const Uint64 end = SDL_GetPerformanceCounter();
    // Контроль теоретически возможного переполнения Uint64.
    if (begin <= end)
    {
        return (end - begin) / ((float)SDL_GetPerformanceFrequency());
    } else {
        return (begin + (UINT64_MAX - end)) / ((float)SDL_GetPerformanceFrequency());
    }
}
