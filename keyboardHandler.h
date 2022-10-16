#include "struct.h"

#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H


Bomb * initBomb(Player * player);
void poseBomb(Map * map, Player * player);
void keyHandler(char c, Map * map, Player * player);
void move (char c, Map * map, Player * player);

#endif