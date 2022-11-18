#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "item.h"

int tileChecker(int x, int y, Map * map, Player * player){
    /* return value of the Tile.
    0 = empty or item pickable;
    1 = player or wall;
    2 = bomb;
    Will have to implement items here <-----
    */
    switch (map->tab[y][x]){
        case 'x':
        case 'm':
        case '1':
        case '2':
        case '3':
        case '4':
            return 1;
            break;
        case 'B':
            return 2;
            break;
        case ' ':
            return 0;
        default:
            pickItem(map->tab[y][x], player);
            return 0;
    }
}

int collisionChecker(Player * player, Map * map, char direction){
    /* check if player next Tile is an empty Tile.
    0 = empty ;
    1 = player or wall;
    2 = bomb;
    Will have to implement items here <-----
    */
    int x = player->x;
    int y = player->y;
    int tileCheckerValue = -1;
    switch (direction){
        case 'z':
            if (y == 0){
                tileCheckerValue = tileChecker(x, map->rows-1, map, player);
                }
            else {
                tileCheckerValue = tileChecker(x, y-1, map, player);
            }
            break;
        case 's':
            if (y == map->rows -1){
                tileCheckerValue = tileChecker(x, 0, map, player);
            } else {
                tileCheckerValue = tileChecker(x, y+1, map, player);
            }
            break;
        case 'd':
            if (x == map->columns-1){
                tileCheckerValue = tileChecker(0, y, map, player);
            } else {
                tileCheckerValue = tileChecker(x+1, y, map, player);
            }
            break;
        case 'q':
            if (x == 0){
                tileCheckerValue = tileChecker(map->columns-1, y, map, player);
            } else {
                tileCheckerValue = tileChecker(x-1, y, map, player);
            }
            break;
    }
    return tileCheckerValue;
}
