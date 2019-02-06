#include "boot.h"
#include "window.h"
#include "render.h"
#include "hwnd.h"
#include "glyphs.h"
#include "textures.h"
#include "crash.h"
#include "systems.h"

#include <stdlib.h>

// Инициализирует все системы и загружает все ресурсы.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void boot(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("boot.c, boot(), произошла попытка повторной инициализации всех систем и загрузки всех ресурсов.");
    }

    systems_init();

    window_create();

    hwnd_create();

    render_create();

    glyphs_load();

    textures_load();
}
