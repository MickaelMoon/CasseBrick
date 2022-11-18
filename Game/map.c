#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include "struct.h"
#include "color.h"

#ifdef WIN32
#elif defined (linux)
#endif

void generateProceduralMap(int nbPlayers){
    srand(time(NULL));
    int rows = (rand()%(12))+3*nbPlayers;
    int columns = rand()%(20)+3*nbPlayers;
    char tab[rows][columns];
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < columns; x++){
            if (x == 0 || x == columns-1 || y == 0 || y == rows-1){
                tab[y][x] = 'x';
            } else {
                int choice = rand()%3;
                if (choice == 0){
                    tab[y][x] = ' ';
                } else {
                    tab[y][x] = 'm';
                }
            }
        }
    }
    switch(nbPlayers){
        case 4:
            tab[1][columns-2] = 'p';
            tab[2][columns-2] = ' ';
            tab[1][columns-3] = ' ';

            tab[rows-2][1] = 'p';
            tab[rows-3][1] = ' ';
            tab[rows-2][2] = ' ';

            tab[1][1] = 'p';
            tab[1][2] = ' ';
            tab[2][1] = ' ';

            tab[rows-2][columns-2] = 'p';
            tab[rows-3][columns-2] = ' ';
            tab[rows-2][columns-3] = ' ';

            break;
        case 2:
            tab[1][1] = 'p';
            tab[1][2] = ' ';
            tab[2][1] = ' ';

            tab[rows-2][columns-2] = 'p';
            tab[rows-3][columns-2] = ' ';
            tab[rows-2][columns-3] = ' ';
            break;
    }

    FILE *f;
    int bombInit;
    if (nbPlayers == 2){
        bombInit = 1;
        f = fopen("./Maps/map4.txt","w+");
    } else {
        bombInit = 2;
        f = fopen("./Maps/map9.txt","w+");
    }
    if (f == NULL){
        printf("Erreur ouverture fichier");
        exit(1);
    }
    fputc(bombInit+'0',f);
    fputc('\n',f);
    fprintf(f,"%d %d",columns,rows);
    fputc('\n',f);
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < columns; x++){
            fputc(tab[y][x],f);
        }
        fputc('\n',f);
    }

    fclose(f);
}

void afficherSerializedMap(char * map){
    setlocale(LC_ALL, "");
    #ifdef WIN32
    for (int i = 0; i < strlen(map); i++) {
        if (map[i] == 'B' || map[i] == 'F'){
                red();
            }else {
                char tile = map[i];
                Color color;
                if (tile != ' '){
                    switch(tile){
                        case '6':
                            red();
                            break;
                        case '3':
                            yellow();
                            break;
                        case '2':
                            green();
                            break;
                        case '1':
                            blue();
                            break;
                        case '4':
                            purple();
                            break;
                        case '5':
                            cyan();
                            break;
                        default:
                            break;
                    }
                }
            printf("%lc",map[i]);
            reset();
        }
    }
    #else
    for (int i = 0; i < strlen(map); i++) {
        if (map[i] == 'x'){
            printf("%lc",0x2593);
        }else if (map[i] == 'm'){
            printf("%lc",0x2591);
        }else if (map[i] == '+'){
            printf("%lc",0x2A39);
        }else if (map[i] == '-'){
            printf("%lc",0x2A3A);
        }else if (map[i] == 'O'){
            printf("%lc",0x2C90);
        }else if (map[i] == 'o'){
            printf("%lc",0x2C91);
        }else if (map[i] == 'P'){
            printf("%lc",0x2348);
        }else if (map[i] == 'K'){
            printf("%lc",0x27B2);
        }else if (map[i] == '*'){
            printf("%lc",0x2605);
        }else if (map[i] == '#'){
            printf("%lc",0x29BC);
        }else if (map[i] == '@'){
            printf("%lc",0x2665);
        }else if (map[i] == '&'){
            printf("%lc",0x290A);
        }else if (map[i] == 'B'){
            red();
            printf("%lc",0x25EF);
            reset();
        }else if (map[i] == 'F'){
                red();
                printf("%c",'F');
                reset();
            }else {
                char tile = map[i];
                Color color;
                if (tile != ' '){
                    switch(tile){
                        case '6':
                            red();
                            break;
                        case '3':
                            yellow();
                            break;
                        case '2':
                            green();
                            break;
                        case '1':
                            blue();
                            break;
                        case '4':
                            purple();
                            break;
                        case '5':
                            cyan();
                            break;
                        default:
                            break;
                    }
                }
            printf("%lc",map[i]);
            reset();
        }
    }
    #endif
}

