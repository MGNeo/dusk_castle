#include "dt.h"

#include <SDL.h>
#include <stdlib.h>

// Вычисляет dt итерации игрового цикла.
// За одну итерацию игрового цикла функция должна вызываться один раз.
// Функция должна вызываться первой в в цикле любой сцены.
float dt_calculate(void)
{
    static Uint64 t1, t2;
    t2 = t1;
    t1 = SDL_GetPerformanceCounter();
    // dt первого кадра определить невозможно.
    static size_t first;
    if (first == 0)
    {
        ++first;
        return 0.f;
    }

    // Контроль теоретически возможного переполнения Uint64.
    if (t2 > t1)
    {
        return (t1 + (SDL_MAX_UINT64 - t2)) / (float) SDL_GetPerformanceFrequency();
    } else {
        return (t1 - t2) / (float) SDL_GetPerformanceFrequency();
    }
}
