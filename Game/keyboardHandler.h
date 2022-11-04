#include "struct.h"

#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H


Bomb * initBomb(Player * player);
int poseBomb(Map * map, Player * player);
int keyHandler(char c, Map * map, Player * player);
void kickBomb (char c, Map * map, Player * player);
void move (char c, Map * map, Player * player);

#endif