#include "resources_high.h"
#include "resources_medium.h"

// Инициализирует и загружает все ресурсы.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void resources_init_and_load(void)
{
    system_init();

    font_load();

    textures_load();
}
