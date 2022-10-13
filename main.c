#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <fcntl.h>
#include <locale.h>



typedef enum {
    waiting = 0,
    isDead = 0,
    isPlaying = 1,
} PlayerStatus;

typedef struct {
    int id;
    char token;
    PlayerStatus status;
    int bombMax;
    int currentNumberOfBombsLaunched;
    int firePower;
} Player;

typedef struct {
    Player player;
    int turnRemainingBeforeExploding;
    int x;
    int y;
} Bomb;

typedef struct {
    int columns;
    int rows;
    char ** tab;
    int nbBombsOnMap;
    int nbPlayers;
    Bomb ** bombList; // Array of bombs launched, to be modified with each exploding bombs
    Player ** playerList; // Array of players in the game, do not remove any player. Modifies the PlayerStatus if needed.
}Map;


void removeBomb(Bomb *bomb, Bomb * bombList, Map * map){
    for (int i = 0; i < map->nbBombsOnMap; i++){
        if (bomb->x == bombList[i].x && bomb->y == bombList[i].y ){
            for (int j = i; j < map->nbBombsOnMap-1; j++){
                bombList[j] = bombList[j+1];
            }
            break;
        }
    }
    map->nbBombsOnMap--;
};

Bomb * initBomb(int x, int y, Player * player){
    Bomb * bomb = malloc(sizeof(Bomb));
    bomb->x = x;
    bomb->y = y;
    bomb->player = *player;
    bomb->player.currentNumberOfBombsLaunched++;

    return bomb;
};

void explosion(Bomb * bomb, Map *map){
    int X = bomb->x;
    int Y = bomb ->y;
    removeBomb(bomb, *map->bombList, map);
    map->tab[Y][X] = 'F';
    // explosion to the right
    for (int i = X; i < X+bomb->player.firePower; i++){
        if (i == map->columns || map->tab[Y][i] == 'X') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == i && map->bombList[j]->y == Y ) {
                    explosion(map->bombList[j], map);
                    break;
                }
            }
        } else if (map->tab[Y][i] == 'P'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[Y][i] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                }
            }
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the left
    for (int i = X; i > X-bomb->player.firePower; i--){
        if (i < 0 || map->tab[Y][i] == 'X') {
            break;
        } else if (map->tab[Y][i] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == i && map->bombList[j]->y == Y ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[Y][i] == 'P'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[Y][i] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                }
            }
        }
        map->tab[Y][i] = 'F';
    }
    // explosion to the top
    for (int i = Y; i > Y-bomb->player.firePower; i--){
        if (i < 0 || map->tab[i][X] == 'X') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == X && map->bombList[j]->y == i ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == 'P'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
                }
            }
        }
        map->tab[i][X] = 'F';
    }
    // explosion to the bottom
    for (int i = Y; i < Y+bomb->player.firePower; i++){
        if (i == map->rows || map->tab[i][X] == 'X') {
            break;
        } else if (map->tab[i][X] == 'B'){
            for (int j = 0; j < map->nbBombsOnMap; j++){
                if (map->bombList[j]->x == X && map->bombList[j]->y == i ) {
                    explosion(map->bombList[j], &(*map));
                    break;
                }
            }
        } else if (map->tab[i][X] == 'P'){
            for (int k = 0; k < map->nbPlayers; k++) {
                if (map->tab[i][X] == map->playerList[k]->token) {
                    map->playerList[k]->status = isDead;
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
            if(i == 0 || i == rows - 1 || j == 0 || j == columns - 1) {
                tab[i][j] = 'x';
            }
            else if(j%3 == 0 && i%2 == 0) {
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
    setlocale(LC_CTYPE, "");

    for (int i = 0; i < rows; ++i) {

        for (int j = 0; j < column; ++j) {
            if(tab[i][j] == 'x'){
                //wchar_t test = 0x2593;
                //wprintf(L"%lc",test);
                printf("X");
            }
            else{
                //wprintf(L" ");
                printf(" ");
            }
            //printf("%c", tab[i][j]);
        }
        //wprintf(L"\n");
        printf("\n");
    }
}

Player * initPlayer(int i){
    Player * player = malloc(sizeof (Player));
    player->id = i+1;
    player->token = player->id+'0';
    if (i == 0){
        player->status = isPlaying;
    } else {
        player->status = waiting;
    }
    player->bombMax = 1;
    player->currentNumberOfBombsLaunched = 0;

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
    }
    map->playerList = playerList;
    map->tab = initialiseTab(rows,columns);

    return map;
}

int main() {
    //_setmode(_fileno(stdout), _O_U16TEXT);
    int nbPlayer;
    int rows = 6;
    int columns = 10;

    do{
        printf("How Many players will play ? (2-4)\n");
        scanf("%d", &nbPlayer);
    } while (nbPlayer != 2 && nbPlayer != 3 && nbPlayer != 4);

    Map * map = initMap(columns,rows,nbPlayer);

    //char** tab = initialiseTab(6,10);

    afficherMap(map->tab);

    return 0;
}



