#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int readingFile(char file_config_name[], char* map, int** life){
    FILE *file = fopen(file_config_name, "r");
    
    char line[2048];
    
    while(fgets(line, sizeof(line), file)){
        char island[2] = "!";
        char position[4] = "!!!";
        char direction[2] = "!";

        if(line[0] == 'Z'){
            island[0] = line[1];
            island[1] = '\0';
        
            if(line[6] == ':'){
                position[0] = line[3];
                position[1] = line[4];
                position[2] = line[5];
                position[3] = '\0';

                direction[0] = line[7];
                direction[1] = '\0';
            }

            else if(line[4] >= '0' && line[4] <= '9'){
                position[0] = line[3];
                position[1] = line[4];
                position[2] = '\0';

                direction[0] = line[6];
                direction[1] = '\0';
            }

            else return 1;
            
            if(building_islands(map, life, island, position, direction) == 1) return 1;
        }
    }
    
        return 0;
}


int building_islands(char* map, int** life, char island_type[], char position[], char direction[]){
    int initial_position = inputToPosition(position);
    
    if(strcmp(island_type, ".") == 0){
        if(makingIsland(map, life, initial_position, island_type, direction, 1, 0) == 1) {
            printf("Error: Ilha: Z%s, Posição: %s, Direção: %s\n", island_type, position, direction);
            return 1;   
        }
        else return 0;
    }
    else if(strcmp(island_type, "i") == 0){
        if(makingIsland(map, life, initial_position, island_type, direction, 2, 0) == 1) {
            printf("Error: Ilha: Z%s, Posição: %s, Direção: %s\n", island_type, position, direction);
            return 1;
        }
        else return 0;
    }
    else if(strcmp(island_type, "I") == 0){
        if(makingIsland(map, life, initial_position, island_type, direction, 4, 0) == 1) {
            printf("Error: Ilha: Z%s, Posição: %s, Direção: %s\n", island_type, position, direction);
            return 1;
        }
        else return 0;
    }
    else if(strcmp(island_type, "p") == 0){
        if(makingIsland(map, life, initial_position, island_type, direction, 5, 0) == 1) {
            printf("Error: Ilha: Z%s, Posição: %s, Direção: %s\n", island_type, position, direction);
            return 1;
        }
        else return 0;
    }
    else if(strcmp(island_type, "q") == 0){
        if(square(map, life, initial_position) == 1) {
            printf("Error: Ilha: Z%s, Posição: %s, Direção: %s\n", island_type, position, direction);
            return 1;
        }
        else return 0;
    }
    else if(strcmp(island_type, "t") == 0){
        if(triangle(map, life, initial_position, direction) == 1) {
            printf("Error: Ilha: Z%s, Posição: %s, Direção: %s\n", island_type, position, direction);
            return 1;
        }
        else return 0;
    }
    
    return 1;
}

int makingIsland(char* map, int** life, int initial_position, char island_type[], char direction[], int number, int donotregister){
    int positions[number];    
    
    if(strcmp(direction, "H") == 0){
        int line = positionToLine(initial_position);
        for(int i = 0; i < number; i++){
            if(acessingMap(map, initial_position + i) == 'M' && line == positionToLine(initial_position + i)){
                changingMap(map, initial_position + i, 'Y');
                positions[i] = initial_position + i;
            }
            else{
                return 1;
            }
        }
    }
    else if(strcmp(direction, "V") == 0){
        for(int i = 0; i < number; i++){
            if(acessingMap(map, initial_position + i*COLUMNTAM) == 'M'){
                changingMap(map, initial_position + i*COLUMNTAM, 'Y');
                positions[i] = initial_position + i*COLUMNTAM;
            }
            else{
                return 1;
            }
        }
    }

    else{
        return 1;
    }

    if(donotregister == 0){
        if(registringIsland(life, island_type[0], positions, number) == 1) {
            printf("Erro ao registrar ilha.\n");
            return 1;
        }   
    }

    return 0;
}

int square(char* map, int** life, int initial_position){
    int positions[4]; 
    for(int i = 0; i < 2; i++){
        initial_position = initial_position +  COLUMNTAM*i;
        positions[i*2] = initial_position; positions[(i*2)+1] = initial_position + 1;
        if(makingIsland(map, life, initial_position, "q", "H", 2, 1) == 1) return 1;  
    }
    if(registringIsland(life, 'q', positions, 4) == 1) {
        printf("Erro ao registrar ilha.\n");
        return 1;
    } 
    return 0;
}

int triangle(char* map, int** life, int initial_position, char direction[]){
    int positions[3];
    if(strcmp(direction, "H") == 0){
        for(int i = 0; i < 3; i++){
            int position = initial_position + i;
            if(i==1){
                position = position - i*COLUMNTAM;
                makingIsland(map, life, position, "t", "H", 1, 1);
            }
            else makingIsland(map, life, position, "t", "H", 1, 1);
            positions[i] = position;
        }
    }
    else if(strcmp(direction, "V") == 0){
        for(int i = 0; i < 3; i++){
            int position = initial_position + i*COLUMNTAM;
            if(i==1){
                position++;
                makingIsland(map, life, position, "t", "H", 1, 1);
            }
            else makingIsland(map, life, position, "t", "H", 1, 1);
            positions[i] = position;
        }
    }
    if(registringIsland(life, 't', positions, 3) == 1) {
        printf("Erro ao registrar ilha.\n");
        return 1;
    }   
    return 0;
}

int positionToColumn(int position){
    return position%26;
}

int positionToLine(int position){
    return position/26;
}

char *uppercase(char* string){
    for(int i = 0; string[i] != '\0'; i++){
        if(string[i] >= 'a' && string[i] <= 'z'){
            string[i] = (string[i] - 'a') + 'A';
        }
    }
    
    return string;
}