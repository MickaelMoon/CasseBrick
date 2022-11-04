#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

    char * buffer = malloc(sizeof(char)*1024);

    char tab[3][3] = {
        {'a','b','c'},
        {'d','e','f'},
        {'g','h','i'}
    };


    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char cToStr[2];
            cToStr[0] = tab[i][j];
            cToStr[1] ='\0';
            strcat(buffer,cToStr);
        }
        char cToStr[2];
        cToStr[0] = '\n';
        cToStr[1] ='\0';
        strcat(buffer,cToStr);
    }
    printf("%s",buffer);


    return 0;
}