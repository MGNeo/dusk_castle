#include "processing.h"
#include "crash.h"
#include "menu.h"
#include "player.h"
#include "animation.h"

// Сброс игрока.
static void player_reset(void);

// Обработка анимации.
static void animation_processing(animation_state *const _animation_state,
                                 const float _dt);

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

static void player_reset(void)
{
    //player.x = MAP_WIDTH / 2;
    //player.y = MAP_HEIGHT / 2;
}

// Обработка анимации.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
static void animation_processing(animation_state *const _state,
                                 const float _dt)
{
    if (_state == NULL)
    {
        crash("animation_state(), _animation_state == NULL");
    }
    _state->current_frame += _state->first_frame * _dt;
    if (_state->current_frame > _state->first_frame)
    {
        _state->current_frame = _state->first_frame;
        // А что если  size_t->float->size_t приведет к 3 -> 2.99997 -> 2?
    }
}
