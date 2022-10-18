#ifndef STRUCT_H
#define STRUCT_H

typedef enum {
    waiting = 0,
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
    int hasPlayed;
    int hasMoved;
    int passBombs;
    int bombKick;
    int invincibilityTime;
    int heart; //Max 1 per map
    int life; //No limit
} Player;

typedef struct {
    int id;
    Player player;
    int turnRemainingBeforeExploding;
    int x;
    int y;
    int timer;
} Bomb;

typedef struct {
    int columns;
    int rows;
    char ** tab;
    int bombMin;
    int nbBombsOnMap;
    int nbPlayers;
    int nbPlayerAlive;
    Bomb ** bombList; // Array of bombs launched, to be modified with each exploding bombs
    Player ** playerList; // Array of players in the game, do not remove any player. Modifies the PlayerStatus if needed.
} Map;

#endif