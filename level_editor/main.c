#include "boot.h"
#include "editor.h"

int main(int argc, char **argv)
{
    system_init();
    glyphs_load();
    textures_load();

    editor();

    return 0;
}

// TODO: Не нужно просчитывать и отрисовывать 100500 раз в секунду.
