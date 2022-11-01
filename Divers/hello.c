#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    
  srand(time(NULL));
    int randomfilePath = -1;
    int previousFilePath = -1;
    //boucle while Do you want to play next map ? Yes all player/ or quit/wait other player
    int i = 0;
    while (i < 20){
        do{
            randomfilePath = rand()%4;

        }while(randomfilePath == previousFilePath);
        printf("rand:%d, prev:%d\n",randomfilePath,previousFilePath);
        previousFilePath = randomfilePath;
        i++;
    }



    return 0;
}