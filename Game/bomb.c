#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "map.h"
#include "../Server/server.h"


Bomb * initBomb(Player * player){
    Bomb * bomb = malloc(sizeof(Bomb));
    bomb->x = player->x;
    bomb->y = player->y;
    bomb->player = player;
    bomb->player->currentNumberOfBombsLaunched++;
    bomb->timer = 3;

    return bomb;
};

int poseBomb(Map * map, Player * player){
    printf("nbBombLaunched: %d\n",player->currentNumberOfBombsLaunched);

    if (player->currentNumberOfBombsLaunched < player->bombMax){
        int bombAlreadyOnTheTile = 0;
        for (int i = 0; i < map->nbBombsOnMap; i++){
            int x = map->bombList[i]->x;
            int y = map->bombList[i]->y;
            if (x == player->x && y == player->y){
                bombAlreadyOnTheTile = 1;
            }
        }
        if (bombAlreadyOnTheTile == 0){
            map->bombList[map->nbBombsOnMap] = initBomb(player);
            map->bombList[map->nbBombsOnMap]->id = map->nbBombsOnMap;
            printf("x: %d, y: %d\n", map->bombList[map->nbBombsOnMap]->x, map->bombList[map->nbBombsOnMap]->y);
            map->nbBombsOnMap++;
            printf("nbBombLaunched: %d\n",player->currentNumberOfBombsLaunched);
        } else {
            printf("a bomb is already on this tile\n");
            return -1;
        }
    } else {
        printf("Maximum bombs already launched!\n");
        return -1;
    }
    
    updateDangerMap(map);

    return 0;
}

void itemGenerator(Map * map, int x, int y){
    int r = rand()%100;
    char item;
    if (r < 50){
        item = ' '; //Nothing dropped
    } else if (r < 55){
        item = '+'; //Fire Up
    } else if (r < 60){
        item = '-'; //Fire down
    } else if (r < 65){
        item = 'O'; //Bomb Up
    } else if (r < 70){
        item = 'o'; //Bomb Down
    } else if (r < 75){
        item = 'P'; //PasseBomb
    } else if (r < 80){
        item = 'K'; //KickBomb
    } else if (r < 85){
        item = '*'; //Invincibility
    } else if (r < 90){
        item = '#'; //Heart
    } else if (r < 95){
        item = '@'; //Life
    } else if (r < 98){
        item = '&'; //Fire MAX
    } else {
        item = ' ';
    }
    map->tab[y][x] = item;
}

void removeBomb(Bomb *bomb, Map * map){
        //printf("nbBombLaunched: %d\n",bomb->player->currentNumberOfBombsLaunched);

    for (int i = 0; i < map->nbBombsOnMap; i++){
        if (bomb == map->bombList[i]){ //get the index of the exploding bomb
        bomb->player->currentNumberOfBombsLaunched--;
            for (int k = i; k < map->nbBombsOnMap-1; k++){ //put the exploded bomb at the end of the array
                Bomb * temp = map->bombList[k];
                map->bombList[k] = map->bombList[k+1];
                map->bombList[k+1] = temp;
            }
            map->nbBombsOnMap--;
            map->bombList[map->nbBombsOnMap] = NULL; //remove the exploded bomb from the array
                //printf("nbBombLaunched: %d\n",bomb->player->currentNumberOfBombsLaunched);

            break;
        }
    }
    updateDangerMap(map);
}

