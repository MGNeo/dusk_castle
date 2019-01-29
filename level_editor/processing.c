#include "processing.h"
#include "menu_1.h"
#include "cursor.h"
#include "crash.h"
#include "map.h"
#include "sprite.h"
#include "render_point.h"
#include "menu_2.h"
#include "statistics.h"
#include "window.h"
#include "hwnd.h"

#include <limits.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Сброс положения курсора.
static void cursor_reset(void);
// Сброс положения точки рендера.
static void render_point_reset(void);
// Сброс состояния карты.
static void map_reset(void);

// Диалог загрузки карты из файла.
static int map_load_dialog(void);
// Диалог сохранения карты в файл.
static int map_save_dialog(void);

// Загрузка карты из файла.
static int map_load(const char *const _file_name);
// Сохранение карты в файл.
static int map_save(const char *const _file_name);

// Сброс меню 1.
static void menu_1_reset(void);
// Сброс меню 2.
static void menu_2_reset(void);

// Обрабатывает статистику, подсчитывая количество блоков каждого типа на карте.
void statistics_reset(void)
{
    memset(statistics, 0, sizeof(size_t) * UNITS_COUNT);
    for (size_t x = 0; x < MAP_WIDTH; ++x)// Что быстрее x->y или y->x?
    {
        for (size_t y = 0; y < MAP_HEIGHT; ++y)
        {
            ++statistics[map[x][y]];
        }
    }
}

// Обрабатывает выделение и активацию пунктов меню.
// Если пункт меню был активирован, и активация прошла успешно, возвращает 1, иначе 0.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
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

        // Если активировали выбранный пункт меню.
        if (_event->key.keysym.sym == SDLK_SPACE)
        {
            switch (menu_1_selected_item)
            {
                case (MENU_1_NEW):
                {
                    // Сбрасываем положение курсора.
                    cursor_reset();
                    // Сбрасываем положение точки рендера.
                    render_point_reset();
                    // Сбрасываем состояние карты.
                    map_reset();
                    // Сбрасываем меню выбора блока,
                    menu_2_reset();
                    // Сбрасываем статистику.
                    statistics_reset();

                    return 1;
                }
                case (MENU_1_OPEN):
                {
                    // Загружаем карту из файла.
                    if (map_load_dialog() == 1)
                    {
                        // Сбрасываем положение курсора.
                        cursor_reset();
                        // Сбрасываем положение точки рендера.
                        render_point_reset();
                        // Сбрасываем первое меню.
                        menu_1_reset();
                        // Сбрасываем статистику.
                        statistics_reset();

                        return 1;
                    }
                    return 0;
                }
                default:
                {
                    crash("menu_1_processing(), выбран неизвестный пункт меню: %Iu",
                          menu_1_selected_item);
                    break;
                }
            }
        }
    }

    return 0;
}

