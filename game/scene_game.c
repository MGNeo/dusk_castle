#include "scene_game.h"
#include "map.h"
#include "crash.h"
#include "units.h"
#include "enemy.h"
#include "frames.h"
#include "textures.h"
#include "player.h"
#include "animation.h"
#include "fps.h"
#include "dt.h"
#include "render.h"
#include "window.h"
#include "sprite.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define MAX_LEVEL 99

// Номер уровня.
static uint8_t level;
// Количество серебра на счету игрока.
static size_t silver_score;
// Количество золота на счету игрока.
static size_t gold_score;

// Карта уровня.
static uint8_t map[MAP_WIDTH][MAP_HEIGHT];
// Количество врагов.
static size_t enemies_count;
// Враги.
static enemy_unit enemies[MAX_ENEMIES];
// Игрок.
static player_unit player;

// Анимация серебрянной монетки.
static animation animation_silver_coin;
// Анимация золотой монетки.
static animation animation_gold_coin;
// Анимация токсичной жижи.
static animation animation_toxic;

// Загружает карту из файла.
static void map_load_from_file(void);
// Проверяет загруженную карту на корректность.
static void map_check_correctness(void);
// Строит мир.
static void world_build(void);

// Добавляет призрака.
static void ghost_add(const float _x, const float _y);
// Добавляет мышь.
static void bat_add(const float _x, const float _y);
// Добавляет игрока.
static void player_add(const float _x,
                       const float _y);

// Обработка игрока (вся).
static size_t player_processing(const float _dt);
// Контроль игрока.
static size_t player_control(void);
// Анимации игрока.
static void player_animation(const float _dt);
// Движение игрока.
static void player_move(const float _dt);
// Отрисовка игрока.
static void player_draw(void);

// Обработка карты (вся).
static void map_processing(const float _dt);
// Анимации карты.
static void map_animation(const float _dt);
// Отрисовка карты.
static void map_draw(void);

// Обработка врагов (вся).
static void enemies_processing(const float _dt);
// Анимация врага.
static void enemy_animation(enemy_unit *const _enemy, const float _dt);
// Движение врага.
static void enemy_move(enemy_unit *const _enemy, const float _dt);
// Отрисовка врага.
static void enemy_draw(enemy_unit *const _enemy);

