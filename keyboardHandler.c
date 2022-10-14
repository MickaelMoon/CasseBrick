#include <stdio.h>
#include <stdlib.h>


void keyHandler(char c, Map * map, Player * player) {
    // Player want to move on map (n-NORD / s-SUD / e-EST / o-OUEST)
    if (c == 'n' || c == 's' || c == 'e' || c == 'o') {
            move(c, map, player);
        } else if ()
    } else if (c == 'w'){ // Player want to pass (w-WAIT)
        wait(c, map, player);
    } else if (c == 'b'){// Player want to pose a bomb
        poseBomb(map, player)
    } else {
        printf("Invalid Key pressed\n choose between the following actions:\n Move: n (NORTH), s (SOUTH), e (EAST), o (WEST);\n Action: w (WAIT), b (BOMB);\n");
    }
}

void move (char c, Map * map, Player * player){
    int x = player->x;
    int y = player->y;

    switch(c){
        case 'n':
            map[x][y] = ' ';
            map[x-1][y] = player->token;
            player->x = x-1;
            break;
        case 's':
            map[x][y] = ' ';
            map[x+1][y] = player->token;
            player->x = x+1;
            break;
        case 'e':
            map[x][y] = ' ';
            map[x][y+1] = player->token;
            player->y = y+1;
            break;
        case 'o':
            map[x][y] = ' ';
            map[x][y-1] = player->token;
            player->y = y-1;
            break;
    }
}