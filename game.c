#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int mainGame(int start, char *maps[], int** life, char ip[]){
    
    int turn = start;
    int* enemyLife = malloc(sizeof(int));
    *enemyLife = 38*1000000; 

    while(turn==0 || turn==1){
        printf("\n---------------------------------------------------------------------------------------------------------------\n\n");
        if(turn==1){
            turn = myturn(ip, maps, life, enemyLife);
        }
        else if(turn==0){
            turn = histurn(ip, maps, life, enemyLife);
        }
    }

    return turn;
    printf("Jogo finalizado.");
}

int myturn(char ip[],char *maps[], int** life, int* enemyLife){
    char *response;
    char *message;
    int position;
    float time = 0.005;
    
    verifyTotalLife(life);
    printf("Total de Habitantes Inimigos restantes: %d\n", *enemyLife);
    printf("Mapas:\n");
    printMaps(maps);

    printf("Escreva a posição que deseja atacar: ");
    message = talker(ip, "", time);
    position = inputToPosition(message);
    printf("Ataque realizado! Aguardando retorno...");

    response = listener();

    return ver_myturn(ip, maps, life, response, position, enemyLife);
}

int ver_myturn(char ip[], char *maps[], int** life, char* response, int position, int* enemyLife){
    if((strcmp(response, "Y") == 0) || response[0] == 'Z'){
        changingMap(maps[1], position,'Y');
        *enemyLife = *enemyLife - 1000000;
        if(response[0] == 'Z') destroyed(response[1]);
        if(*enemyLife == 0) return 100;
        if(response[0] != 'Z') printf("acertou! Jogue novamente!\n");
        return 1;
    }
    else if(strcmp(response, "X") == 0){
        changingMap(maps[1], position, 'X');
        printf("errou!\n");
        return 0;
    }
    else if(strcmp(response, "R") == 0){
        printf("já atacou essa posição!\n");
        return 1;
    }

    else if(strcmp(response, "E") == 0){
        printf("Posição Inválida! Formato: LetraNumero\n");
        return 1;
    }

    return -1;
}

int histurn(char ip[], char *maps[], int** life, int* enemyLife){
    int searching;
    int position;
    char *response;

    verifyTotalLife(life);
    printf("Total de Habitantes Inimigos restantes: %d\n", *enemyLife);
    printf("Esperando ataque...");

    response = listener();

    position = inputToPosition(response);
    searching = acessingMap(maps[0], position);
 
    return ver_histurn(ip, maps, life, response, searching, position);
}

int ver_histurn(char ip[], char *maps[], int** life, char* response, int searching, int position){
    usleep(5000);
    float time = 0.001;

    if(position < 0){
        talker(ip, "E", time);
        return 0;
    }
    else if(searching == 'M'){
        printf("Seu Oponente acertou o mar, posição: %s \n", response);
        changingMap(maps[0], position,'X');
        talker(ip, "X", time);
        return 1;
    }
    else if(searching == 'Y'){
        int result_destroy = destroyIsland(life, position);
        char answer_destroy[2] = "Z!";

        printf("Seu Oponente acertou uma ilha, posição: %s \n", response);
        changingMap(maps[0], position,'F');

        if(result_destroy == -1) return -1;
        else if(result_destroy == 0) talker(ip, "Y", time);
        else { 
            answer_destroy[1] = result_destroy; 
            talker(ip, answer_destroy, time); 
            printf("Ilha Destruída! %s\n", answer_destroy);
            }

        if(verifyTotalLife(life) == 1) return 200;
        printMaps(maps);
        return 0;
    }
    else if(searching == 'F' || searching == 'X'){
        printf("Seu Oponente mirou em uma posição repetida \n");
        talker(ip, "R", time);
        return 0;
    }

    return -1;
}

void destroyed(int caracter){
    char island[2];
    island[0] = caracter;
    island[1] = '\0';

    char *point[] = {".", "Ponto"};
    char *little[] = {"i", "iPequena"};
    char *big[] = {"I", "iMaior"};
    char *principal[] = {"p", "Principal"};
    char *square[] = {"q", "Quadrada"};
    char *triangle[] = {"t", "Triangular"};

    char** islands_types[] = {point, little, big, principal, square, triangle};

    for(int i = 0; i < 6; i++){ /*Percorrendo islands_types*/
        if(strcmp(islands_types[i][0], island)==0){
            printf("Ilha Destruída!\n");
            printf("Tipo da Ilha: %s\n", islands_types[i][1]);
            break;
        }
    }
}