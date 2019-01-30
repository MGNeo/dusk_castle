/*
 * Модуль кадров.
 * Предоставляет информацию о диапазонах и количествах кадров.
 *
 */

#ifndef FRAMES_H
#define FRAMES_H

#define MAX_FRAMES_COUNT 100

// Игрок.

// Ходьба.
#define PLAYER_WALK_FIRST  0
#define PLAYER_WALK_LAST   3
// Лазанье.
#define PLAYER_CLIMB_FIRST 4
#define PLAYER_CLIMB_LAST  7
// Падение.
#define PLAYER_FALL_FIRST 8
#define PLAYER_FALL_LAST 11
// Смерть.
#define PLAYER_DEAD_FIRST 12
#define PLAYER_DEAD_LAST  15
// Всего кадров.
#define PLAYER_FRAMES_COUNT (PLAYER_DEAD_LAST + 1)

// Старт.
#define START_FRAMES_COUNT 1

// Финиш.
#define FINISH_FRAMES_COUNT 1

// Стена.
#define WALL_FRAMES_COUNT 1

// Лестница.
#define LADDER_FRAMES_COUNT 1

// Шипы.
#define STAKES_FRAMES_COUNT 1

// Токсичная слизь.
#define TOXIC_FRAMES_COUNT 10

//Призрак.
#define GHOST_FRAMES_COUNT 5

// Летучая мышь.
#define BAT_FRAMES_COUNT 5

// Серебрянная монетка.
#define SILVER_COIN_FRAMES_COUNT 10

// Золотая монетка.
#define GOLD_COIN_FRAMES_COUNT 10

#endif
