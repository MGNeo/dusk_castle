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
#define PLAYER_DIE_FIRST 12
#define PLAYER_DIE_LAST  15
// Всего кадров.
#define PLAYER_FRAMES_COUNT (PLAYER_DIE_LAST + 1)

// Токсичная слизь.
#define TOXIC_FRAMES_COUNT 10

// Призрак.
#define GHOST_FRAMES_COUNT 12

// Летучая мышь.
#define BAT_FRAMES_COUNT 10

// Серебряная монетка.
#define SILVER_COIN_FRAMES_COUNT 12

// Золотая монетка.
#define GOLD_COIN_FRAMES_COUNT 12

#endif
