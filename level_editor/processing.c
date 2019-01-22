#include "processing.h"
#include "menu_1.h"
#include "cursor.h"
#include "crash.h"
#include "map.h"
#include "window.h"// Тестовое.
#include "sprite.h"
#include "render_point.h"
#include "menu_2.h"

#include <windows.h>

// Обрабатывает выделение и активацию пунктов меню.
// Если пункт меню был активирован, возвращает 1, иначе 0.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
int menu_1_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("menu_1_processing(), _event == NULL");
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

// Обрабатывает курсор: перемещение, смену типа блока, активацию.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void cursor_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("cursor_processing(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        switch (_event->key.keysym.sym)
        {
            case (SDLK_UP):
            {
                --cursor_y;
                if (cursor_y < 0)
                {
                    cursor_y = 0;
                }
                break;
            }
            case (SDLK_RIGHT):
            {
                ++cursor_x;
                if (cursor_x > MAP_WIDTH - 1)
                {
                    cursor_x = MAP_WIDTH - 1;
                }
                break;
            }
            case (SDLK_DOWN):
            {
                ++cursor_y;
                if (cursor_y > MAP_HEIGHT - 1)
                {
                    cursor_y = MAP_HEIGHT - 1;
                }
                break;
            }
            case (SDLK_LEFT):
            {
                --cursor_x;
                if (cursor_x < 0)
                {
                    cursor_x = 0;
                }
                break;
            }
            case (SDLK_SPACE):
            {
                if (map[cursor_x][cursor_y] == 0)
                {
                    map[cursor_x][cursor_y] = menu_2_selected_item;
                } else {
                    map[cursor_x][cursor_y] = 0;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

// Обрабатывает точку рендеринга.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void render_point_processing(void)
{
    // Контроль переполнения не нужен, так как границы карты лежат
    // далеко от границ int.

    // Определяем размеры окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Определяем размеры окна в клетках.
    const int count_x = w / SPRITE_SIZE;
    const int count_y = h / SPRITE_SIZE;

    // Определяем положение курсора относительно точки рендеринга.
    const int dx = cursor_x - render_point_x;
    const int dy = cursor_y - render_point_y;

    // Двигаем смещение, если курсор уехал за границу отображения.

    // Курсор вылез за правую границу отображения.
    if (dx > count_x - 1)
    {
        render_point_x += dx - (count_x - 1);
    }

    // Курсор вылез за левую границу отображения.
    if (dx < 1)
    {
        render_point_x += dx;
    }

    // Курсор вылез за нижнюю границу отображения.
    if (dy > count_y - 1)
    {
        render_point_y += dy - (count_y - 1);
    }

    // Курсор вылез за верхнюю границу отображения.
    if (dy < 1)
    {
        render_point_y += dy;
    }
}

// Выставляет курсор в центр уровня.
void cursor_reset(void)
{
    cursor_x = MAP_WIDTH / 2;
    cursor_y = MAP_HEIGHT / 2;
}

// Выставляет точку рендера в центр уровня.
void render_point_reset(void)
{
    render_point_x = MAP_WIDTH / 2;
    render_point_y = MAP_HEIGHT / 2;
}

// Обработка меню №2.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void menu_2_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("menu_2_processing(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        switch (_event->key.keysym.sym)
        {
            case (SDLK_PAGEUP):
            {
                ++menu_2_selected_item;
                break;
            }
            case (SDLK_PAGEDOWN):
            {
                --menu_2_selected_item;
                break;
            }
            default:
            {
                break;
            }
        }
    }
}
