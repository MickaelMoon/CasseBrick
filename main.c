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

   char action;

   //system("clear");
   int turn = 0;
   afficherMap(map);
    Player * currentPlayer;

   do{
       if (map->playerList[turn%nbPlayer]->status == isDead){
        turn++;
       } else {
        currentPlayer = map->playerList[turn%nbPlayer];
        printf("nb Bomb on field: %d\n", map->nbBombsOnMap);

       printf("Do something, Player %c:\n",currentPlayer->token);
       scanf(" %c",&action);
       keyHandler(action, map, currentPlayer);
       updateTimerBomb(map, currentPlayer);
       //system("clear");
       afficherMap(map);
       turn++;
       }
   } while (map->nbPlayerAlive > 1);

    return 0;
}



