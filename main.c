#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>

#include "struct.h"
#include "keyboardHandler.h"
//#include "bomb.h"
#include "map.h"



int main() {
    int nbPlayer;

    //recupData("map2.txt");
   do{
       printf("How Many players will play ? (2-4)\n");
       scanf("%d", &nbPlayer);
   } while (nbPlayer != 2 && nbPlayer != 3 && nbPlayer != 4);

   Map * map = initMap(nbPlayer);

   int play = 0;
   char action;

   //system("clear");
   afficherMap(map);
   do{
       printf("nb Bomb on field: %d\n", map->nbBombsOnMap);

       printf("Do something, Player %c:\n",map->playerList[play%nbPlayer]->token);
       scanf(" %c",&action);
       keyHandler(action, map, map->playerList[play%nbPlayer]);
       updateTimerBomb(map, map->playerList[play%nbPlayer]);
       //system("clear");
       afficherMap(map);
       play++;
   } while (play < 200);

    return 0;
}



