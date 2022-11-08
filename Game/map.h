#include "struct.h"

#ifndef MAP_H
#define MAP_H

void afficherSerializedMap(char * map);
char * serializeMap(Map * map);
void afficherMap(Map * map);
char **initialiseTab(int rows, int columns);
int nbPlayerMaxPerMap(char * filepath);
Player * initPlayer(int i);
void recupData(char *filepath, Map * map);
void updateDangerMap(Map *map);
Map * initMap(char * filePath, int mode, int nbHumanPlayers);
void updateTimerBomb(Map * map, Player * player);

#endif