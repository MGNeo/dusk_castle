#include "game.h"
#include "boot.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    int i = 700;
    float f = i;
    printf("%7.7f", f);

    system_init();

    glyphs_load();

    textures_load();

    sounds_load();

    game();

    return 0;
}
