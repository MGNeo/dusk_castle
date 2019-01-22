/*
 * Модуль отрисовки.
 * Предоставляет функции отрисовки текста, текстур, линий и пр.
 *
 */

#ifndef DRAW_H
#define DRAW_H

#include "text.h"
#include <stdlib.h>

void text_draw(const char *const _text,
               const size_t _size,
               const int _x,
               const int _y,
               const text_align _align);

void menu_1_draw(void);

void grid_draw(void);

void cursor_draw(void);

void map_draw(void);

void info_draw(void);

#endif
