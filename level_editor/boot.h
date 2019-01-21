/*
 * Модуль начальной инициализации и загрузки.
 * Предоставляет для main.c функции инициализации всех систем и загрузки всех ресурсов.
 *
 */

#ifndef BOOT_H
#define BOOT_H

void system_init(void);

void glyphs_load(void);

void textures_load(void);

#endif
