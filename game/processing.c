#include "processing.h"
#include "crash.h"
#include "menu.h"

// Обработка выбора и активации пунктов меню.
// Возвращает -1, давая команду на выход.
// Если пункт меню не активирован, возвращает 0.
// Возвращает 1, давая команду на переход к игре.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
int menu_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("menu_1_processing(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        if ( (_event->key.keysym.sym == SDLK_w) ||
             (_event->key.keysym.sym == SDLK_s) )
        {
            menu_selected_item = !menu_selected_item;
            return 0;
        }
        if (_event->key.keysym.sym == SDLK_SPACE)
        {
            if (menu_selected_item == 0)
            {
                return 1;
            } else {
                return -1;
            }
        }
    }
}
