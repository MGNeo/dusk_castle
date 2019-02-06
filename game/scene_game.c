#include "scene_game.h"
#include "map.h"
#include "crash.h"
#include "units.h"
#include "enemy.h"
#include "frames.h"
#include "textures.h"
#include "player.h"
#include "animation.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_LEVEL 99

// Номер уровня.
static uint8_t level;
// Карта уровня.
static uint8_t map[MAP_WIDTH][MAP_HEIGHT];
// Количество серебра на счету игрока.
static size_t silver_score;
// Количество золота на счету игрока.
static size_t gold_score;
// Количество врагов.
static size_t enemies_count;
// Враги.
static enemy_unit enemies[MAX_ENEMIES];
// Игрок.
static player_unit player;

// Анимация серебрянной монетки.
static animation silver_animation;
// Анимация золотой монетки.
static animation gold_animation;
// Анимация токсичной жижи.
static animation toxic_animation;

// Загружает карту из файла.
static void map_load_from_file(void);
// Проверяет загруженную карту на корректность.
static void map_check_correctness(void);
// Строит уровень.
static void level_build(void);

// Добавляет призрака.
static void ghost_add(const float _x, const float _y);
// Добавляет мышь.
static void bat_add(const float _x, const float _y);
// Добавляет игрока.
static void player_add(const float _x,
                       const float _y);
// Сбрасывает анимацию серебряных монеток.
static void silver_animation_reset(void);
// Сбрасывает анимацию

// Сцена-игра.
// Обрабатывает _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern scene_return_value scene_game(const size_t _param)
{
    switch (_param)
    {
        case (SCENE_GAME_PARAM):
        {
            // nothing
            break;
        }
        case (SCENE_GAME_PARAM_RESET_LEVEL):
        {
            level = 0;
            break;
        }
        default:
        {
            crash("scene_game, scene_game(), неизвестное значение _param.");
        }
    }



    scene_return_value srv = {0, 0};
    return srv;
}

// Загружает карту из файла.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void map_load_from_file(void)
{
    if (level > MAX_LEVEL)
    {
        crash("scene_game, map_load_from_file(), задан уровень, который превышает максимальный.\nТекущий: %Iu\nМаксимальный: %Iu",
              (size_t)level,
              (size_t)MAX_LEVEL);
    }

    // Формируем имя файла.
    char file_name[] = "maps/map01";
    sprintf(file_name, "maps/map%Iu", (size_t)level);

    // Пытаемся открыть файл.
    FILE *f = fopen(file_name, "rb");
    if (f == NULL)
    {
        crash("scene_game, map_load_from_file(), не удалось открыть файл.\nerrno: %i",
              errno);
    }

    // Пытаемся перейти в конец файла.
    if (fseek(f, 0, SEEK_END) != 0)
    {
        crash("scene_game, map_load_from_file(), не удалось перейти в конец файла.\nerrno: %i",
              errno);
    }

    // Пытаемся определить размер файла.
    const long size = ftell(f);
    if (size < 0)
    {
        crash("scene_game, map_load_from_file(), не удалось определить размер файла.\nerrno: %i",
              errno);
    }

    // Проверяем размер файла на соответствие необходимому.
    const long required_size = sizeof(uint8_t) * MAP_WIDTH * MAP_HEIGHT;
    if (size != required_size)
    {
        crash("scene_game, map_load_from_file(), неправильный размер файла.\nТекущий: %ld\nТребуемый: %ld",
              size,
              required_size);
    }

    // Пытаемся перейти в начало файла.
    if (fseek(f, 0, SEEK_SET) != 0)
    {
        crash("scene_game, map_load_from_file(), не удалось перейти в начало файла.\nerrno: %i",
              errno);
    }

    // Пытаемся считать данные из файла.
    if (fread(map, size, 1, f) != 1)
    {
        crash("scene_game, map_load_from_file(), не удалось считать данные из файла.\nerrno: %i",
              errno);
    }

    // Пытаемся закрыть файл.
    if (fclose(f) != 0)
    {
        crash("scene_game, map_load_from_file(), не удалось закрыть файл.\nerrno: %i",
              errno);
    }
}

