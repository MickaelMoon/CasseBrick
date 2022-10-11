#include <stdio.h>
#include <stdlib.h>

typedef enum {
    waiting = 0,
    isDead = 0,
    isPlaying = 1,
} PlayerStatus;

typedef struct {
    int ID;
    PlayerStatus status;
    int bombMax;
    int currentBombLaunched;
} Player;

typedef struct {
    Player player;
    int turnRemainingBeforeExploding;
    int firePower;
    int x;
    int y;
} Bomb;

typedef struct {
    int columns;
    int rows;
    char ** tab;
    Bomb ** bombList;
    Player ** playerMap;
}Map;

void removeBomb(Bomb *bomb, Bomb ** bombList){

};

void explosion(Bomb *bomb, Map *map){
    int X = bomb->x;
    int Y = bomb ->y;
    removeBomb(bomb, &(*map->bombList));
    map->tab[Y][X] = 'F';
    // explosion to the right
    for (int i = X; i < X+bomb->firePower; i++){
        if (i == map->columns || map->tab[Y][i] == 'X') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            explosion(&map->bombList[Y][i], &(*map));
        } else if (map->tab[Y][i] == 'P'){
            map->playerMap[Y][i].status = isDead;
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the left
    for (int i = X; i > X-bomb->firePower; i--){
        if (i < 0 || map->tab[Y][i] == 'X') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            explosion(&map->bombList[Y][i], &(*map));
        } else if (map->tab[Y][i] == 'P'){
            map->playerMap[Y][i].status = isDead;
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the top
    for (int i = Y; i > Y-bomb->firePower; i--){
        if (i < 0 || map->tab[i][X] == 'X') {
            break;
        } else if (map->tab[i][X] == 'B'){
            explosion(&map->bombList[i][X], &(*map));
        } else if (map->tab[i][X] == 'P'){
            map->playerMap[i][X].status = isDead;
        }
        map->tab[i][X] = 'F';
    }
    // explosion to the bottom
    for (int i = Y; i < Y+bomb->firePower; i++){
        if (i == map->rows || map->tab[i][X] == 'X') {
            break;
        } else if (map->tab[i][X] == 'B'){
            explosion(&map->bombList[i][X], &(*map));
        } else if (map->tab[i][X] == 'P'){
            map->playerMap[i][X].status = isDead;
        }
        map->tab[i][X] = 'F';
    }
}

int main() {
    return 0;
}