void afficherMap(Map * map){
    setlocale(LC_ALL, "");
    // ajoute les bombes sur la map (gère le soucis où un joueur pose ou traverse une bombe, afin de n'afficher que le joueur)
    for (int i = 0; i < map->nbBombsOnMap; i++){
        int x = map->bombList[i]->x;
        int y = map->bombList[i]->y;
        if (map->tab[y][x] == ' ' && map->bombList[i]->timer > 0){
            map->tab[y][x] = 'B';
        }
    }
#ifdef WIN32
    for (int i = 0; i < map->rows; i++){
        for (int j = 0; j < map->columns; j++){
            if (map->tab[i][j] == 'B' || map->tab[i][j] == 'F'){
                red();
            }else {
                char tile = map->tab[i][j];
                Color color;
                if (tile != ' '){
                    for (int i = 0; i < map->nbPlayers; i++){
                        if (map->playerList[i]->token == tile){
                            color = map->playerList[i]->color;
                            break;
                        }
                    }
                    switch(color){
                        case RED:
                            red();
                            break;
                        case YELLOW:
                            yellow();
                            break;
                        case GREEN:
                            green();
                            break;
                        case BLUE:
                            blue();
                            break;
                        case PURPLE:
                            purple();
                            break;
                        case CYAN:
                            cyan();
                            break;
                    }
                }
            }
                printf("%c",map->tab[i][j]);
                reset();
        }
        printf("\n");
    }
#elif defined (linux)
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if (map->tab[i][j] == 'x'){
                printf("%lc",0x2593);
            }else if (map->tab[i][j] == 'm'){
                printf("%lc",0x2591);
            }else if (map->tab[i][j] == '+'){
                printf("%lc",0x2A39);
            }else if (map->tab[i][j] == '-'){
                printf("%lc",0x2A3A);
            }else if (map->tab[i][j] == 'O'){
                printf("%lc",0x2C90);
            }else if (map->tab[i][j] == 'o'){
                printf("%lc",0x2C91);
            }else if (map->tab[i][j] == 'P'){
                printf("%lc",0x2348);
            }else if (map->tab[i][j] == 'K'){
                printf("%lc",0x27B2);
            }else if (map->tab[i][j] == '*'){
                printf("%lc",0x2605);
            }else if (map->tab[i][j] == '#'){
                printf("%lc",0x29BC);
            }else if (map->tab[i][j] == '@'){
                printf("%lc",0x2665);
            }else if (map->tab[i][j] == '&'){
                printf("%lc",0x290A);
            }else if (map->tab[i][j] == 'B'){
                red();
                printf("%lc",0x25EF);
                reset();
            }else if (map->tab[i][j] == 'F'){
                red();
                printf("%c",'F');
                reset();
            }else {
                char tile = map->tab[i][j];
                Color color;
                if (tile != ' '){
                    for (int i = 0; i < map->nbPlayers; i++){
                        if (map->playerList[i]->token == tile){
                            color = map->playerList[i]->color;
                            break;
                        }
                    }
                    switch(color){
                        case RED:
                            red();
                            break;
                        case YELLOW:
                            yellow();
                            break;
                        case GREEN:
                            green();
                            break;
                        case BLUE:
                            blue();
                            break;
                        case PURPLE:
                            purple();
                            break;
                        case CYAN:
                            cyan();
                            break;
                    }
                }
            printf("%lc",map->tab[i][j]);
            reset();
            }
        }
        printf("\n");
    }
#endif

    /*printf("DangerMap\n");
        for (int i = 0; i < map->rows; ++i) {
            for (int j = 0; j < map->columns; j++){
                printf("%d",map->dangerMap[i][j]);
            }
            printf("\n");
        }
    */
}