// Обрабатывает курсор: перемещение, смену типа блока, активацию.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
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
                --statistics[map[cursor_x][cursor_y]];
                // Если клетка карты была пуста, устанавливаем выделенный тип блока.
                if (map[cursor_x][cursor_y] == 0)
                {
                    map[cursor_x][cursor_y] = menu_2_selected_item;
                } else {
                    // Если клетка карты не пуста, делаем ее пустой.
                    map[cursor_x][cursor_y] = 0;
                }
                ++statistics[map[cursor_x][cursor_y]];
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
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
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

    // Такое решение приводит к дерганью точки рендеринга,
    // если окно уменьшается слишком сильно.
    //const int dx = cursor_x - render_point_x;
    //const int dy = cursor_y - render_point_y;

    // А такое не приводит.
    #define RPP_DX (cursor_x - render_point_x)
    #define RPP_DY (cursor_y - render_point_y)

    // Границы контроля.
    const int right_border = count_x - 1;
    const int left_border = 0;
    const int top_border = 1;
    const int bottom_border = count_y - 2;

    // Двигаем смещение, если курсор уехал за границу отображения.

    // Курсор вылез за правую границу отображения.
    if (RPP_DX > right_border)
    {
        render_point_x += RPP_DX - right_border;
    }

    // Курсор вылез за левую границу отображения.
    if (RPP_DX < left_border)
    {
        render_point_x += RPP_DX;
    }

    // Курсор вылез под нижнюю панель.
    if (RPP_DY > bottom_border)
    {
        render_point_y += RPP_DY - bottom_border;
    }

    // Курсор вылез под верхнюю панель.
    if (RPP_DY < top_border)
    {
        render_point_y += RPP_DY - top_border;
    }

    #undef RPP_DX
    #undef RPP_DY
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
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
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
                // Контролируем переполнение через верхнюю границу диапазона юнитов.
                if (menu_2_selected_item == UNITS_COUNT)
                {
                    menu_2_selected_item = 0;
                }
                break;
            }
            case (SDLK_q):
            {
                --menu_2_selected_item;
                // Контролируем переполнение через нижнюю границу диапазона юнитов.
                if (menu_2_selected_item == UINT8_MAX)
                {
                    menu_2_selected_item = UNITS_COUNT - 1;
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

// Обработка кнопки f8 (сохранение карты в файл).
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void f8_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("save_processing(), save_processing(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        if (_event->key.keysym.sym == SDLK_F8)
        {
            map_save_dialog();
        }
    }
}

// Сброс карты.
void map_reset(void)
{
    memset(map, 0, sizeof(uint8_t) * MAP_WIDTH * MAP_HEIGHT);
}

// Диалог загрузки карты из файла.
// В случае критической успешной загрузки возвращает 1, иначе 0.
int map_load_dialog(void)
{
    char file_name[MAX_PATH];
    file_name[0] = 0;

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);

    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) != 0)
    {
        printf("\nДля загрузки карты выбран файл: %s\n", file_name);

       if (map_load(file_name) == 1)
       {
           return 1;
       }
    }

    return 0;
}
// Диалог сохранения карты в файл.
// В случае успешного сохранения возвращает 1, иначе 0.
int map_save_dialog(void)
{
    char file_name[MAX_PATH];
    file_name[0] = 0;

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);

    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

    if (GetSaveFileName(&ofn) != 0)
    {
        printf("\nДля сохранения карты выбран файл: %s\n", file_name);

        if (map_save(file_name) == 1)
        {
            return 1;
        }
    }

    return 0;
}

// Обработка нажатия Escape (возврат в первое меню).
// Возвращает 1, давая согласие на переход к первой сцене.
// Иначе возвращает 0.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
int escape_processing(const SDL_Event *const _event)
{
    if (_event == NULL)
    {
        crash("escape_processing(), _event == NULL");
    }

    if (_event->type == SDL_KEYDOWN)
    {
        if (_event->key.keysym.sym == SDLK_ESCAPE)
        {
            // Предлагаем пользователю сохранить результаты его работы.
            const int m = MessageBox(hwnd, "Сохранить карту?", "Выход в первое меню", MB_YESNO);

            switch (m)
            {
                // Пользователь не хочет сохранить карту.
                case (IDNO):
                {
                    return 1;
                }
                // Пользователь хочет сохранить карту.
                case (IDYES):
                {
                    // Пытаемся сохранить карту.
                    if (map_save_dialog() == 1)
                    {
                        // Сохранить удалось.
                        return 1;
                    }
                    // Сохранить не удалось.
                    return 0;
                }
                default:
                {
                    crash("escape_processing(), неизвестный код возврата MessageBox().");
                }
            }
        }
    }
    return 0;
}

// Сброс меню 1.
static void menu_1_reset(void)
{
    menu_1_selected_item = 0;
}
// Сброс меню 2.
static void menu_2_reset(void)
{
    menu_2_selected_item = 0;
}

// Обработка автоматического сохранения.
void auto_save_processing(const float _dt)
{
    // Резервное сохранение выполняется раз в 5 минут.
    static float t = 0.f;
    t += _dt;
    if (t < 300.f)
    {
        return;
    }
    t = 0;

    // Храним 5 последних сохранений.
    static size_t index = 4;
    ++index;
    index %= 5;

    // Формируем имя файла.
    char file_name[MAX_PATH] = {0};
    sprintf(file_name, "maps/temp%Iu", index);

    printf("\nАвтоматическое сохранение...\n");

    map_save(file_name);

    return;
}

