#include "struct.h"

#ifndef MAP_H
#define MAP_H

void afficherMap(Map * map);
char **initialiseTab(int rows, int columns);
Player * initPlayer(int i);
char ** recupData(char *filepath, Map * map);
Map * initMap(int nbPlayer);
void updateTimerBomb(Map * map, Player * player);

#endif