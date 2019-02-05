#include "game.h"
#include "boot.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    boot();

    game();

    return 0;
}


/* TODO: Сделать так:

    const float fps = 60.f;
    const float spf = 1 / fps;

    while (SDL_WaitEvent(&event, spf))
    {
        const float dt = dt_calculate();
        t += dt;

        // Обработка ввода.

        if (t > spf)
        {
            t = 0.f
            // Отрисовка и просчеты.
        }
    }

*/
