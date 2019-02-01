#include "processing.h"
#include "crash.h"
#include "menu.h"
#include "player.h"
#include "animation.h"
#include "frames.h"
#include "level.h"
#include "map.h"

#include <stdlib.h>
#include <stdio.h>
// Сброс игрока.
static void player_reset(void);

// Сброс уровня.
static void level_reset(void);

// Обработка анимации.
static void animation_processing(animation_state *const _animation_state,
                                 const float _dt);

// Загрузка карты.
static void map_load(void);

// Загрузка карты.
// В случае критической ошибки показывает информацию о причине ошибки и крашит программу.
static void map_load(void)
{
    if (level == LEVELS_COUNT)
    {
        return;
    }

    // Формируем имя уровня, который нужно загрузить.
    char file_name[] = "map/level0123456789";
    sprintf(file_name, "map/level%10Iu", level);

    // Попытаемся открыть файл.
    FILE *f = fopen(file_name, "rb");

    //if ()
}

// Сброс уровня.
static void level_reset(void)
{
    level = 0;
}

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
                player_reset();
                return 1;
            } else {
                return -1;
            }
        }
    }
}

static void player_reset(void)
{
    player.anim.current_frame = PLAYER_CLIMB_FIRST;
    player.anim.first_frame = PLAYER_CLIMB_FIRST;
    player.anim.last_frame = PLAYER_CLIMB_LAST;//PLAYER_FRAMES_COUNT - 1;
    player.anim.t = 0.f;
}

// Обработка всех анимаций.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void animations_processing(const float _dt)
{
    animation_processing(&player.anim, _dt);
}

// Обработка анимации.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void animation_processing(animation_state *const _animation_state,
                                 const float _dt)
{
    if (_animation_state == NULL)
    {
        crash("animation_state(), _animation_state == NULL");
    }
    _animation_state->t += _dt * ANIMATIONS_FPS;
    if (_animation_state->t > 1.f)
    {
        _animation_state->current_frame += _animation_state->t + 0.5f;
        _animation_state->t = 0.f;
        if (_animation_state->current_frame > _animation_state->last_frame)
        {
            _animation_state->current_frame -=
            (_animation_state->last_frame - _animation_state->first_frame) + 1;
        }
    }
}
