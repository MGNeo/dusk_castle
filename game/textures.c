#include "textures.h"
#include "frames.h"
#include "sprite.h"
#include "render.h"
#include "crash.h"

#include <string.h>

SDL_Texture *texture_player,
            *texture_start,
            *texture_finish,
            *texture_wall,
            *texture_ladder,
            *texture_stakes,
            *texture_toxic,
            *texture_ghost,
            *texture_bat,
            *texture_silver_coin,
            *texture_gold_coin;

// Загружает текстуру.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture,
                         const size_t _frames_count);

// Загружает текстуры.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void textures_load(void)
{
    // Защита от повторного вызова.
    static size_t again;
    if (again++ != 0)
    {
        crash("textures_load(), попытка повторной загрузки текстур.\n");
    }

    // Игрок.
    texture_load("textures/player.bmp", &texture_player, PLAYER_FRAMES_COUNT);

    // Старт.
    texture_load("textures/start.bmp", &texture_start, START_FRAMES_COUNT);

    // Финиш.
    texture_load("textures/finish.bmp", &texture_finish, FINISH_FRAMES_COUNT);

    // Стена.
    texture_load("textures/wall.bmp", &texture_wall, WALL_FRAMES_COUNT);

    // Лестница.
    texture_load("textures/ladder.bmp", &texture_ladder, LADDER_FRAMES_COUNT);

    // Шипы.
    texture_load("textures/stakes.bmp", &texture_stakes, STAKES_FRAMES_COUNT);

    // Токсичная слизь.
    texture_load("textures/toxic.bmp", &texture_toxic, TOXIC_FRAMES_COUNT);

    // Призрак.
    texture_load("textures/ghost.bmp", &texture_ghost, GHOST_FRAMES_COUNT);

    // Летучая мышь.
    texture_load("textures/bat.bmp", &texture_bat, BAT_FRAMES_COUNT);

    // Серебрянная монетка.
    texture_load("textures/silver coin.bmp", &texture_silver_coin, SILVER_COIN_FRAMES_COUNT);

    // Золотая монетка.
    texture_load("textures/gold coin.bmp", &texture_gold_coin, GOLD_COIN_FRAMES_COUNT);

}

// Загружает текстуру.
// В случае критической ошибки показывает ионформацию о причине сбоя и крашит программу.
static void texture_load(const char *const _file_name,
                         SDL_Texture **const _texture,
                         const size_t _frames_count)
{
    if (_file_name == NULL)
    {
        crash("texture_load(), _file_name == NULL");
    }

    if (strlen(_file_name) == 0)
    {
        crash("texture_load(), strlen(_file_name) == 0");
    }

    if (_texture == NULL)
    {
        crash("texture_load(), _texture == NULL");
    }

    if (_frames_count == 0)
    {
        crash("texture_load(), _frames_count == 0");
    }

    if (_frames_count > MAX_FRAMES_COUNT)
    {
        crash("texture_load(), _frames_count > MAX_FRAMES_COUNT");
    }

    // Пытаемся создать поверхность, загрузив данные из файла.
    SDL_Surface *h_surface = SDL_LoadBMP(_file_name);
    if (h_surface == NULL)
    {
        crash("texture_load(), не удалось создать поверхность на оснвое файла: %s\nSDL_GetError() : %s",
              _file_name,
              SDL_GetError());
    }

    // Проверяем высоту текстуры на соответствие требованиям.
    if (h_surface->h != SPRITE_SIZE)
    {
        crash("texture_load(), неверная высота текстуры.\nТекущая: %i\nТребуемая: %i",
              h_surface->h,
              SPRITE_SIZE);
    }
    // Вычисляем заданную ширину.
    // Целочисленное переполнение знакового возникнуть не может.
    const int required_w = SPRITE_SIZE * _frames_count;
    // Проверяем ширину текстуры на соответствие требованиям.
    if (h_surface->w != required_w)
    {
        crash("texture_load(), неверная ширина текстуры.\nТекущая: %i\nТребуемая: %i",
              h_surface->w,
              required_w);
    }

    // Пытаемся создать текстуру на основе поверхности.
    SDL_Texture *h_texture = SDL_CreateTextureFromSurface(render, h_surface);
    if (h_surface == NULL)
    {
        crash("texture_load(), не удалось создать текстуру на основе поверхности.\nSDL_GetError() : %s",
              SDL_GetError());
    }

    SDL_FreeSurface(h_surface);

    *_texture = h_texture;
}
