#include "struct.h"

#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H


int collisionChecker(Player * player, Map * map, char direction);
int tileChecker(int x, int y, Map * map);

#endif