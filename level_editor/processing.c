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
#include <stdlib.h>
#include <stdio.h>

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
        if ( (_event->key.keysym.sym == SDLK_w) ||
             (_event->key.keysym.sym == SDLK_s) )
        {
            menu_1_selected_item = !menu_1_selected_item;
            return 0;
        }

        // Если нажали Enter, то активируем пункт меню.
        if (_event->key.keysym.sym == SDLK_SPACE)
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
            case (SDLK_w):
            {
                --cursor_y;
                if (cursor_y < 0)
                {
                    cursor_y = 0;
                }
                break;
            }
            case (SDLK_d):
            {
                ++cursor_x;
                if (cursor_x > MAP_WIDTH - 1)
                {
                    cursor_x = MAP_WIDTH - 1;
                }
                break;
            }
            case (SDLK_s):
            {
                ++cursor_y;
                if (cursor_y > MAP_HEIGHT - 1)
                {
                    cursor_y = MAP_HEIGHT - 1;
                }
                break;
            }
            case (SDLK_a):
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

    // Границы контроля.
    const int right_border = count_x - 1;
    const int left_border = 0;
    const int top_border = 1;
    const int bottom_border = count_y - 2;

    // Двигаем смещение, если курсор уехал за границу отображения.

    // Курсор вылез за правую границу отображения.
    if (dx > right_border)
    {
        render_point_x += dx - right_border;
    }

    // Курсор вылез за левую границу отображения.
    if (dx < left_border)
    {
        render_point_x += dx;
    }

    // Курсор вылез под нижнюю панель.
    if (dy > bottom_border)
    {
        render_point_y += dy - bottom_border;
    }

    // Курсор вылез под верхнюю панель.
    if (dy < top_border)
    {
        render_point_y += dy - top_border;
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
            case (SDLK_e):
            {
                ++menu_2_selected_item;
                break;
            }
            case (SDLK_q):
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

// Обработка сохранения: детектирование нажатия Escape и процесс сохранения.
// В случае ошибки показывает информацию о причине сбоя и крашит программу.
void save_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("save_processing(), save_processing(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        if (_event->key.keysym.sym == SDLK_ESCAPE)
        {
            char file_name[MAX_PATH];
            file_name[0] = 0;

            OPENFILENAME ofn;
            memset(&ofn, 0, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);

            // Возможно, удастся получить хэндл окна,
            // чтобы диалог выбора файла прицепился к окну редактора.

            ofn.hwndOwner = NULL;
            ofn.hInstance = NULL;
            ofn.lpstrFilter = NULL;
            ofn.lpstrCustomFilter = NULL;
            ofn.lpstrFile = file_name;
            ofn.nMaxFile = MAX_PATH;// Считается ли нультерминатор частью имени файла?
            ofn.lpstrFileTitle = NULL;
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            if (GetSaveFileName(&ofn) != 0)
            {
                printf("file_name: %s\n", file_name);

                FILE *f = fopen(file_name, "wb");
                // Не удалось открыть файл.
                if (fopen == NULL)
                {
                    char message[1024];

                    sprintf(message, "Невозможно сохранить карту в выбранный файл.\nGetLastError() : %lu",
                            GetLastError());

                    MessageBox(NULL, message, "Ошибка сохранения", MB_ICONSTOP);

                    return;
                }

                // Записываем.
                if (fwrite(map, sizeof(uint8_t) * MAP_WIDTH * MAP_HEIGHT, 1, f) != 1)
                {
                    crash("save_processing(), произошла ошибка записи карты в файл.\n");
                }

                // Debug
                fread(map, sizeof(uint8_t) * MAP_WIDTH * MAP_HEIGHT, 1, f);

                if (fclose(f) != 0)
                {
                    crash("save_processing(), невозможно закрыть файл, отказ fclose()");
                }
            }
        }
    }
}
