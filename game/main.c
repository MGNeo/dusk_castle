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

// TODO: Не нужно просчитывать и отрисовывать 100500 раз в секунду.