// Проверяет загруженную карту на корректность.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void map_check_correctness(void)
{
    size_t starts_count = 0;
    size_t finishes_count = 0;
    for (size_t x = 0; x < MAP_WIDTH; ++x)
    {
        for (size_t y = 0; y < MAP_HEIGHT; ++y)
        {
            // Превышен диапазон правильных значений ячейки.
            if (map[x][y] > U_GOLD_COIN)
            {
                crash("scene_game, map_check_correctness(), map[%Iu][%Iu] == %Iu (>U_GOLD_COUN)",
                      x,
                      y,
                      map[x][y],
                      U_GOLD_COIN);
            }
            // Регистрируем все старты.
            if (map[x][y] == U_START)
            {
                ++starts_count;
            }
            // Регистрируем все финиши.
            if (map[x][y] == U_FINISH)
            {
                ++finishes_count;
            }
        }
    }
    // На карте должен быть ровно один старт.
    if (starts_count != 1)
    {
        crash("scene_game, map_check_correctness(), на карте должен быть ровно один старт.\nТекущее количество: %Iu",
              starts_count);
    }
    // На карте должен быть хотя бы один выход.
    if (finishes_count == 0)
    {
        crash("scene_game, map_ckeck_correctness(), на карте должен быть хотя бы один финишь.");
    }
}

// Строит уровень.
// В критической случае ошибки показывает информацию о причине сбоя и крашит программу.
static void level_build(void)
{
    for (size_t x = 0; x < MAP_WIDTH; ++x)
    {
        for (size_t y = 0; y < MAP_HEIGHT; ++y)
        {
            switch (map[x][y])
            {
                // В ячейке со значением U_GHOST появляется призрак.
                // Ячейка принимает состояние U_EMPTY.
                case (U_GHOST):
                {
                    ghost_add(x, y);
                    map[x][y] = U_EMPTY;
                    break;
                }
                // В ячейке со значением U_BAT появляется мышь.
                // Ячейка принимает состояние U_EMPTY.
                case (U_BAT):
                {
                    bat_add(x, y);
                    map[x][y] = U_EMPTY;
                    break;
                }
                // В ячейке со значением U_START появляется игрок.
                case (U_START):
                {
                    player_add(x, y);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    // Дать ли каждой монетке собственную анимацию, или все они будут крутиться синхронно?
}

// Добавляет призрака.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void ghost_add(const float _x, const float _y)
{
    if (enemies_count == MAX_ENEMIES)
    {
        crash("scene_game, ghost_add(), enemies_count == MAX_ENEMIES");
    }

    // Активируем врага.
    enemies[enemies_count].active = 1;

    // Размещаем.
    enemies[enemies_count].x = _x;
    enemies[enemies_count].y = _y;

    // Задаем скорость движения.
    enemies[enemies_count].vx = GHOST_SPEED;
    enemies[enemies_count].vy = 0.f;

    // Инициализируем анимацию.
    animation_ghost(&enemies[enemies_count].a);

    // Устанавливаем врагу соответствующую текстуру.
    enemies[enemies_count].texture = texture_ghost;

    ++enemies_count;
}
// Добавляет мышь.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void bat_add(const float _x, const float _y)
{
    if (enemies_count == MAX_ENEMIES)
    {
        crash("scene_game, bat_add(), enemies_count == MAX_ENEMIES");
    }

    // Активируем врага.
    enemies[enemies_count].active = 1;

    // Размещаем.
    enemies[enemies_count].x = _x;
    enemies[enemies_count].y = _y;

    // Задаем скорость движения.
    enemies[enemies_count].vx = 0.f;
    enemies[enemies_count].vy = BAT_SPEED;

    // Инициализируем анимацию.
    animation_bat(&enemies[enemies_count].a);

    // Устанавливаем врагу соответствующую текстуру.
    enemies[enemies_count].texture = texture_bat;

    ++enemies_count;
}

// Добавляет игрока.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void player_add(const float _x,
                       const float _y)
{
    // Размещаем.
    player.x = _x;
    player.y = _y;

    // Задаем скорость движения.
    player.vx = 0.f;
    player.vy = 0.f;

    // Инициализируем анимацию.
    animation_player_walk(&player.a);
}

// Сбрасывает анимацию серебрянных монеток.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void silver_animation_reset(void)
{
    silver_animation.t = 0.f;
    silver_animation.first_frame = 0;
    silver_animation.current_frame = 0;
    silver_animation.last_frame = SILVER_COIN_FRAMES_COUNT - 1;
}
