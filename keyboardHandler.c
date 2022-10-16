#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "collisionChecker.h"


Bomb * initBomb(Player * player){
    Bomb * bomb = malloc(sizeof(Bomb));
    bomb->x = player->x;
    bomb->y = player->y;
    bomb->player = *player;
    bomb->player.currentNumberOfBombsLaunched++;
    bomb->timer = 4;

    return bomb;
};

void poseBomb(Map * map, Player * player){
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
        map->nbBombsOnMap++;
    } else {
        printf("a bomb is already on this tile\n");
    }
}

void move (char c, Map * map, Player * player){
    int j = player->x;
    int i = player->y;
    //printf("player: %c, x: %d, y: %d\n",player->token, i, j);

    switch(c){
        case 'z':
            map->tab[i][j] = ' ';
            map->tab[i-1][j] = player->token;
            player->y = i-1;
            //printf("Nord\n");
            break;
        case 's':
            map->tab[i][j] = ' ';
            map->tab[i+1][j] = player->token;
            player->y = i+1;
            //printf("Sud\n");
            break;
        case 'd':
            map->tab[i][j] = ' ';
            map->tab[i][j+1] = player->token;
            player->x = j+1;
            //printf("Est\n");
            break;
        case 'q':
            map->tab[i][j] = ' ';
            map->tab[i][j-1] = player->token;
            player->x = j-1;
            //printf("Ouest\n");
            break;
    }
}

void kickBomb (char c, Map * map, Player * player){
    int decallage = 1;
    int i = player->y;
    int j = player->x;
    Bomb * bomb;
    
    switch(c){
        case 'z':
            i=i-1; //bomb is 1 tile up to player
            while (map->tab[i-decallage][j] == ' '){
                decallage++;
            }
            break;
        case 's':
            i=i+1; //bomb is 1 tile down to player
            while (map->tab[i+decallage][j] == ' '){
                decallage++;
            }
            break;
        case 'd':
            j=j+1; //bomb is 1 tile right to player
            while (map->tab[i][j+decallage] == ' '){
                decallage++;
            }
            break;
        case 'q':
            j=j-1; //bomb is 1 tile left to player
            while (map->tab[i][j-decallage] == ' '){
                decallage++;
            }
            break;
    }

    for (int k = 0; k < map->nbBombsOnMap; k++){
        if (map->bombList[k]->x == j && map->bombList[k]->y == i){
            bomb = map->bombList[k];
        }
    }

    switch(c){
        case 'z':
            map->tab[i][j] = ' '; // initial bomb position cleared
            map->tab[i-(decallage-1)][j] = 'B'; //bomb new position (-1 because decallage initialized at 1)
            //bomb->y = i-(decallage-1); //bomb item updated
            break;
        case 's':
            map->tab[i][j] = ' '; // initial bomb position cleared
            map->tab[i+(decallage-1)][j] = 'B'; //bomb new position (-1 because decallage initialized at 1)
            bomb->y = i+(decallage-1); //bomb item updated
            break;
        case 'd':
            map->tab[i][j] = ' '; // initial bomb position cleared
            map->tab[i][j+(decallage-1)] = 'B'; //bomb new position (-1 because decallage initialized at 1)
            bomb->x = j+(decallage-1); //bomb item updated
            break;
        case 'q':
            map->tab[i][j] = ' '; // initial bomb position cleared
            map->tab[i][j-(decallage-1)] = 'B'; //bomb new position (-1 because decallage initialized at 1)
            bomb->x = j-(decallage-1); //bomb item updated
            break;
    }
}

void keyHandler(char c, Map * map, Player * player) {
    // Player want to move on map (n-NORD / s-SUD / e-EST / o-OUEST)
    if (c == 'z' || c == 's' || c == 'd' || c == 'q') {
        int collision = collisionChecker(player, map, c);
        if (collision == 0){ //next Tile is empty
            move(c, map, player);
        } else if (collision == 1){ //next Tile is a player or a wall
            printf("Move impossible, choose another action\n");
        } else if (collision == 2){ //next Tile is a bomb
            if (player->passBombs == 1){ //Player can walk on bombs
                move(c,map,player);
            } else if (player->bombKick == 1){ //Player can push bombs
                kickBomb(c,map,player);
            }
        }
    } else if (c == 'w'){ // Player want to pass (w-WAIT)
        //wait(c, map, player);
    } else if (c == 'b'){// Player want to pose a bomb
        poseBomb(map, player);
    } else {
        printf("Invalid Key pressed\n choose between the following actions:\n Move: n (NORTH), s (SOUTH), e (EAST), o (WEST);\n Action: w (WAIT), b (BOMB);\n");
    }
}