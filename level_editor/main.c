#include "resources_high.h"
#include "editor.h"

int main(int argc, char **argv)
{
    resources_init_and_load();

    editor();

    return 0;
}
