#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

char *creatingMap(void){

    char *map;

    map = malloc(MAPTAM*sizeof(char));

    for(int i=0; i < MAPTAM; i++){
        if(i==(MAPTAM-1)){
            map[i] = 'M';
            map[i+1] = '\0';
        }
        else {
            map[i] = 'M';
        }
    }

    return map;
}

void printMaps(char *maps[]){
    int lmymap = 0;
    int lenemymap = 0;

    printf("\n\t(Seu Planeta - Planeta Inimigo)\n");
    for(int linha = 1; linha <= 26; linha++){
        printf("\t");
        for(; lmymap < linha*26; lmymap++){
            if(maps[0][lmymap] != '\0'){
                putchar(maps[0][lmymap]);
                printf(".");
            }
        }
        printf("\t-\t");
        for(; lenemymap < linha*26; lenemymap++){
            putchar(maps[1][lenemymap]);
            printf(".");
        }
        printf("\n");
    }
    printf("\n");
}


int acessingMap(char *map, int position){

    if(position >= 0){
        return map[position];
    }

    return -1;
}

void changingMap(char *map, int position, int caract){
    if(position >= 0){
        map[position] = caract;
    }
    else{
        printf("Posição Inválida.");
    }
}

int inputToPosition(char input[]){

    int position = -1;
    int column = -1;
    int line = -1;

    for(int c = 0 ; (input[c] != '\0') && ((column == -1) || (line == -1)); c++) {
        if(input[c] >= 'A' && input[c] <= 'Z'){
            if(line == -1){
                line = input[c] - 'A';
            }
        }
        if((input[c] >= '0') && (input[c] <= '9')){
            column = 0;
            for(int d = 0; (input[c + d] >= '0') && (input[c + d] <= '9'); d++){
                int digit;
                digit = input[c + d] - '0';                 
                column = column * 10 + digit;
            }
            break;
        }
    }

    if(column >= (COLUMNTAM)){
        column = -1;
    }

    if(column >= 0 && line >= 0){
        position = ((line * COLUMNTAM-1) + column) + 1;
    }
    
    return position;
}