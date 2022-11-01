#include <wchar.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include "struct.h"
#include "keyboardHandler.h"
//#include "bomb.h"
#include "map.h"
#include "../Server/server.h"
#include "../Client/client.h"

int mapIsInTab(char ** tab, int lenTab, char * map){
    for (int i = 0; i < lenTab; i++){
        if (tab[i] == map){
            return 1;
        }
    }
    return 0;
}

Game * initGame(int mode){
    Game * game = malloc(sizeof(Game));
    game->status = mode;

    char ** mapChoosen = malloc(sizeof(char*)*6); //let say 6 maps max to be choosen
    for (int i = 0; i < 6; i++){
        mapChoosen[i] = malloc(sizeof(char)*20); //map filePath should not be higher than 20 characters
    }
    int nbOfMapChoosen = 0;
    int mapChoice = -1;

    const char *map2Players[3] = {
        [0] = "1: Classic | ",
        [1] = "2: CheeseMap | ",
        [2] = "3: Duel | "
    };
    const char *map4Players[4] = {
        [0] = "4: FullDestruction | ",
        [1] = "5: CrossingBlades | ",
        [2] = "6: SkullIsland | ",
        [3] = "7: OverPowered | "
    };
    int *pendingMaps = malloc(sizeof(int)*6);
    int nb2playerMaps = 0;
    int nb4playerMaps = 0;


do {
   system("clear");
   printf("Which map would you like in this game pool?\n");
   printf("Choices are:\n");
   if (nb4playerMaps == 0){
        printf("2 players maps available:\n");
        for (int i = 0; i < 3; i++){
            int j = 0;
            int alreadyChosen = 0;
            while (j < nb2playerMaps){
                if (map2Players[i] == pendingMaps[j]){
                    alreadyChosen = 1;
                }
                j++;
            }
            if (!alreadyChosen){
                printf("%s",map2Players[i]);
            }
        }
    printf("\n");
   }
   if (nb2playerMaps == 0){
        printf("4 players maps:\n");
        for (int i = 0; i < 4; i++){
            int j = 0;
            int alreadyChosen = 0;
            while (j < nb4playerMaps){
                if (map4Players[i] == pendingMaps[j]){
                    alreadyChosen = 1;
                }
                j++;
            }
            if (!alreadyChosen)
                printf("%s",map4Players[i]);
        }
    printf("\n");
   }
   printf("write the number you wish to add to the pool. Write it again to remove it from the pool.\nWrite 0 to end the selection.\n");
   printf("current maps choosen:\n");
   if (nbOfMapChoosen == 0){
    printf("None\n");
   } else {
        for (int i = 0; i < nbOfMapChoosen; i++){
            printf("%d - ",pendingMaps[i]);
        }
        printf("\n");
   }

    do{
        int c; 
  
        do { 
            c = getchar(); 
        } while (c != '\n' && c != EOF); 
        mapChoice = -1;
    }while (scanf("%d",&mapChoice) != 1 && (mapChoice > 7 || mapChoice < 0));

    if (mapChoice == 0){
        break;
    }
   
    int alreadySelected = 0;
    for (int i = 0; i < nbOfMapChoosen; i++){
        if (pendingMaps[i] == mapChoice){
            alreadySelected = 1;
            break;
        }
    }
    if (alreadySelected == 0){
        pendingMaps[nbOfMapChoosen] = mapChoice;
        nbOfMapChoosen++;
        if (mapChoice == 1 || mapChoice == 2 || mapChoice == 3){
            nb2playerMaps++;
        } else {
            nb4playerMaps++;
        }
    } else {
        for (int i = 0; i < nbOfMapChoosen-1; i++){
            while(pendingMaps[i] != mapChoice){
                i++;
            }
            while( i < nbOfMapChoosen-1){
                pendingMaps[i] = pendingMaps[i+1];
                i++;
            }
        }
        pendingMaps[nbOfMapChoosen-1] = -1; //remove the map selected
        nbOfMapChoosen--;
        if (mapChoice == 1 || mapChoice == 2 || mapChoice == 3){
            nb2playerMaps--;
        } else {
            nb4playerMaps--;
        }
    }


}while (nbOfMapChoosen < 6 && mapChoice != 0);

   char * filePath;
   switch(mapChoice){
    case 1:
        filePath = "Map/map.txt";
        break;
    case 2:
        filePath = "Map/map2.txt";
        break;
   }

}

void launchGame(int mode){
    /*int nbPlayer;

   do{
       printf("How Many players will play ? (2-4)\n");
       scanf("%d", &nbPlayer);
   } while (nbPlayer != 2 && nbPlayer != 3 && nbPlayer != 4);*/

    initGame(mode);

    //boucle while Do you want to play next map ? Yes all player/ or quit/wait other player

    char * filePath = "./Map/map3.txt"; // Temp

   Map * map = initMap(filePath);

   //1st map - launchMap(Map * map);

   char action;

   system("clear");
   int turn = 0;
   afficherMap(map);
    Player * currentPlayer;

   do{
       if (map->playerList[turn%map->nbPlayers]->status == isDead){
        turn++;
       } /* elsif player->isBot == true
            botTurn() --Algo IA
            turn++ */
       else {
        currentPlayer = map->playerList[turn%map->nbPlayers];
        //printf("nb Bomb on field: %d\n", map->nbBombsOnMap);
        printf("\nPlayer %c status:\nMax bomb: %d / Fire Pwr: %d\nPasseBomb: %d / BombKick: %d\nInvincibility: %d turn(s)\nHeart Shield: %d / Life(s): %d\n", currentPlayer->token, currentPlayer->bombMax, currentPlayer->firePower, currentPlayer->passBombs, currentPlayer->bombKick, currentPlayer->invincibilityTime, currentPlayer->heart, currentPlayer->life);

       printf("\nDo something, Player %c:\n",currentPlayer->token);
       scanf(" %c",&action);
       keyHandler(action, map, currentPlayer);
       updateTimerBomb(map, currentPlayer);
       system("clear");
       afficherMap(map);
       turn++;
       if (currentPlayer->invincibilityTime != 0){
         currentPlayer->invincibilityTime--;
       }
       }
   } while (map->nbPlayerAlive > 1);
}