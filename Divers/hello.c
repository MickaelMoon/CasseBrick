#include <stdio.h>

int main() {
    

    FILE * f = fopen("./test.txt","r");
    char tab[5][5];

    char c = fgetc(f);
    int i = 0;
    int j = 0;
    while (c != EOF){
         if (j == 5){
            j = j % 5;
            i++;
        }
        if (c != '\n'){
            tab[i][j] = c;
            j++;
        }
        c = fgetc(f);
    }

            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 5; j++){
                    printf("%c",tab[i][j]);
                }
                printf("\n");
        }

    return 0;
}