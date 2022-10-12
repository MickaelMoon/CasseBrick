#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <io.h>

typedef enum {
    waiting = 0,
    isDead = 0,
    isPlaying = 1,
} PlayerStatus;

typedef struct {
    char token;
    PlayerStatus status;
    int bombMax;
    int currentNumberOfBombsLaunched;
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
    int nbBombsOnMap;
    int nbPlayers;
    Bomb * bombList; // Array of bombs launched, to be modified with each exploding bombs
    Player * playerList; // Array of players in the game, do not remove any player. Modifies the PlayerStatus if needed.
}Map;

void removeBomb(Bomb *bomb, Bomb * bombList){

};

void launchBomb(int x, int y, Player * player){

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
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j].x == i && map->bombList[j].y == Y ) {
                    explosion(&map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[Y][i] == 'Player.token'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[Y][i] == map->playerList[k].token) {
                    map->playerList[k].status = isDead;
                }
            }
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the left
    for (int i = X; i > X-bomb->firePower; i--){
        if (i < 0 || map->tab[Y][i] == 'X') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j].x == i && map->bombList[j].y == Y ) {
                    explosion(&map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[Y][i] == 'Player.token'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[Y][i] == map->playerList[k].token) {
                    map->playerList[k].status = isDead;
                }
            }
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the top
    for (int i = Y; i > Y-bomb->firePower; i--){
        if (i < 0 || map->tab[i][X] == 'X') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j].x == X && map->bombList[j].y == i ) {
                    explosion(&map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == 'Player.token'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k].token) {
                    map->playerList[k].status = isDead;
                }
            }
        }
        map->tab[i][X] = 'F';
    }
    // explosion to the bottom
    for (int i = Y; i < Y+bomb->firePower; i++){
        if (i == map->rows || map->tab[i][X] == 'X') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j].x == X && map->bombList[j].y == i ) {
                    explosion(&map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == 'Player.token'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k].token) {
                    map->playerList[k].status = isDead;
                }
            }
        }
        map->tab[i][X] = 'F';
    }
}

char **initialiseTab(int rows, int columns){
    char ** tab = malloc(sizeof(char *) * rows);

    for (int i = 0; i < rows; ++i) {
        tab[i] = malloc(sizeof(char)* columns);
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if(i == 0 || i == rows - 1 || j == 0 || j == columns - 1){
                tab[i][j] = 'x';
            }
            else if(j%3 == 0 && i%2 == 0){
                tab[i][j] = 'x';
            }
            else{
                tab[i][j] = ' ';
            }
        }
    }

    return tab;
}

//void afficherMap(Map *map){
//
//    for (int i = 0; i < map->rows; ++i) {
//
//        for (int j = 0; j < map->columns; ++j) {
//            printf("%c", map->tab[i][j]);
//        }
//        printf("\n");
//    }
//}

void afficherMap(char **tab){
    int rows = 6;
    int column = 10;

    for (int i = 0; i < rows; ++i) {

        for (int j = 0; j < column; ++j) {
            if(tab[i][j] == 'x'){

                wprintf(L"\x2593");
            }
            else{
                wprintf(L" ");
            }
            //printf("%c", tab[i][j]);
        }
        wprintf("\n");
    }
}

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);


    char** tab = initialiseTab(6,10);

    afficherMap(tab);

    return 0;
}



