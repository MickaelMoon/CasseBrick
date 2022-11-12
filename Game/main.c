#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "../Server/server.h"
#include "../Client/client.h"


int main() {

    printf("Welcome To Bomberm... Casse Brique Game ! (developped by Kanomi)\nPlease choose your play Mode:\n1) Start Local Game\n2) Start a server\n3) Join a server\n");
    int startingChoice;
    do{
        printf("Choice number ?\n");
        scanf("%d",&startingChoice);
    } while (startingChoice != 1 && startingChoice != 2 && startingChoice != 3);

    switch(startingChoice){
        case 1:
        int nbHumanPlayers = 0;
            do{
                printf("How many human players ?\n");
                do { 
                    nbHumanPlayers = getchar(); 
                } while (nbHumanPlayers != '\n' && nbHumanPlayers != EOF);
                scanf("%d",&nbHumanPlayers);
            } while (nbHumanPlayers != 1 && nbHumanPlayers != 2 && nbHumanPlayers != 3 && nbHumanPlayers != 4);
            launchGame(nbHumanPlayers);
            break;
        case 2:
            // voir parameters
            launchServer();
            break;
        case 3:
            char **argv = malloc(sizeof(char*)*2);
            argv[0] = malloc(sizeof(char)*20);
            argv[1] = malloc(sizeof(char)*20);
            printf("Please provide the server IP address:\n");
            scanf("%s", argv[0]);
            printf("What is your pseudo ?\n");
            scanf("%s", argv[1]);

            launchClient(2, argv);

            free(argv[0]);
            free(argv[1]);
            free(argv);
            break;
    }

    return 0;
}