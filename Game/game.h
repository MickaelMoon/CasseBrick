#include "struct.h"

#ifndef GAME_H
#define GAME_H
#define SOLO 0
#define MULTI 1

Game * initGame(int nbHumanPlayers, char * mapProvided);
void launchGame(int nbHumanPlayers, char * mapProvided);

#endif