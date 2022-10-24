#include "struct.h"

#ifndef MAP_H
#define MAP_H

void afficherMap(Map * map);
char **initialiseTab(int rows, int columns);
Player * initPlayer(int i);
void recupData(char *filepath, Map * map);
Map * initMap(char * filePath);
void updateTimerBomb(Map * map, Player * player);

#endif