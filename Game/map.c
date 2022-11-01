#include <stdio.h>
#include <stdlib.h>
#include "struct.h"


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

//deprecated, replaced by recupData
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
            else if(j%2 == 0 && i%2 == 0) {
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
    player->status = isPlaying;
    /*if (i == 0){
        player->status = isPlaying;
    } else {
        player->status = waiting;
    }*/
    player->bombMax = 1;
    player->currentNumberOfBombsLaunched = 0;
    player->bombKick = 0;
    player->firePower = 2;
    player->passBombs = 0;
    player->invincibilityTime = 0;
    player->heart = 0; //Max 1 per map
    player->life = 0; //No limit

    return player;
}

int nbPlayerMaxPerMap(char * filepath){
    FILE *f = fopen(filepath, "r");
    int nbPlayers = 0;
        char buffer[100] = {0};
    fgets(buffer, 29, f); //remove 1st line

    fgets(buffer, 29, f);
    int index = 0;
    int i = 0;
    char ro[3];
    char co[3];

    while (buffer[index] != ' ' && buffer[index] != '\0'){
        co[i] = buffer[index];
        i++;
        index++;
    }
    i = 0;
    index++;
    while (buffer[index] != ' ' && buffer[index] != '\0'){
        ro[i] = buffer[index];
        i++;
        index++;
    }
    char rows = atoi(ro);
    char columns = atoi(co);

    char c = fgetc(f);
    int k = 0;
    int j = 0;
    while (c != EOF){
        if (c == 'p'){ //recup the number of player max per map
            nbPlayers++;
        }
        c = fgetc(f);
    }
    return nbPlayers;

}

void recupData(char *filepath, Map * map){
    printf("%s\n",filepath);
    FILE *f = fopen(filepath, "r");
    int nbPlayers = 0;

    char buffer[100] = {0};

    if (f == NULL){
        printf("Error at opening file config.txt");
    }

    map->bombMin = atoi(fgets(buffer, 29,f));

    fgets(buffer, 29, f);
    int index = 0;
    int i = 0;
    char ro[3];
    char co[3];

    while (buffer[index] != ' ' && buffer[index] != '\0'){
        co[i] = buffer[index];
        i++;
        index++;
    }
    i = 0;
    index++;
    while (buffer[index] != ' ' && buffer[index] != '\0'){
        ro[i] = buffer[index];
        i++;
        index++;
    }
    map->rows = atoi(ro);
    map->columns = atoi(co);
    // Initialize the Array with new loaded rows & columns
    char ** tab = malloc(sizeof(char *) * map->rows);

    for (int i = 0; i < map->rows; ++i) {
        tab[i] = malloc(sizeof(char)* map->columns);
    }

    char c = fgetc(f);
    int k = 0;
    int j = 0;
    while (c != EOF){
        if (j == map->columns){
            j = j % map->columns;
            k++;
        }
        if (c != '\n'){
            tab[k][j] = c;
            if (c == 'p'){ //recup the number of player max per map
                nbPlayers++;
            }
            j++;
        }
        c = fgetc(f);
    }
    map->nbPlayers = nbPlayers;
    map->tab = tab;
}

Map * initMap(char * filePath){
    Map * map = malloc(sizeof(Map));
    map->columns;
    map->rows;
    map->pathFile = filePath;
    recupData(filePath, map);
    map->nbBombsOnMap = 0;
    map->nbPlayerAlive = map->nbPlayers;
    Bomb ** bombList = malloc(sizeof(Bomb*)*40); //Max 40 bombs at the same time on map
    for (int i = 0; i < 40; i++){
        bombList[i] = malloc(sizeof(Bomb));
    }
    map->bombList = bombList;
    
    //create the players at each place a 'p' is written in the map->tab, and replace the 'p' by the player token
    Player ** playerList = malloc(sizeof(Player*)*map->nbPlayers);
    int index = 0;
    for (int i = 0; i < map->rows; i++){
        for (int j = 0; j < map->columns; j++){
            if (map->tab[i][j] == 'p'){
                playerList[index] = malloc(sizeof(Player));
                playerList[index] = initPlayer(index);
                playerList[index]->x = j;
                playerList[index]->y = i;
                map->tab[i][j] = playerList[index]->token;
                index++;
            }
        }
    }

    map->playerList = playerList;

    return map;
}

void updateTimerBomb(Map * map, Player * player){
    for (int i = 0; i < map->nbBombsOnMap; i++){
        if (map->bombList[i]->player->token == player->token){ //timer updated only for the bombs dropped by the current player
            int bombIsInactive = 0;
            for (int j = 0; j < map->nbPlayers; j++){
                // check playerList to see if a player is on the current bomb checked, if yes bomb timer is disable temporary
                if (map->playerList[j]->x == map->bombList[i]->x && map->playerList[j]->y == map->bombList[i]->y){
                    bombIsInactive = 1;
                }
            }
            if (bombIsInactive == 0){
                map->bombList[i]->timer--;
            }
        };
        if (map->bombList[i]->timer == 0){
            explosion(map->bombList[i], map);
        }
    }

}
