#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#define ISLANDS 14

int **creatingLife(){
    int **life = malloc(ISLANDS *sizeof(int*));
    int island_types_sizes[] = {2,2,2,2,3,3,5,5,6,5,5,4,4,4};

    for(int i = 0; i < ISLANDS; i++){
        life[i] = malloc(island_types_sizes[i]*sizeof(int));
        for(int j = 0; j < island_types_sizes[i]; j++){
            life[i][j] = 0;
        }
    }

    return life;
}

int registringIsland(int** life, int island_type, int positions[], int number_pos){
    int point[] = {'.',0,1,2,3};
    int little[] = {'i',4,5};
    int big[] = {'I',6,7};
    int principal[] = {'p',8};
    int square[] = {'q',9,10};
    int triangle[] = {'t',11,12,13};

    int* islands_types[] = {point, little, big, principal, square, triangle};
    int number_islands[] = {4,2,2,1,2,3};

    for(int i = 0; i < 6; i++){ /*Percorrendo islands_types*/
        if(islands_types[i][0] == island_type){
            for(int j = 1; j < (number_islands[i] + 1); j++){ /*Percorrendo array da ilha*/
                if(life[islands_types[i][j]][0] == 0){
                    life[islands_types[i][j]][0] = number_pos;
                    for(int k = 1; k < (number_pos + 1); k++){  /*Percorrendo array da ilha do life*/
                        life[islands_types[i][j]][k] = positions[k-1];
                    }   
                    return 0;
                }
            }
        }
    }
    return 1;
}

void printLife(int **life) {
    int island_types_sizes[] = {2,2,2,2,3,3,5,5,6,5,5,4,4,4};

    for (int i = 0; i < ISLANDS; i++) {
        printf("Island %d: ", i);
        for (int j = 0; j < island_types_sizes[i]; j++) {
            printf("%d ", life[i][j]);
        }
        printf("\n");
    }
}

int verifyTotalLife(int **life){
    int sumlife = 0;

    for(int i=0; i < ISLANDS; i++){
        sumlife = sumlife + life[i][0]*1000000;
    }

    printf("Total de Habitantes restantes: %d\n", sumlife);

    if(sumlife == 0){
        return 1;
    }
    
    return 0;
}


int destroyIsland(int **life, int position){
    int island_types_sizes[] = {2,2,2,2,3,3,5,5,6,5,5,4,4,4};
    int island_types[] = {'.','.','.','.','i','i','I','I','p','q','q','t','t','t'};

    for (int i = 0; i < ISLANDS; i++) {
        for (int j = 0; j < island_types_sizes[i]; j++) {
            if(life[i][j] == position){
                life[i][0]--;
                if(life[i][0] == 0) return island_types[i];
                else return 0;
            }   
        }
    }

    printf("Erro ao atingir ilha\n");
    return -1;
}