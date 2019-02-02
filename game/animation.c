#include "animation.h"
#include "crash.h"

// Обработка анимации.
// В случае критической ошибки показывает информацию о причине сбоя и крашит программу.
void animation_processing(animation *const _animation,
                          const float _dt)
{
    if (_animation == NULL)
    {
        crash("animation(), _animation == NULL");
    }
    _animation->t += _dt * ANIMATIONS_FPS;
    if (_animation->t > 1.f)
    {
        _animation->current_frame += _animation->t + 0.5f;
        _animation->t = 0.f;
        if (_animation->current_frame > _animation->last_frame)
        {
            _animation->current_frame -=
            (_animation->last_frame - _animation->first_frame) + 1;
        }
    }
}
