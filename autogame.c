#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int autoGame(int start, char *maps[], int** life, char ip[]){
    int turn = start;
    int* enemyLife = malloc(sizeof(int));
    int* historyPlays[COLUMNTAM*LINETAM*2];  /* Historico de Posições Jogadas*/
    int* historyAttackPositions[COLUMNTAM*LINETAM*2]; /* Historico de Acertos, Erros ou Destruição */
    
    for(int i = 0; i < COLUMNTAM*LINETAM*2; i++) {
        historyPlays[i] = malloc(sizeof(int));
        historyAttackPositions[i] = malloc(sizeof(int));
        *historyPlays[i] = -1;
        *historyAttackPositions[i] = -1;
    }

    *historyPlays[0] = 7; /* 7 marca o início do array */
    *historyAttackPositions[0] = 7; /* 7 marca o início do array */
    *enemyLife = 38*1000000; 

    while(turn==0 || turn==1){
        printf("\n---------------------------------------------------------------------------------------------------------------\n\n");
        if(turn==1){
            turn = botturn(ip, maps, life, enemyLife, historyPlays, historyAttackPositions);
        }
        else if(turn==0){
            turn = histurn(ip, maps, life, enemyLife);
        }
    }

    return turn;
    printf("Jogo finalizado.");
}

int botturn(char ip[],char *maps[], int** life, int* enemyLife, int* historyPlays[], int* historyAttackPositions[]){
    char *response;
    char *message;
    int position;
    char *decision;
    float time = 0.005;
    
    verifyTotalLife(life);
    printf("Total de Habitantes Inimigos restantes: %d\n", *enemyLife);
    printf("Mapas:\n");
    printMaps(maps);

    decision = clockAttack(historyAttackPositions, historyPlays);
    message = talker(ip, decision, time);
    printf("Escreva a posição que deseja atacar: ");
    printf("%s\n", message);
    position = inputToPosition(message);
    printf("Ataque realizado! Aguardando retorno...");

    response = listener();

    if((strcmp(response, "Y") == 0) || response[0] == 'Z'){
        verifyTotalLife(life);
        printf("Total de Habitantes Inimigos restantes: %d\n", *enemyLife);
        printf("Mapas:\n");
        printMaps(maps);
        changingMap(maps[1], position,'Y');
        *enemyLife = *enemyLife - 1000000;
        if(response[0] == 'Z') {
            destroyed(response[1]);
            registerAttack(2,historyAttackPositions);
        }
        else {
            registerAttack(1,historyAttackPositions);
        }
        registerPosition(position, historyPlays);
        if(*enemyLife == 0) return 100;
        if(response[0] != 'Z') printf("acertou! Jogue novamente!\n");
        return 1;
    }
    else if(strcmp(response, "X") == 0){
        changingMap(maps[1], position, 'X');
        registerAttack(0,historyAttackPositions);
        registerPosition(position, historyPlays);
        printf("errou!\n");
        return 0;
    }
    else if(strcmp(response, "R") == 0){
        printf("já atacou essa posição!\n");
        registerAttack(9,historyAttackPositions); /*9 é o numero que indica que foi uma posição repetida ou invalida*/
        return 1;
    }

    else if(strcmp(response, "E") == 0){
        printf("Posição Inválida! Formato: LetraNumero\n");
        registerAttack(9, historyAttackPositions);
        return 1;
    }

    return -1;
}

int verifyPositions(int position, int* historyPlays[]){
    int i = 0;
    while(*historyPlays[i] != -1){
        if(*historyPlays[i] == position){
            return 1;
        }
        i++;
    }

    return 0;
}

void registerPosition(int position, int* historyPlays[]){
    for(int i = 0; i < LINETAM*COLUMNTAM; i++){
        if(*historyPlays[i] == -1){
            *historyPlays[i] = position;
            break;
        }
    }
}

void registerAttack(int result, int* historyAttackPositions[]){
    for(int i = 0; i < LINETAM*COLUMNTAM; i++){
        if(*historyAttackPositions[i] == -1){
            *historyAttackPositions[i] = result;
            break;
        }
    }
}

