/*
 * Модуль текста. Предоставляет информацию о максимальной длине выводимого текста и
 * возможное выравнивание.
 *
 */

#ifndef TEXT_H
#define TEXT_H

// Максимально допустимая длина текста в символах.
#define MAX_TEXT_LENGTH 256

typedef enum e_text_align
{
    TEXT_ALIGN_LEFT,// TA_LEFT каким-то образом переопределяло определение из wingdi.h
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER
} text_align;

#endif