void explosion(Bomb * bomb, Map *map){
    int X = bomb->x;
    int Y = bomb ->y;
    removeBomb(bomb, map);
    map->tab[Y][X] = 'F';
    // explosion to the right
    for (int i = X, k = 0; k <= bomb->player->firePower; i++, k++){
        if (i == map->columns){
            i = 0;
        }
        if (map->tab[Y][i] == 'x') {
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
                    Player * playerHit = map->playerList[k];
                    if (playerHit->invincibilityTime == 0){
                        if (playerHit->heart == 1){
                            playerHit->heart = -1;
                        } else if (playerHit->life > 0){
                                playerHit->life--;
                            } else {
                                playerHit->status = isDead;
                                playerHit->x = -1;
                                playerHit->y = -1;
                                for (int z = 0; z < map->nbBombsOnMap;z++){
                                    if (map->bombList[z]->x == i && map->bombList[z]->y == Y){
                                        explosion(map->bombList[z], map);
                                        break;
                                    }
                                }
                                map->tab[Y][i] = 'F';
                                map->nbPlayerAlive--;

                            }
                    }
                }
            }
        } else if (map->tab[Y][i] == 'm'){
            itemGenerator(map, i, Y);
            if (map->tab[Y][i] == ' '){
                map->tab[Y][i] = 'F';
            }
            break;
        } else {
            map->tab[Y][i] = 'F';
        }
    }
    // explosion to the left
    for (int i = X, k = 0; k <= bomb->player->firePower; i--, k++){
        if (i < 0){
            i = map->columns-1;
        }
        if (map->tab[Y][i] == 'x') {
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
                    Player * playerHit = map->playerList[k];
                    if (playerHit->invincibilityTime == 0){
                        if (playerHit->heart == 1){
                            playerHit->heart = -1;
                        } else if (playerHit->life > 0){
                                playerHit->life--;
                            } else {
                                playerHit->status = isDead;
                                playerHit->x = -1;
                                playerHit->y = -1;
                                for (int z = 0; z < map->nbBombsOnMap;z++){
                                    if (map->bombList[z]->x == i && map->bombList[z]->y == Y){
                                        explosion(map->bombList[z], map);
                                    }
                                }
                                map->tab[Y][i] = 'F';
                                map->nbPlayerAlive--;
                            }
                    }
                }
            }
        } else if (map->tab[Y][i] == 'm'){
            itemGenerator(map, i, Y);
            if (map->tab[Y][i] == ' '){
                map->tab[Y][i] = 'F';
            }
            break;
        } else {
            map->tab[Y][i] = 'F';
        }
    }
    // explosion to the top
    for (int i = Y, k = 0; k <= bomb->player->firePower; i--, k++){
        if (i < 0){
            i = map->rows-1;
        }
        if (map->tab[i][X] == 'x') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j <= map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == X && map->bombList[j]->y == i ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == '1' || map->tab[i][X] == '2' || map->tab[i][X] == '3' || map->tab[i][X] == '4'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k]->token) {
                    Player * playerHit = map->playerList[k];
                    if (playerHit->invincibilityTime == 0){
                        if (playerHit->heart == 1){
                            playerHit->heart = -1;
                        } else if (playerHit->life > 0){
                                playerHit->life--;
                            } else {
                                playerHit->status = isDead;
                                playerHit->x = -1;
                                playerHit->y = -1;
                                for (int z = 0; z < map->nbBombsOnMap;z++){
                                    if (map->bombList[z]->x == X && map->bombList[z]->y == i){
                                        explosion(map->bombList[z], map);
                                    }
                                }
                                map->tab[i][X] = 'F';
                                map->nbPlayerAlive--;
                            }
                    }
                }
            }
        } else if (map->tab[i][X] == 'm'){
            itemGenerator(map, X, i);
            if (map->tab[i][X] == ' '){
                map->tab[i][X] = 'F';
            }
            break;
        } else {
            map->tab[i][X] = 'F';
        }
    }
    // explosion to the bottom
    for (int i = Y, k = 0; k <= bomb->player->firePower; i++, k++){
        if (i == map->rows){
            i = 0;
        }
        if (map->tab[i][X] == 'x') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j <= map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == X && map->bombList[j]->y == i ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == '1' || map->tab[i][X] == '2' || map->tab[i][X] == '3' || map->tab[i][X] == '4'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k]->token) {
                    Player * playerHit = map->playerList[k];
                    if (playerHit->invincibilityTime == 0){
                        if (playerHit->heart == 1){
                            playerHit->heart = -1;
                        } else if (playerHit->life > 0){
                                playerHit->life--;
                            } else {
                                playerHit->status = isDead;
                                playerHit->x = -1;
                                playerHit->y = -1;
                                for (int z = 0; z < map->nbBombsOnMap;z++){
                                    if (map->bombList[z]->x == X && map->bombList[z]->y == i){
                                        explosion(map->bombList[z], map);
                                    }
                                }
                                map->tab[i][X] = 'F';
                                map->nbPlayerAlive--;
                            }
                    }
                }
            }
        } else if (map->tab[i][X] == 'm'){
            itemGenerator(map, X, i);
            if (map->tab[i][X] == ' '){
                map->tab[i][X] = 'F';
            }
            break;
        } else {
            map->tab[i][X] = 'F';
        }
    }
    
    system("clear");

    afficherMap(map);
    if (map->playerList[0]->socket != -1){
        sendAll(map->playerList,map->nbPlayers, serializeMap(map));
    }
    map->pause = 1;
}