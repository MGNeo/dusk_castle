/*
 * Модуль текста.
 * Предоставляет информацию о максимальной длине выводимого текста, возможное выравнивание и
 * функцию вывода текста.
 *
 */

#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>

// Максимально допустимая длина текста в символах.
#define MAX_TEXT_LENGTH 1024

typedef enum e_text_align
{
    TEXT_ALIGN_LEFT,// TA_LEFT каким-то образом переопределяло определение из wingdi.h
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER
} text_align;

extern void text_draw(const char *const _text,
                      const size_t _size,
                      const int _x,
                      const int _y,
                      const text_align _align);

#endif