char * serializeMap(Map * map){
    char * buffer = malloc(sizeof(char)*1024);
    // ajoute les bombes sur la map (gère le soucis où un joueur pose ou traverse une bombe, afin de n'afficher que le joueur)
    for (int i = 0; i < map->nbBombsOnMap; i++){
        int x = map->bombList[i]->x;
        int y = map->bombList[i]->y;
        if (map->tab[y][x] == ' ' && map->bombList[i]->timer > 0){
            map->tab[y][x] = 'B';
        }
    }
    int index = 0;
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            buffer[index] = map->tab[i][j];
            index++;
        }
        buffer[index] = '\n';
        index++;
    }
    buffer[index] = '\0';
    return buffer;
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
    Color colorPanel[] = {BLUE,GREEN,YELLOW,PURPLE,CYAN,RED};
    Player * player = malloc(sizeof (Player));
    player->x = 0; // a updater dans la création de map
    player->y = 0; // a updater dans la création de map
    player->id = i+1;
    player->token = player->id+'0';
    player->status = isPlaying;
    player->color = colorPanel[i%6];
    player->currentNumberOfBombsLaunched = 0;
    player->bombKick = 0;
    player->firePower = 2;
    player->passBombs = 0;
    player->invincibilityTime = 0;
    player->heart = 0; //Max 1 per map
    player->life = 0; //No limit
    player->socket = -1;

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

Map * initMap(char * filePath, int mode, int nbHumanPlayers){
    Map * map = malloc(sizeof(Map));
    map->columns;
    map->rows;
    map->pathFile = filePath;
    map->pause = 0;
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
                playerList[index]->bombMax = map->bombMin;
                map->tab[i][j] = playerList[index]->token;
                index++;
            }
        }
    }

    if (mode == soloplayer){
        for (int i = nbHumanPlayers; i < map->nbPlayers; i++){ // modifier i pour ajuster le nombre de Bots (i = nombres de joueurs humain)
            playerList[i]->status = isBot;
            printf("Player %c is a bot (%d)\n",playerList[i]->token,playerList[i]->status);
        }
    }

    map->playerList = playerList;
    char ** dangerMap = calloc(map->rows, sizeof(char *));

    for (int i = 0; i < map->rows; ++i) {
        dangerMap[i] = calloc(map->columns, sizeof(char));
    }

    map->dangerMap = dangerMap;

    return map;
}

void updateDangerMap(Map *map){
    for (int i = 0; i < map->rows; i++){
        for (int j = 0; j < map->columns; j++){
            map->dangerMap[i][j] = 0;
        }
    }
    for (int z = 0; z < map->nbBombsOnMap; z++){
        Bomb * bomb = map->bombList[z];
        int X = bomb->x;
        int Y = bomb ->y;
        map->dangerMap[Y][X] = 1;
        // explosion to the right
        for (int i = X, k = 0; k <= bomb->player->firePower; i++, k++){
            if (i == map->columns){
                i = 0;
            }
            if (map->tab[Y][i] == 'x' || map->tab[Y][i] == 'm') {
                break;
            } else {
                map->dangerMap[Y][i] = 1;
            }
        }
        // explosion to the left
        for (int i = X, k = 0; k <= bomb->player->firePower; i--, k++){
            if (i < 0){
                i = map->columns-1;
            }
            if (map->tab[Y][i] == 'x' || map->tab[Y][i] == 'm') {
                break;
            } else {
                map->dangerMap[Y][i] = 1;
            }
        }
        // explosion to the top
        for (int i = Y, k = 0; k <= bomb->player->firePower; i--, k++){
            if (i < 0){
                i = map->rows-1;
            }
            if (map->tab[i][X] == 'x' || map->tab[i][X] == 'm') {
                break;
            } else {
                map->dangerMap[i][X] = 1;
            }
        }
        // explosion to the bottom
        for (int i = Y, k = 0; k <= bomb->player->firePower; i++, k++){
            if (i == map->rows){
                i = 0;
            }
            if (map->tab[i][X] == 'x' || map->tab[i][X] == 'm') {
                break;
            } else {
                map->dangerMap[i][X] = 1;
            }
        }
    }
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
    /*printf("updated bombList:\n");
    for(int i = 0; i < map->nbBombsOnMap; i++ ){
        printf("Bombe %d, joueur %c, timer %d\n", map->bombList[i]->id, map->bombList[i]->player->token, map->bombList[i]->timer);
    }*/

}
