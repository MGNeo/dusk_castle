/*
 * Модуль обработки.
 * Предоставляет для editor.c функции обработки.
 *
 *
 */

#ifndef PROCESSING_H
#define PROCESSING_H

#include "SDL.h"

int menu_1_processing(const SDL_Event *const _event);

void cursor_processing(const SDL_Event *const _event);

void render_point_processing(void);

void menu_2_processing(const SDL_Event *const _event);

void f8_processing(const SDL_Event *const _event);

void escape_processing(const SDL_Event *const _event);

#endif
