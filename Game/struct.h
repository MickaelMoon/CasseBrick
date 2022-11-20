#ifndef STRUCT_H
#define STRUCT_H

typedef enum {
    BLUE = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    PURPLE = 4,
    CYAN = 5,
} Color;

typedef enum {
    isDead = 0,
    isPlaying = 1,
    isBot = 2,
} PlayerStatus;

typedef enum {
    soloplayer = 0,
    multiplayer = 1,
} GameStatus;

typedef struct {
    int x;
    int y;
    int id;
    char token;
    PlayerStatus status;
    int bombMax;
    int currentNumberOfBombsLaunched;
    int firePower;
    int hasPlayed;
    int hasMoved;
    int passBombs;
    int bombKick;
    int invincibilityTime;
    int heart; //Max 1 per map
    int life; //No limit
    int socket;
    Color color;
} Player;

typedef struct {
    int id;
    Player * player;
    int turnRemainingBeforeExploding;
    int x;
    int y;
    int timer;
} Bomb;

typedef struct {
    char * pathFile;
    char ** dangerMap;
    int columns;
    int rows;
    char ** tab;
    int bombMin;
    int nbBombsOnMap;
    int nbPlayers;
    int nbPlayerAlive;
    int pause;
    Bomb ** bombList; // Array of bombs launched, to be modified with each exploding bombs
    Player ** playerList; // Array of players in the game, do not remove any player. Modifies the PlayerStatus if needed.
} Map;


typedef struct {
    GameStatus status;
    int nbMaxPlayer;
    int nbHumanPlayers;
    char ** filePathMapChoosen;
    int nbOfMapsAvailable;
    Map * currentMap;
} Game;

#endif