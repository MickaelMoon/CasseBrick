#include <stdio.h>
#include <stdlib.h>
#include "struct.h"


int collisionChecker(Player * player, Map * map, char direction){
    /* check if player next Tile is an empty Tile.
    0 = empty;
    1 = player or wall;
    2 = bomb;
    Will have to implement items here <-----
    */
    int j = player->x;
    int i = player->y;

    switch (direction){
        case 'z':
            if (i == 0 || map->tab[i-1][j] != ' '){
                if (map->tab[i-1][j] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
        case 's':
            if (i == map->rows -1 || map->tab[i+1][j] != ' '){
                if (map->tab[i+1][j] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
        case 'd':
            if (j == map->columns-1 || map->tab[i][j+1] != ' '){
                if (map->tab[i][j+1] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
        case 'q':
            if (j == 0 || map->tab[i][j-1] != ' '){
                if (map->tab[i][j-1] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
    }
    return 0;
}
