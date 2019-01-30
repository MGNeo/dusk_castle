/*
 * Модуль отрисовки.
 * Предоставляет для editor.c функции отрисовки курсора, карты и пр.
 *
 */

#ifndef DRAW_H
#define DRAW_H

#include "text.h"
#include <stdlib.h>

void menu_1_draw(void);

void grid_draw(void);

void cursor_draw(void);

void map_draw(void);

void info_draw(void);

void menu_2_draw(void);

#endif
