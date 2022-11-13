#include <wchar.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "keyboardHandler.h"
#include "map.h"
#include "../Server/server.h"
#include "../Client/client.h"
#include "bot.h"



Game * initGame(int nbHumanPlayers, char * mapProvided){
    Game * game = malloc(sizeof(Game));
    game->nbHumanPlayers = nbHumanPlayers;

    char ** mapChoosen = malloc(sizeof(char*)*6); //let say 6 maps max to be choosen
    for (int i = 0; i < 6; i++){
        mapChoosen[i] = malloc(sizeof(char)*20); //map filePath should not be higher than 20 characters
    }
    int nbOfMapChoosen = 0;
    int mapChoice = -1;
    int nb2playerMaps = 0;
    int nb4playerMaps = 0;

    if (mapProvided == NULL){

    const char *map2Players[4] = {
        [0] = "1: Classic | ",
        [1] = "2: CheeseMap | ",
        [2] = "3: Duel | ",
        [3] = "4: Random Generated Map |"
    };
    const char *map4Players[5] = {
        [0] = "5: FullDestruction | ",
        [1] = "6: CrossingBlades | ",
        [2] = "7: SkullIsland | ",
        [3] = "8: OverPowered | ",
        [4] = "9: Random Generated Map |"
    };
    // pendingMaps stocks the numbers corresponding to the map selected. Make it easier to check duplicate. Final mapChoosen tab is build at the end of the process.
    int *pendingMaps = malloc(sizeof(int)*6);


do {
    #ifdef WIN32
    system("cls");
    #else
    system("clear");
    #endif
   printf("Which map would you like in this game pool?\n");
   printf("Choices are:\n");
   if (nb4playerMaps == 0){
        printf("2 players maps available:\n");
        for (int i = 0; i < 4; i++){
            printf("%s",map2Players[i]);
        }
    printf("\n");
   }
   if (nb2playerMaps == 0){
        printf("4 players maps:\n");
        for (int i = 0; i < 5; i++){
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

    //solo player or server creator select the map he wants to add to the pool.
    do{
        int c; 
  
        do { 
            c = getchar(); 
        } while (c != '\n' && c != EOF); 
        mapChoice = -1;
    }while (scanf("%d",&mapChoice) != 1 || (mapChoice > 9 || mapChoice < 0) || (mapChoice > 4 && nb2playerMaps != 0) || (mapChoice < 5 && mapChoice > 0 && nb4playerMaps != 0));

    // 0 finish the period of selection
    if (mapChoice == 0){
        break;
    }
   
    // check if the number add or remove a map from the pool
    int alreadySelected = 0;
    for (int i = 0; i < nbOfMapChoosen; i++){
        if (pendingMaps[i] == mapChoice){
            alreadySelected = 1;
            break;
        }
    }
    if (alreadySelected == 0){ //new map
        pendingMaps[nbOfMapChoosen] = mapChoice;
        nbOfMapChoosen++;
        if (mapChoice == 1 || mapChoice == 2 || mapChoice == 3 || mapChoice == 4){
            nb2playerMaps++;
        } else {
            nb4playerMaps++;
        }
    } else { //duplicate map = map to be removed
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
        if (mapChoice == 1 || mapChoice == 2 || mapChoice == 3 || mapChoice == 4){
            nb2playerMaps--;
        } else {
            nb4playerMaps--;
        }
    }


}while (nbOfMapChoosen < 6 && mapChoice != 0);

//tableau de filePath

    //filePath constructor to aliment mapChoosen
   for (int i = 0; i < nbOfMapChoosen; i++){
    char str [40] = "./Maps/map";
    str[10] = pendingMaps[i] + '0';
    char end [5] = ".txt";
    strcat(str,end);
    strcpy(mapChoosen[i], str);
   }
   //Default map if nothing has been choosen
   if (nbOfMapChoosen == 0){
    mapChoosen[0] = "./Maps/map1.txt";
    nbOfMapChoosen = 1;
    nb2playerMaps++;
   }
    game->nbMaxPlayer = nb2playerMaps > nb4playerMaps ? 2 : 4;

    } else {
        mapChoosen[0] = mapProvided;
        nbOfMapChoosen = 1;
        game->nbMaxPlayer = nbPlayerMaxPerMap(mapProvided);
    }
   game->filePathMapChoosen = mapChoosen;
   game->nbOfMapsAvailable = nbOfMapChoosen;

    return game;
}

//Solo or multiplayer game on same Machine (not server)
void launchGame(int nbHumanPlayers, char *mapProvided){

    Game *game = initGame(nbHumanPlayers, mapProvided);
    game->status = soloplayer;
    srand(time(NULL));
    int randomfilePath = -1;
    int previousFilePath = -1;
    int startOfMap = 1;
    int continuer = 1;

    /*MAP GENERATED*/
    while(continuer){

        if (startOfMap){
            //generate a random filePath (different from previous one, except if only one map has been selected)
            if (game->nbOfMapsAvailable == 1){
                randomfilePath = 0;
            } else {
                do{
                    randomfilePath = rand()%game->nbOfMapsAvailable;

                }while(randomfilePath == previousFilePath);
                previousFilePath = randomfilePath;
            }
            char *filePath = game->filePathMapChoosen[randomfilePath];

            if (filePath[10] == '4'){
                generateProceduralMap(2);
            } else if (filePath[10] == '9'){
                generateProceduralMap(4);
            }

            Map * map = initMap(filePath, game->status,game->nbHumanPlayers);
            game->currentMap = map;
            startOfMap = 0;
        }
   /* */

    /*PLAY START HERE*/
        Map * map = game->currentMap;
        int c; 
        
        do { 
            c = getchar(); 
                } while (c != '\n' && c != EOF);
        char action;

        #ifdef WIN32
        system("cls");
        #else
        system("clear");
        #endif
        int turn = 0;
        afficherMap(map);
            Player * currentPlayer;

        do{
            currentPlayer = map->playerList[turn%map->nbPlayers];

            if (currentPlayer->status == isDead){
                turn++;
            }else if (currentPlayer->status == isBot){
                action = botNextMove(map, currentPlayer);
                keyHandler(action, map, currentPlayer);
            }
            else {
            printf("\nPlayer %c status:\nMax bomb: %d / Fire Pwr: %d\nPasseBomb: %d / BombKick: %d\nInvincibility: %d turn(s)\nHeart Shield: %d / Life(s): %d\n", currentPlayer->token, currentPlayer->bombMax, currentPlayer->firePower, currentPlayer->passBombs, currentPlayer->bombKick, currentPlayer->invincibilityTime, currentPlayer->heart, currentPlayer->life);

            printf("\nDo something, Player %c:\n",currentPlayer->token);
            int c;

            scanf("%c",&action);

            while (keyHandler(action, map, currentPlayer) != 0){
                int c; 
        
                do { 
                    c = getchar(); 
                } while (c != '\n' && c != EOF);
                scanf("%c",&action);
            }
            do { 
                    c = getchar(); 
                } while (c != '\n' && c != EOF);
            }
            afficherMap(map);
            updateTimerBomb(map, currentPlayer);
            if (map->pause){
                sleep(3);
                map->pause = 0;
                for (int i = 0; i < map->rows; i++) {
                    for (int j = 0; j < map->columns; j++) {
                        if (map->tab[i][j] == 'F'){
                            map->tab[i][j] = ' ';
                        }
                    }
                }
            }
            #ifdef WIN32
            system("cls");
            #else
            system("clear");
            #endif
            afficherMap(map);
            turn++;
            if (currentPlayer->invincibilityTime != 0){
                currentPlayer->invincibilityTime--;
            }
            
        } while (map->nbPlayerAlive > 1);
        char wish;
        printf("Do you wish to continue ? y/n\n");
        scanf("%c",&wish);
        switch (wish){
            case 'y':
                startOfMap = 1;
                break;
            default:
                continuer = 0;
                printf("Thank you for playing. See you soon !\n");
                sleep(3);
                break;
        }
        for (int i = 0; i < map->nbPlayers; i++){
            free(map->playerList[i]);
        }
        free(map->playerList);
        for (int i = 0; i < map->nbBombsOnMap; i++){
            free(map->bombList[i]);
        }
        free(map->bombList);
        free(map);
    }
   /* */
}