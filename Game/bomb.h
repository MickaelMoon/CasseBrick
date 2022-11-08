#include "struct.h"

#ifndef BOMB_H
#define BOMB_H

Bomb * initBomb(Player * player);
int poseBomb(Map * map, Player * player);
void removeBomb(Bomb *bomb, Map * map);
void explosion(Bomb * bomb, Map *map);

#endif