/*
 * Модуль оконного хэндла.
 * Предоставляет ресурс-хэндл окна.
 * Хэндл окна необходим для того, чтобы диалоговые окна правильно
 * блокировали родительское окно.
 *
 */
#ifndef HWND_H
#define HWND_H

#include <windows.h>

extern HANDLE hwnd;

extern void hwnd_create(void);

#endif
