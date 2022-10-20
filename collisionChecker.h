#include "struct.h"

#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H

void pickItem(char item, Player * player);
int collisionChecker(Player * player, Map * map, char direction);
int tileChecker(int x, int y, Map * map, Player * player);

#endif