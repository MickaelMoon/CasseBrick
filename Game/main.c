#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "../Server/server.h"
#include "../Client/client.h"


int main() {

    printf("Welcome To Bomberm... Casse Brique Game ! (developped by Kanomi)\nPlease choose your play Mode:\n1) Start Solo Game\n2) Start a server\n3) Join a server\n");
    int startingChoice;
    do{
        printf("Choice number ?\n");
        scanf("%d",&startingChoice);
    } while (startingChoice != 1 && startingChoice != 2 && startingChoice != 3);

    switch(startingChoice){
        case 1:
            launchGame();
            break;
        case 2:
            // voir parameters
            launchServer();
            //break; //see if remove break to launch server & client as the server would like to play too
        case 3:
            char **argv = malloc(sizeof(char*)*2);
            argv[0] = malloc(sizeof(char)*20);
            argv[1] = malloc(sizeof(char)*20);
            printf("Please provide the server IP address:\n");
            scanf("%s", argv[0]);
            printf("What is your pseudo ?\n");
            scanf("%s", argv[1]);
            launchClient(2, argv);
            break;
    }

    

    return 0;
}



