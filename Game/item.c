#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

void itemGenerator(Map * map, int x, int y){
    int r = rand()%100;
    char item;
    if (r < 50){
        item = ' '; //Nothing dropped
    } else if (r < 55){
        item = '+'; //Fire Up
    } else if (r < 60){
        item = '-'; //Fire down
    } else if (r < 65){
        item = 'O'; //Bomb Up
    } else if (r < 70){
        item = 'o'; //Bomb Down
    } else if (r < 75){
        item = 'P'; //PasseBomb
    } else if (r < 80){
        item = 'K'; //KickBomb
    } else if (r < 85){
        item = '*'; //Invincibility
    } else if (r < 90){
        item = '#'; //Heart
    } else if (r < 95){
        item = '@'; //Life
    } else if (r < 98){
        item = '&'; //Fire MAX
    } else {
        item = ' ';
    }
    map->tab[y][x] = item;
}

void pickItem(char item, Player * player){
    switch(item){
        case '+': //Fire Up
            if (player->firePower < 9){
                player->firePower++;
            }
            break;
        case '-': //Fire down
            if (player->firePower > 1){
                player->firePower--;
            }
            break;
        case 'O': //Bomb Up
            if (player->bombMax < 9){
                player->bombMax++;
            }
            break;
        case 'o': //Bomb Down
            if (player->bombMax > 1){
                player->bombMax--;
            }
            break;
        case 'P': //PasseBomb
            player->passBombs = 1;
            player->bombKick = 0;
            break;
        case 'K': //KickBomb
            player->passBombs = 0;
            player->bombKick = 1;
            break;  
        case '*': //Invincibility
            player->invincibilityTime = 3;
            break;
        case '#': //Heart
            if (player->heart == 0){ //No heart already picked up
                player->heart = 1;
            }
            break;
        case '@': //Life
            if (player->life < 9){
                player->life++;
            }
            break;
        case '&': //Fire MAX
            player->firePower = 9;
            break;
    }
}