// Загрузка карты из файла.
// В случае успешной загрузки возвращает 1, иначе 0.
static int map_load(const char *const _file_name)
{
    if (_file_name == NULL)
    {
        printf("map_load(), _file_name == NULL\n");
        return 0;
    }

    if (strlen(_file_name) == 0)
    {
        printf("map_load(), strlen(_file_name) == 0\n");
        return 0;
    }

    // Пытаемся открыть файл.
    FILE *f = fopen(_file_name, "rb");
    if (f == NULL)
    {
        printf("map_load(), не удалось открыть файл.\n");
        printf("errno: %i\n", errno);
        return 0;
    }
    printf("map_load(), файл успешно открыт.\n");

    // Закрытие файла.
    // Для избавления от дублирования.
    void file_close(void)
    {
        if (fclose(f) != 0)
        {
            printf("map_load(), не удалось закрыть файл.\n");
            printf("errno: %i\n", errno);
        } else {
            printf("map_load(), файл успешно закрыт.\n");
        }
    }

    // Определяем размер.

    // Переходим в конец файла.
    if (fseek(f, 0, SEEK_END) != 0)
    {
        printf("map_load(), не удалось перейти в конец файла.\n");
        printf("errno: %i\n", errno);
        file_close();
        return 0;
    }
    printf("map_load(), переход в конец файла успешно осуществлен.\n");

    // Определяем размер файла.
    const long size = ftell(f);
    if (size < 0)
    {
        printf("map_load(), не удалось определить размер файла.\n");
        printf("errno: %i\n", errno);
        file_close();
        return 0;
    }
    printf("map_load(), размер файла успешно определен: %ld байт.\n", size);

    // Проверяем размер на соответствие требуемому размеру.
    const int required_size = sizeof(uint8_t) * MAP_WIDTH * MAP_HEIGHT;
    if (size != required_size)
    {
        printf("map_load(), неверный размер файла, требуемый: %i\n", required_size);
        file_close();
        return 0;
    }
    printf("map_load(), файл имеет правильный размер.\n");

    // Переходим в начало файла.
    if (fseek(f, 0, SEEK_SET) != 0)
    {
        printf("map_load(), не удалось перейти в начало файла.\n");
        printf("errno: %i\n", errno);
        file_close();
        return 0;
    }
    printf("map_load(), переход в начало файла успешно осуществлен.\n");

    // Грузим данные.
    if (fread(map, size, 1, f) != 1)
    {
        printf("map_load(), не удалось загрузить данные из файла.\n");
        printf("errno: %i\n", errno);
        file_close();
        return 0;
    }
    printf("map_load(), данные успешно загружены.\n");

    // Закрываем файл.
    file_close();

    // Проверяем, все ли ячейки содержат корректный ID.
    for (size_t x = 0; x < MAP_WIDTH; ++x)
    {
        for (size_t y = 0; y < MAP_HEIGHT; ++y)
        {
            if (map[x][y] >= UNITS_COUNT)
            {
                printf("map_load(), некорректная карта, map[%Iu][%Iu] == %Iu\n", x, y, (size_t)map[x][y]);
                return 0;
            }
        }
    }
    printf("map_load(), корректная карта.\n");

    return 1;
}
// Сохранение карты в файл.
// В случае успешного сохранения возвращает 1, иначе 0.
static int map_save(const char *const _file_name)
{
    if (_file_name == NULL)
    {
        printf("map_save(), _file_name == NULL\n");
        return 0;
    }

    if (strlen(_file_name) == 0)
    {
        printf("map_save(), strlen(_file_name) == 0\n");
        return 0;
    }

    // Пытаемся открыть файл.
    FILE *f = fopen(_file_name, "wb");
    if (f == NULL)
    {
        printf("map_save(), не удалось открыть файл.\n");
        printf("errno: %i\n", errno);
        return 0;
    }
    printf("map_save(), файл успешно открыт.\n");

    // Закрытие файла.
    // Для избавления от дублирования.
    void file_close(void)
    {
        if (fclose(f) != 0)
        {
            printf("map_save(), не удалось закрыть файл.\n");
            printf("errno: %i\n", errno);
        } else {
            printf("map_save(), файл успешно закрыт.\n");
        }
    }

    // Записываем.
    if (fwrite(map, sizeof(uint8_t) * MAP_WIDTH * MAP_HEIGHT, 1, f) != 1)
    {
        printf("map_save(), не удалось записать данные в файл.\n");
        printf("errno: %i\n", errno);
        file_close();
        return 0;
    }
    printf("map_save(), данные успешно записаны в файл.\n");

    // Закрываем файл.
    file_close();

    return 1;
}
