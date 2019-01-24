/*
 * Модуль юнитов.
 * Определяет количество юнитов и диапазоны их типов.
 *
 */

#ifndef UNITS_H
#define UNITS_H

#include <limits.h>

#define UNITS_COUNT 256

// Пустота.
#define U_EMPTY           0
// Монетка.
#define U_COIN            1
// Точка входа и выхода.
#define U_RESPAWN         2
// Стены.
#define U_WALL_START      3
#define U_WALL_END        100
// Декорации.
#define U_DECOR_START     101
#define U_DECOR_END       200
// Лестницы.
#define U_LADDER_START    201
#define U_LADDER_END      211
// Смертельные вещи.
#define U_FATAL_START     212
#define U_FATAL_END       250
// Место обитания призрака.
#define U_GHOST_START     251
#define U_GHOST_END       255

#endif
