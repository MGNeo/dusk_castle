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
// Монетка.
#define U_COIN            4
// Стена.
#define U_WALL            5
// Лестница.
#define U_LADDER          6
// Шипы.
#define U_DAMAGE          7
// Токсичная слизь.
#define U_TOXIC           8
// Призрак.
#define U_GHOST           9
// Летучая мышь.
#define U_BAT            10
// Жизнь.
#define U_LIFE           11

#endif
