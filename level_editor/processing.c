#include "processing.h"
#include "menu_1.h"
#include "crash.h"
#include "window.h"// Тестовое.

#include <windows.h>

// Обрабатывает выделение и активацию пунктов меню.
// Если пункт меню был активирован, возвращает 1, иначе 0.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
int menu_1_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("menu_1_processing(), _event == NULL\n");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        // Если нажали Up или Down, то выделяем другой пункт меню.
        if ( (_event->key.keysym.sym == SDLK_UP) ||
             (_event->key.keysym.sym == SDLK_DOWN) )
        {
            menu_1_selected_item = !menu_1_selected_item;
            return 0;
        }

        // Если нажали Enter, то активируем пункт меню.
        if (_event->key.keysym.sym == SDLK_RETURN)
        {
            /*char buffer1[1000];
            char* _buffer_=(char*)(buffer1);
            buffer1[0]=0;
            OPENFILENAME w1;
            memset(&w1,0,sizeof(OPENFILENAME));
            w1.lStructSize=sizeof(OPENFILENAME);
            // Возможно, удастся получить хэндл окна,
            // чтобы диалог выбора файла прицепился к окну редактора.
            w1.hwndOwner=NULL;
            w1.hInstance=NULL;
            w1.lpstrFilter=NULL;
            w1.lpstrCustomFilter=NULL;
            w1.lpstrFile=_buffer_;
            w1.nMaxFile=1000;
            w1.lpstrFileTitle=NULL;
            w1.lpstrInitialDir=NULL;
            w1.lpstrTitle=NULL;
            w1.Flags=OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY OFN_;
            GetOpenFileName(&w1);*/

            return 1;
        }
    }

    return 0;
}
