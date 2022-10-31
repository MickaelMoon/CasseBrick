#include <wchar.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>

#include "struct.h"
#include "keyboardHandler.h"
//#include "bomb.h"
#include "map.h"
#include "../Server/server.h"
#include "../Client/client.h"

Game * initGame(int mode){
    Game * game = malloc(sizeof(Game));
    game->status = mode;

    char ** mapChoosen = malloc(sizeof(char*)*6); //let say 6 maps max to be choosen
    for (int i = 0; i < 6; i++){
        mapChoosen[i] = malloc(sizeof(char)*20); //map filePath should not be higher than 20 characters
    }
    int nbOfMapChoosen;
    int mapChoice = -1;


do {
   printf("Which map would you like in this game pool?\n");
   printf("Choices are:\n");
   printf("2 players maps:\n");
   //faire boucle des .txt dans /Game/Map/2Players
   printf(".......\n");
   printf("4 players maps:\n");
   //faire boucle des .txt dans /Game/Map/4Players
   printf("....\n");
   printf("write the number you wish to add to the pool. Write it again to remove it from the pool.\nWrite 0 to end the selection.");
   printf("current maps choosen:\n");
   //faire boucle dans currentChoosenMap

   printf(".....\n");

   scanf("%d",&mapChoice);

   char * filePath;
   switch(mapChoice){
    case 1:
        filePath = "Map/map.txt";
        break;
    case 2:
        filePath = "Map/map2.txt";
        break;
   }
}while (nbOfMapChoosen < 6 && mapChoice != 0);



}

void launchGame(int mode){
    /*int nbPlayer;

   do{
       printf("How Many players will play ? (2-4)\n");
       scanf("%d", &nbPlayer);
   } while (nbPlayer != 2 && nbPlayer != 3 && nbPlayer != 4);*/

    //initGame(mode);

    //boucle while Do you want to play next map ? Yes all player/ or quit/wait other player

    char * filePath = "./Map/map.txt"; // Temp

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