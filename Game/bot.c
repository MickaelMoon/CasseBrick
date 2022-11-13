#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "struct.h"
#include "collisionChecker.h"


char botNextMove(Map *map, Player *player){

    srand(time(NULL));
    int nbChoices = 0;
    int nbDangerousChoices = 0;
    char movePossible[6];
    char movePossibleButDangerous[6];
    char finalMove;
    int y = player->y;
    int x = player->x;

    if (y == 0){
        if (suroundingTileChecker(x, map->rows-1, map) == 0){
            movePossible[nbChoices] = 'z';
            nbChoices++;
        } else if (suroundingTileChecker(x, map->rows-1, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 'z';
            nbDangerousChoices++;
        }
    } else {
        if(suroundingTileChecker(x, y-1, map) == 0){
            movePossible[nbChoices] = 'z';
            nbChoices++;
        } else if (suroundingTileChecker(x, y-1, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 'z';
            nbDangerousChoices++;
        }
    }

    if (y == map->rows -1){
        if (suroundingTileChecker(x, 0, map) == 0){
            movePossible[nbChoices] = 's';
            nbChoices++;
        } else if (suroundingTileChecker(x, 0, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 's';
            nbDangerousChoices++;
        }
    } else {
        if(suroundingTileChecker(x, y+1, map) == 0){
            movePossible[nbChoices] = 's';
            nbChoices++;
        } else if(suroundingTileChecker(x, y+1, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 's';
            nbDangerousChoices++;
        }
    }

    if (x == map->columns-1){
        if (suroundingTileChecker(0, y, map) == 0){
            movePossible[nbChoices] = 'd';
            nbChoices++;
        } else if (suroundingTileChecker(0, y, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 'd';
            nbDangerousChoices++;
        }
    } else {
        if(suroundingTileChecker(x+1, y, map) == 0){
            movePossible[nbChoices] = 'd';
            nbChoices++;
        } else if(suroundingTileChecker(x+1, y, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 'd';
            nbDangerousChoices++;
        }
    }

    if (x == 0){
        if (suroundingTileChecker(map->columns-1, y, map) == 0){
            movePossible[nbChoices] = 'q';
            nbChoices++;
        } else if (suroundingTileChecker(map->columns-1, y, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 'q';
            nbDangerousChoices++;
        }
    } else {
        if(suroundingTileChecker(x-1, y, map) == 0){
            movePossible[nbChoices] = 'q';
            nbChoices++;
        } else if(suroundingTileChecker(x-1, y, map) == -1){
            movePossibleButDangerous[nbDangerousChoices] = 'q';
            nbDangerousChoices++;
        };
    }

    if (nbDangerousChoices == 0){
        movePossibleButDangerous[nbDangerousChoices] = 'w';
        nbDangerousChoices++;
    }

    if (nbChoices == 0 && map->dangerMap[y][x] == 0){ //aucun choix sure autour, mais case actuelle safe
        finalMove = 'w';
    } else if (nbChoices == 0 && map->dangerMap[y][x] == 1){ //aucun choix sur et case actuelle dangeureuse
        finalMove = movePossibleButDangerous[rand()%nbDangerousChoices];
    } else {
        if (map->dangerMap[y][x] == 0){ //si la case actuelle n'est pas dangeureuse, on rajoute 2 options a tableau des choix surs
            if ( !((x == 1 && y == 1) || (x == map->columns-1 && y == 1) || (x == map->columns-1 && y == map->rows-1) || (x == 1 && y == map->rows-1)) ){
                movePossible[nbChoices] = 'b';
            }
            nbChoices++;
        }
        finalMove = movePossible[rand()%nbChoices];
    }
    printf("Possible Move:");
    for (int i = 0; i < nbChoices; i++){
        printf("%c ",movePossible[i]);
    }
    printf("\nDangerous Moves:");
    for (int i = 0; i < nbDangerousChoices; i++){
        printf("%c ",movePossibleButDangerous[i]);
    }
    printf("\n");

    if (rand()%20 == 19){ //1 chance sur 20 que l'ordi prenne une mauvaise décision
        finalMove = movePossibleButDangerous[rand()%nbDangerousChoices];
    } 

    return finalMove;
}

int suroundingTileChecker(int x, int y, Map *map){
    switch (map->tab[y][x]){
        case 'x':
        case 'm':
        case '1':
        case '2':
        case '3':
        case '4':
            return 1;
            break;
        case 'B':
            return 2;
            break;
        default:
            if (map->dangerMap[y][x] == 1){
                return -1;
                break;
            }
            return 0;
            break;
    }
}