#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int serverport;   // a porta que os usuários vão se conectar
int myport;   // a porta na qual os usuários vão se conectar

void definingPorts(char *mporta, char *sporta){

    if(isdigit(mporta[0]) && isdigit(sporta[0])){
        for(int d = 0; isdigit(mporta[d]); d++){
            myport = myport*10 + (mporta[d] - '0');

        }
        for(int d = 0; isdigit(sporta[d]); d++){
            serverport = serverport*10 + (sporta[d] - '0');
        }
    }
    
    printf("MyPort: %d\n",myport);
    printf("ServerPort: %d\n",serverport);
}

int startingGame(int argc, char ip[]){

    char *message;
    int int_return = -1;
    float local_sleep = 1;

    printf("Conectando com o host: %s \n", ip);

    talker(ip, "START", 0.01);

    printf("Aguardando Resposta...\n");
    message = listener();

    if(strcmp(message, "START") == 0){
        talker(ip, "STARTED", 0.01);
        printf("Conexão foi um sucesso. Sua vez de jogar!\n\n");

        sleep(local_sleep);

        int_return = 1;
    }

    else if(strcmp(message, "STARTED") == 0){

        sleep(local_sleep);

        printf("Conexão foi um sucesso. É a vez do seu oponente!\n\n");
        int_return = 0;
    }

    return int_return;
}