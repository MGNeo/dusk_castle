/*
 * Модуль юнитов.
 * Определяет количество юнитов и диапазоны их типов.
 *
 */

#ifndef UNITS_H
#define UNITS_H

#include <limits.h>

#define UNITS_COUNT 12

// Пустота.
#define U_EMPTY           0
// Граница движения врагов.
#define U_BORDER          1
// Старт.
#define U_START           2
// Финиш.
#define U_FINISH          3
// Стена.
#define U_WALL            4
// Лестница.
#define U_LADDER          5
// Колья.
#define U_STAKES          6
// Токсичная слизь.
#define U_TOXIC           7
// Призрак.
#define U_GHOST           8
// Летучая мышь.
#define U_BAT             9
// Серебрянная монетка.
#define U_SILVER_COIN    10
// Золотая монетка.
#define U_GOLD_COIN      11

#endif
