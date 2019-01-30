#include "game.h"
#include "boot.h"

int main(int argc, char **argv)
{
    system_init();

    glyphs_load();

    textures_load();

    sounds_load();

    return 0;
}
