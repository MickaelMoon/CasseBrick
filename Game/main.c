#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "../Server/server.h"
#include "../Client/client.h"


int main(int argc, char **argv) {

    printf("Welcome To Bomberm... Casse Brique Game ! (developped by Kanomi)\nPlease choose your play Mode:\n1) Start Local Game\n2) Start a server\n3) Join a server\n");
    int startingChoice;
    do{
        printf("Choice number ?\n");
        scanf("%d",&startingChoice);
    } while (startingChoice != 1 && startingChoice != 2 && startingChoice != 3);

    switch(startingChoice){
        case 1: {
        int nbHumanPlayers = 0;
            do{
                printf("How many human players ?\n");
                do { 
                    nbHumanPlayers = getchar(); 
                } while (nbHumanPlayers != '\n' && nbHumanPlayers != EOF);
                scanf("%d",&nbHumanPlayers);
            } while (nbHumanPlayers != 1 && nbHumanPlayers != 2 && nbHumanPlayers != 3 && nbHumanPlayers != 4);
            launchGame(nbHumanPlayers,argv[1]);
            break;
        }
        case 2:
            // voir parameters
            launchServer(argv[1]);
            break;
        case 3:{
            char **info = malloc(sizeof(char*)*2);
            info[0] = malloc(sizeof(char)*20);
            info[1] = malloc(sizeof(char)*20);
            printf("Please provide the server IP address:\n");
            scanf("%s", info[0]);
            printf("What is your pseudo ?\n");
            scanf("%s", info[1]);

            launchClient(2, info);

            free(info[0]);
            free(info[1]);
            free(info);
            break;
        }
    }

    return 0;
}