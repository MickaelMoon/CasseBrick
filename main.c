#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <fcntl.h>
#include <locale.h>

typedef enum {
    //waiting = 0,
    isDead = 0,
    isPlaying = 1,
} PlayerStatus;

typedef struct {
    int x;
    int y;
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
    free(bomb);
    map->nbBombsOnMap--;
};

Bomb * initBomb(Player * player){
    Bomb * bomb = malloc(sizeof(Bomb));
    bomb->x = player->x;
    bomb->y = player->y;
    bomb->player = *player;
    bomb->player.currentNumberOfBombsLaunched++;

    return bomb;
};

void poseBomb(Map * map, Player * player){
    map->bombList[map->nbBombsOnMap] = initBomb(player);
}

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
        } else if (map->tab[Y][i] == '1' || map->tab[Y][i] == '2' || map->tab[Y][i] == '3' || map->tab[Y][i] == '4'){
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

    return tab;
}

void afficherMap(Map * map){
    // ajoute les bombes sur la map (gère le soucis où un joueur pose ou traverse une bombe, afin de n'afficher que le joueur)
    for (int i = 0; i < map->nbBombsOnMap; i++){
        int x = map->bombList[i]->x;
        int y = map->bombList[i]->y;
        if (map->tab[y][x] == ' '){
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
            map->tab[rows-2][1] = playerList[2]->token;
        } else if (i == 3){
            playerList[3]->x = 1;
            playerList[3]->y = rows -2;
            map->tab[1][columns -2] = playerList[3]->token;
        }
    }
    map->playerList = playerList;

    return map;
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
    do{
        system("clear");
        afficherMap(map);
        printf("%d\n",play);
        printf("Do something, Player %c:\n",map->playerList[play]->token);
        scanf("%c",&action);
        while (getchar()!='\n');
        printf("action: %c\n",action);
        play++;
    } while (play < 4);

    return 0;
}



