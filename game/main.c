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

// Явно указать у всех определений функций extern или static.
