#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>
#include "struct.h"
#include "keyboardHandler.h"

void afficherMap(Map * map){
    // ajoute les bombes sur la map (gère le soucis où un joueur pose ou traverse une bombe, afin de n'afficher que le joueur)
    for (int i = 0; i < map->nbBombsOnMap; i++){
        int x = map->bombList[i]->x;
        int y = map->bombList[i]->y;
        if (map->tab[y][x] == ' ' && map->bombList[i]->timer > 0){
            map->tab[y][x] = 'B';
        }
    }

    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            printf("%c",map->tab[i][j]);
        }
        printf("\n");
    }
}

void removeBomb(Bomb *bomb, Map * map){
    Bomb * bombList = *map->bombList;
    //Bomb * newBombList = malloc(sizeof(Bomb)*30);
    for (int i = 0, j = 0; i < map->nbBombsOnMap; i++){
        if (bomb->x == bombList[i].x && bomb->y == bombList[i].y ){
            for (int k = i; k < map->nbBombsOnMap-2; k++){
                bombList[k] = bombList[k+1];
            }
            //break;
            //newBombList[j] = bombList[i];
            //j++;
        }
    }
    map->nbBombsOnMap--;
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

char **initialiseTab(int rows, int columns){
    char ** tab = malloc(sizeof(char *) * rows);

    for (int i = 0; i < rows; ++i) {
        tab[i] = malloc(sizeof(char)* columns);
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if(i == 0 || i == rows - 1 || j == 0 || j == columns - 1) {
                tab[i][j] = 'x' ;
            }
            else if(j%3 == 0 && i%2 == 0) {
                tab[i][j] = 'x' ;
            }
            else{
                tab[i][j] = ' ' ;
            }
        }
    }
    tab[2][1] = 'm';

    return tab;
}

Player * initPlayer(int i){
    Player * player = malloc(sizeof (Player));
    player->x = 0; // a updater dans la création de map
    player->y = 0; // a updater dans la création de map
    player->id = i+1;
    player->token = player->id+'0';
    if (i == 0){
        player->status = isPlaying;
    } else {
        player->status = waiting;
    }
    player->bombMax = 1;
    player->currentNumberOfBombsLaunched = 0;
    player->bombKick = 1;
    player->firePower = 2;

    return player;
}

Map * initMap(int columns, int rows, int nbPlayer){
    Map * map = malloc(sizeof(Map));
    map->columns = columns;
    map->rows = rows;
    map->tab = initialiseTab(rows,columns);
    map->nbBombsOnMap = 0;
    map->nbPlayers = nbPlayer;
    Bomb ** bombList = malloc(sizeof(Bomb*)*40); //Max 40 bombs at the same time on map
    map->bombList = bombList;
    Player ** playerList = malloc(sizeof(Player*)*nbPlayer);
    for (int i = 0; i < nbPlayer; i++){
        playerList[i] = malloc(sizeof(Player));
        playerList[i] = initPlayer(i);
        if (i == 0){
            playerList[0]->x = 1;
            playerList[0]->y = 1;
            map->tab[1][1] = playerList[0]->token;
        } else if (i == 1){
            playerList[1]->x = columns-2;
            playerList[1]->y = rows -2;
            map->tab[rows-2][columns -2] = playerList[1]->token;
        } else if (i == 2){
            playerList[2]->x = columns-2;
            playerList[2]->y = 1;
            map->tab[1][columns-2] = playerList[2]->token;
        } else if (i == 3){
            playerList[3]->x = 1;
            playerList[3]->y = rows -2;
            map->tab[rows-2][1] = playerList[3]->token;
        }
    }
    map->playerList = playerList;

    return map;
}

void updateTimerBomb(Map * map, Player * player){
    for (int i = 0; i < map->nbBombsOnMap; i++){
        if (map->bombList[i]->timer == 0){
            map->bombList[i]->timer--;
            explosion(map->bombList[i], map);
        }
        if (map->bombList[i]->player.token == player->token){
            map->bombList[i]->timer--;
        };
        printf("Bombe %d, joueur %c, timer %d\n", map->bombList[i]->id,map->bombList[i]->player.token, map->bombList[i]->timer);
    }

}


int main() {
    int nbPlayer;
    int rows = 6;
    int columns = 10;

    do{
        printf("How Many players will play ? (2-4)\n");
        scanf("%d", &nbPlayer);
    } while (nbPlayer != 2 && nbPlayer != 3 && nbPlayer != 4);

    Map * map = initMap(columns,rows,nbPlayer);

    int play = 0;
    char action;

    //system("clear");
    afficherMap(map);
    do{
        printf("nb Bomb on field: %d\n", map->nbBombsOnMap);

        printf("Do something, Player %c:\n",map->playerList[play%nbPlayer]->token);
        scanf(" %c",&action);
        keyHandler(action, map, map->playerList[play%nbPlayer]);
        updateTimerBomb(map, map->playerList[play%nbPlayer]);
        //system("clear");
        afficherMap(map);
        play++;
    } while (play < 200);

    return 0;
}



