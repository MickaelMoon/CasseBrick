#include <stdio.h>
#include <stdlib.h>


int collisionChecker(Player * player, Map * map, char direction){
    /* check if player next Tile is an empty Tile.
    0 = empty;
    1 = player or wall;
    2 = bomb;
    Will have to implement items here <-----
    */
    int x = player->x;
    int y = player->y;

    switch (direction){
        case 'n':
            if (x == 0 || map->tab[x-1][y] != ' '){
                if (map->tab[x-1][y] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
        case 's':
            if (x == map->rows -1 || map->tab[x+1][y] != ' '){
                if (map->tab[x+1][y] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
        case 'e':
            if (y == map->columns-1 || map->tab[x][y+1] != ' '){
                if (map->tab[x][y+1] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
        case 'o':
            if (y == 0 || map->tab[x][y-1] != ' '){
                if (map->tab[x][y-1] == 'B'){
                    return 2;
                } else {
                    return 1;
                }
            }
            break;
    }
    return 0;
}