// Сцена-игра.
// Обрабатывает _param.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
extern scene_return_value scene_game(const size_t _param)
{
    // Обрабатываем управляющую команду _param.
    switch (_param)
    {
        // Начало новой игры.
        case (SCENE_GAME_PARAM_NEW_GAME):
        {
            level = 0;
            silver_score = 0;
            gold_score = 0;
            break;
        }
        // Начало следующего уровня.
        case (SCENE_GAME_PARAM_NEXT_LEVEL):
        {
            ++level;
            break;
        }
        // Начало того же уровня.
        case (SCENE_GAME_PARAM_CURRENT_LEVEL):
        {
            // Золото уменьшается в момент смерти игрока,
            // перед переходом к следующей сцене.
            break;
        }
        default:
        {
            crash("scene_game, scene_game(), неизвестное значение _param.");
        }
    }

    // Грузим карту из файла.
    map_load_from_file();

    // Проверяем корректность загруженной карты.
    map_check_correctness();

    // Строим уровень.
    world_build();

    // Устанавливаем точку отсчета времени.
    dt_reset();

    while (1)
    {
        // Определяем, пришло ли время обрабатывать очередной кадр.
        const float dt = dt_get();
        if (dt > SPF)
        {
            // DEBUG
            SDL_RenderClear(render);

            // Обрабатываем карту.
            map_processing(dt);

            // Обрабатываем игрока.
            player_processing(dt);

            // Обрабатываем врагов.
            enemies_processing(dt);

            // Устанавливаем точку отсчета времени.
            dt_reset();

            // DEBUG
            SDL_RenderPresent(render);
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

// Строит мир.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void world_build(void)
{
    enemies_count = 0;

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

    // Инициализируем анимацию серебряных монеток.
    animation_silver_coin_init(&animation_silver_coin);

    // Инициализируем анимацию золотых монеток.
    animation_gold_coin_init(&animation_gold_coin);

    // Инициализируем анимацию токсичной жижи.
    animation_toxic_init(&animation_toxic);
}

// Добавляет призрака.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void ghost_add(const float _x, const float _y)
{
    if (enemies_count == MAX_ENEMIES)
    {
        crash("scene_game, ghost_add(), enemies_count == MAX_ENEMIES");
    }

    // Размещаем.
    enemies[enemies_count].x = _x;
    enemies[enemies_count].y = _y;

    // Задаем скорость движения.
    enemies[enemies_count].vx = GHOST_SPEED;
    enemies[enemies_count].vy = 0.f;

    // Инициализируем анимацию.
    animation_ghost_init(&enemies[enemies_count].a);

    // Инициализируем текстуру.
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

    // Размещаем.
    enemies[enemies_count].x = _x;
    enemies[enemies_count].y = _y;

    // Задаем скорость движения.
    enemies[enemies_count].vx = 0.f;
    enemies[enemies_count].vy = BAT_SPEED;

    // Инициализируем анимацию.
    animation_bat_init(&enemies[enemies_count].a);

    // Инициализируем текстуру.
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
    animation_player_walk_init(&player.a);
}

// Просчет карты.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void map_processing(const float _dt)
{
    // Анимация карты.
    map_animation(_dt);
    // Отрисовка карты.
    map_draw();
}

// Просчет анимации карты.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void map_animation(const float _dt)
{
    animation_processing(&animation_toxic, _dt);
    animation_processing(&animation_silver_coin, _dt);
    animation_processing(&animation_gold_coin, _dt);
}

// Отрисовка карты.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void map_draw(void)
{
    // Определяем размеры клиентской области окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Определяем, сколько клеток карты умещается по ширине и высоте клиентской области.
    const int x_count = w / SPRITE_SIZE + 1;
    const int y_count = h / SPRITE_SIZE + 1;

    // Определяем, сколько клеток умещается между игроком и краями экрана по ширине и высоте.
    const int half_x_count = x_count / 2;
    const int half_y_count = y_count / 2;

    // Определяем x-индекс самой левой отрисовываемой клетки.
    int begin_x = (player.x + 0.5f) - half_x_count;
    if (begin_x < 0)
    {
        begin_x = 0;
    }

    // Определяем x-индекс самой правой отрисовываемой клетки.
    int end_x = (player.x + 0.5f) + half_x_count;
    if (end_x >= MAP_WIDTH)
    {
        end_x = MAP_WIDTH - 1;
    }

    // Определяем y-индекс самой верхней отрисовываемой клетки.
    int begin_y = (player.y + 0.5f) - half_y_count;
    if (begin_y < 0)
    {
        begin_y = 0;
    }

    // Определяем y-индекс самой нижней отрисовываемой клетки.
    int end_y = (player.y + 0.5f) + half_y_count;
    if (end_y >= MAP_HEIGHT)
    {
        end_y = MAP_HEIGHT - 1;
    }

    // Контроль переполнения знакового целого излишен.

    // Отрисовываем.
    for (int x = begin_x; x <= end_x; ++x)
    {
        for (int y = begin_y; y <= end_y; ++y)
        {
            // Текстура, которую будем отрисовывать.
            SDL_Texture *h_texture = NULL;

            // Отрисовываемая область текстуры.
            SDL_Rect source_rect;
            source_rect.w = SPRITE_SIZE;
            source_rect.h = SPRITE_SIZE;
            source_rect.x = 0;
            source_rect.y = 0;

            // Область вывода на поверхности клиентской части окна.
            SDL_Rect destination_rect;
            destination_rect.w = SPRITE_SIZE;
            destination_rect.h = SPRITE_SIZE;
            destination_rect.x = (x - player.x) * SPRITE_SIZE + w / 2.f - SPRITE_SIZE / 2.f + 0.5f;
            destination_rect.y = (y - player.y) * SPRITE_SIZE + h / 2.f - SPRITE_SIZE / 2.f + 0.5f;

            // Выбираем текстуру и область на ней в соответствии с типом клетки.
            switch (map[x][y])
            {
                case (U_START):
                {
                    h_texture = texture_start;
                    break;
                }
                case (U_FINISH):
                {
                    h_texture = texture_finish;
                    break;
                }
                case (U_WALL):
                {
                    h_texture = texture_wall;
                    break;
                }
                case (U_LADDER):
                {
                    h_texture = texture_ladder;
                    break;
                }
                case (U_STAKES):
                {
                    h_texture = texture_stakes;
                    break;
                }
                case (U_TOXIC):
                {
                    h_texture = texture_toxic;
                    source_rect.x = animation_toxic.current_frame * SPRITE_SIZE;
                    break;
                }
                case (U_SILVER_COIN):
                {
                    h_texture = texture_silver_coin;
                    source_rect.x = animation_silver_coin.current_frame * SPRITE_SIZE;
                    break;
                }
                case (U_GOLD_COIN):
                {
                    h_texture = texture_gold_coin;
                    source_rect.x = animation_gold_coin.current_frame * SPRITE_SIZE;
                    break;
                }
            }

            // Если данная клетка не имеет графического представления, переходим к следующей.
            if (h_texture == NULL) continue;

            // Рисуем.
            if (SDL_RenderCopy(render, h_texture, &source_rect, &destination_rect) != 0)
            {
                crash("scene_game.c, map_draw(), не удалось отрисовать клетку карты.\nSDL_GetError() : %s",
                      SDL_GetError());
            }

        }
    }
}

// Обработка игрока.
static size_t player_processing(const float _dt)
{
    // Контроль игрока.
    player_control();
    // Анимация игрока.
    player_animation(_dt);
    // Движение игрока.
    player_move(_dt);
    // Отрисовка игрока.
    player_draw();

    return 0;
}

// Контроль игрока.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static size_t player_control(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
    }
}

// Просчет анимации игрока.
// В случае критической ошибки показывает инфорамцию о причине сбоя и крашит программу.
static void player_animation(const float _dt)
{
    animation_processing(&player.a, _dt);
}

// Движение игрока.
// В случае критической ошибки показывает информацию о причине ошибки и крашит программу.
static void player_move(const float _dt)
{
    // ...
}

// Отрисовка игрока.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void player_draw(void)
{
    // Определяем размеры клиентской области окна.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Определяем точку вывода.
    const int x = w / 2 - SPRITE_SIZE / 2 + 0.5f;
    const int y = h / 2 - SPRITE_SIZE / 2 + 0.5f;

    // Отрисовываемая область текстуры.
    SDL_Rect source_rect;
    source_rect.w = SPRITE_SIZE;
    source_rect.h = SPRITE_SIZE;
    source_rect.x = player.a.current_frame * SPRITE_SIZE;
    source_rect.y = 0;

    // Область вывода на поверхности клиентской части окна.
    SDL_Rect destination_rect;
    destination_rect.w = SPRITE_SIZE;
    destination_rect.h = SPRITE_SIZE;
    destination_rect.x = x;
    destination_rect.y = y;

    // Отрисовываем игрока.
    if (SDL_RenderCopy(render, texture_player, &source_rect, &destination_rect) != 0)
    {
        crash("scene_game.c, player_draw(), не удалось отрисовать игрока, SDL_GetError() : %s",
              SDL_GetError());
    }

}

// Обработка врагов (вся).
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void enemies_processing(const float _dt)
{
    for (size_t e = 0; e < enemies_count; ++e)
    {
        // Анимируем врага.
        enemy_animation(&enemies[e], _dt);
        // Движение врага.
        enemy_move(&enemies[e], _dt);
        // Отрисовка врага.
        enemy_draw(&enemies[e]);
    }
}

// Анимация врага.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void enemy_animation(enemy_unit *const _enemy, const float _dt)
{
    if (_enemy == NULL)
    {
        crash("scene_game.c, enemy_animation(), _enemy == NULL");
    }
    animation_processing(&_enemy->a, _dt);
}
// Движение врага.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void enemy_move(enemy_unit *const _enemy, const float _dt)
{
    if (_enemy == NULL)
    {
        crash("scene_game.c, enemy_move(), _enemy == NULL");
    }

    /* Потенциально, излишне.
    if ( (_enemy->x < 0) || (_enemy->x >= MAP_WIDTH) || (_enemy->y < 0) || (_enemy->y >= MAP_HEIGHT) )
    {
    }
    */

    // Враг движется по x (призрак).
    if (_enemy->vx != 0.f)
    {
        const float f_nx = _enemy->x + _enemy->vx * _dt;
        const int i_ny = _enemy->y;
        // Если призрак движется вправо.
        if (_enemy->vx > 0.f)
        {
            const int i_nx = f_nx + 1;
            // Если призрак напоролся справа на правый край экрана или на не пустоту.
            if ( (i_nx >= MAP_WIDTH) || (map[i_nx][i_ny] != U_EMPTY) )
            {
                _enemy->vx = -_enemy->vx;
                return;
            }
        } else {
            // Если празрак движется влево.
            const int i_nx = f_nx;
            // Если призрак напоролся слева на левый край экрана или на не пустоту.
            if ( (i_nx < 0) || (map[i_nx][i_ny] != U_EMPTY) )
            {
                _enemy->vx = -_enemy->vx;
                return;
            }
        }
        _enemy->x = f_nx;
        return;
    }

    // Враг движется по y (летучая мышь).
    if (_enemy->vy != 0.f)
    {
        const float f_ny = _enemy->y + _enemy->vy * _dt;
        const int i_nx = _enemy->x;
        // Если летучая мышь движется вниз.
        if (_enemy->vy > 0.f)
        {
            const int i_ny = f_ny + 1;
            // Если летучая мышь напоролась снизу на нижний край экрана или на не пустоту.
            if ( (i_ny >= MAP_HEIGHT) || (map[i_nx][i_ny] != U_EMPTY) )
            {
                _enemy->vy = -_enemy->vy;
                return;
            }
        } else {
            // Если летучая мышь движется вверх.
            const int i_ny = f_ny;
            // Если летучая мышь напоролась снизу на нижний край экрана или на не пустоту.
            if ( (i_ny < 0) || (map[i_nx][i_ny] != U_EMPTY) )
            {
                _enemy->vy = -_enemy->vy;
                return;
            }
        }
        _enemy->y = f_ny;
        return;
    }
}
// Отрисовка врага.
// В случае критической ошибки показывает инфорацию о причине сбоя и крашит программу.
static void enemy_draw(enemy_unit *const _enemy)
{
    if (_enemy == NULL)
    {
        crash("scene_game.c, enemy_draw(), _enemy == NULL");
    }

    // Определяем размеры клиентской области экрана.
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Определяем точку вывода.
    const int x = (_enemy->x - player.x) * SPRITE_SIZE + w / 2 - SPRITE_SIZE / 2 + 0.5f;
    const int y = (_enemy->y - player.y) * SPRITE_SIZE + h / 2 - SPRITE_SIZE / 2 + 0.5f;

    // Рисуем только в том случае, если видно хотя бы кусочек врага.
    // Можно проверять быстрее..............................................................
    if ( (x < SPRITE_SIZE) || (x > w) || (y < SPRITE_SIZE) || (y > h) )
    {
        return;
    }

    // Отрисовываемая область текстуры.
    SDL_Rect source_rect;
    source_rect.w = SPRITE_SIZE;
    source_rect.h = SPRITE_SIZE;
    source_rect.x = _enemy->a.current_frame * SPRITE_SIZE;
    source_rect.y = 0;

    // Область вывода на поверхности клиентской части окна.
    SDL_Rect destination_rect;
    destination_rect.w = SPRITE_SIZE;
    destination_rect.h = SPRITE_SIZE;
    destination_rect.x = x;
    destination_rect.y = y;

    if (SDL_RenderCopy(render, _enemy->texture, &source_rect, &destination_rect) != 0)
    {
        crash("scene_game.c, enemy_draw(), не удалось отрисовать врага.\nSDL_GetError() : %s",
              SDL_GetError());
    }
}


// Тщательнее разобраться со всеми этими +0.5f и .f смещениями.
