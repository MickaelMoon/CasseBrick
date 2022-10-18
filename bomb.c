#include <stdio.h>
#include "struct.h"
#include "map.h"

void removeBomb(Bomb *bomb, Map * map){
    //printf("Removed Bomb: %d\n",bomb->id);
    for (int i = 0; i < map->nbBombsOnMap; i++){
        if (bomb == map->bombList[i]){
            //printf("bomb reconnized: n°%d, value: %p\n", i, map->bombList[i]);
            for (int k = i; k < map->nbBombsOnMap-1; k++){
                //printf("bomb k:%p, bomb k+1: %p\n", map->bombList[k], map->bombList[k+1]);
                Bomb * temp = map->bombList[k];
                map->bombList[k] = map->bombList[k+1];
                map->bombList[k+1] = temp;
                //printf("Edit: bomb k:%p, bomb k+1: %p\n", map->bombList[k], map->bombList[k+1]);
            }
            map->nbBombsOnMap--;
            map->bombList[map->nbBombsOnMap] = NULL;
            break;
        }
    }
    printf("updated bombList:\n");
    for(int i = 0; i < map->nbBombsOnMap; i++ ){
        printf("Bombe %d, joueur %c, timer %d\n", map->bombList[i]->id,map->bombList[i]->player.token, map->bombList[i]->timer);
    }
    //map->nbBombsOnMap--;
    //map->bombList = &newBombList;
}

void explosion(Bomb * bomb, Map *map){
    int X = bomb->x;
    int Y = bomb ->y;
    removeBomb(bomb, map);
    map->tab[Y][X] = 'F';
    // explosion to the right
    for (int i = X; i <= X+bomb->player.firePower; i++){
        if (i == map->columns || map->tab[Y][i] == 'x') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == i && map->bombList[j]->y == Y ) {
                    explosion(map->bombList[j], map);
                    break;
                }
            }
        } else if (map->tab[Y][i] == '1' || map->tab[Y][i] == '2' || map->tab[Y][i] == '3' || map->tab[Y][i] == '4'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[Y][i] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                    map->nbPlayerAlive--;
                }
            }
        } else if (map->tab[Y][i] == 'm'){
            map->tab[Y][i] = 'F';
            break;
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the left
    for (int i = X; i >= X-bomb->player.firePower; i--){
        if (i < 0 || map->tab[Y][i] == 'x') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == i && map->bombList[j]->y == Y ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[Y][i] == '1' || map->tab[Y][i] == '2' || map->tab[Y][i] == '3' || map->tab[Y][i] == '4'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[Y][i] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                    map->nbPlayerAlive--;
                }
            }
        } else if (map->tab[Y][i] == 'm'){
            map->tab[Y][i] = 'F';
            break;
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the top
    for (int i = Y; i >= Y-bomb->player.firePower; i--){
        if (i < 0 || map->tab[i][X] == 'x') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == X && map->bombList[j]->y == i ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == '1' || map->tab[i][X] == '2' || map->tab[i][X] == '3' || map->tab[i][X] == '4'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                    map->nbPlayerAlive--;
                }
            }
        } else if (map->tab[i][X] == 'm'){
            map->tab[i][X] = 'F';
            break;
        }
        map->tab[i][X] = 'F';
    }
    // explosion to the bottom
    for (int i = Y; i <= Y+bomb->player.firePower; i++){
        if (i == map->rows || map->tab[i][X] == 'x') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == X && map->bombList[j]->y == i ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == '1' || map->tab[i][X] == '2' || map->tab[i][X] == '3' || map->tab[i][X] == '4'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                    map->nbPlayerAlive--;
                }
            }
        } else if (map->tab[i][X] == 'm'){
            map->tab[i][X] = 'F';
            break;
        }
        map->tab[i][X] = 'F';
    }
    //system("clear");

    afficherMap(map);
    sleep(3);
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if (map->tab[i][j] == 'F'){
                map->tab[i][j] = ' ';
            }
        }
    }
}