int findLastAttack(int *historyAttackPositions[]){
    for(int i = 0; i < COLUMNTAM*LINETAM; i++){
        if(*historyAttackPositions[i] == -1){
            return --i;
        }
    }
    return -1;
}

int direction_clock = 0;
int lastplay = 0;
int steps = 0;

char *clockAttack(int* historyAttackPositions[], int* historyPlays[]){
    if(*historyAttackPositions[findLastAttack(historyAttackPositions)] == 1){
        if(steps > 1) direction_clock = (direction_clock + steps) % 8;
        steps = 0;
    }
    else{
        countStepsClock(historyAttackPositions);
        if(steps == 0){
            while(verifyPositions(lastplay, historyPlays) != 0){
                lastplay = (lastplay + 1) % 676;
            }
            return positionToInput(lastplay);
            } 
    }

    return positionToInput(nextClockAttackPosition(historyPlays));
}

void countStepsClock(int* historyAttackPositions[]){
    int count = 1;
    int lastAttack = findLastAttack(historyAttackPositions);

    for(int i = 1; i < 8; i++){
        if(lastAttack == 0){
            steps = 0;
            break;
        }
        else if(*historyAttackPositions[lastAttack - count] == 1){
            steps = count;
            break;
        }
        else if(*historyAttackPositions[lastAttack - count] == 2
        ||
        *historyAttackPositions[lastAttack - count] == 7
        ||
        *historyAttackPositions[lastAttack - count] == 9){
            steps = 0;
            break;
        }
        
        else {
            count++;
            }
    } 
}

int nextClockAttackPosition(int* historyPlays[]){
    int position = (direction_clock + steps) % 7;

    printf("NEXTCLOCKATTACK\n");
    printf("input: %s\n", positionToInput(*historyPlays[lastPositionAttack(historyPlays)]));
    printf("position: %d\n", position);
    printf("steps %d\n",steps);

    if(steps < 8){
        if(position == 0 || position == 6){
            int nextposition = *historyPlays[lastPositionAttack(historyPlays)] + 1;

            if(inputToPosition(positionToInput(nextposition)) == -1 || verifyPositions(nextposition, historyPlays) != 0){
                steps++;
                return nextClockAttackPosition(historyPlays);
            } 
            return nextposition;
        }
        else if(position == 1){
            int nextposition = *historyPlays[lastPositionAttack(historyPlays)] + COLUMNTAM;

            if(inputToPosition(positionToInput(nextposition)) == -1 || verifyPositions(nextposition, historyPlays) != 0){
                steps++;
                return nextClockAttackPosition(historyPlays);
            } 
            return nextposition;
        }
        else if(position == 2 || position == 3){
            int nextposition = *historyPlays[lastPositionAttack(historyPlays)] - 1;

            if(inputToPosition(positionToInput(nextposition)) == -1 || verifyPositions(nextposition, historyPlays) != 0){
                steps++;
                return nextClockAttackPosition(historyPlays);
            } 
            return nextposition;
        }
        else if(position == 4 || position == 5){
            int nextposition = *historyPlays[lastPositionAttack(historyPlays)] - COLUMNTAM;

            if(inputToPosition(positionToInput(nextposition)) == -1 || verifyPositions(nextposition, historyPlays) != 0){
                steps++;
                return nextClockAttackPosition(historyPlays);
            } 
            return nextposition;
        }
    }

    steps = 0;
    return -1;
}

int lastPositionAttack(int* historyPlays[]){
    for(int i = 0; i < LINETAM*COLUMNTAM; i++){
        if(*historyPlays[i] == -1 && i !=0){
            return --i;
        }
    }

    return -1;
}

char *positionToInput(int position){
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* input = malloc(4*sizeof(char));
    input[3] = '\0';

    input[0] = letters[positionToLine(position)];

    if(position >= 0 && position <= 9){
        input[1] = '0' + position;
        input[2] = '\0';
    }
    else{
        input[1] = '0' + positionToColumn(position)/10;
        input[2] = '0' + positionToColumn(position)%10;
    }

    return input;
}