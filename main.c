#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[]){

    int start;
    char* maps[2] = {creatingMap(), creatingMap()};
    int** life = creatingLife();
    int result;
    char** inputs = verify_argv(argc, argv);
    (strcmp(inputs[0], "-a") == 0) ? argc--: argc;

    if (argc < 3 || checkingInitial(argc, inputs)){
        printf("A Inicialização Falhou!\n");
        printf("Uso: ./main hostname file_config_name myport(default:4950) serverport(default:4950)\n");
        printf("Inclua '-a' na entrada para ativar o modo automático de jogo.\n");
        return 0;
    }
    else if(readingFile(inputs[2],maps[0], life) == 1){
        printf("Falha no posicionamento de ilhas, revise o arquivo de entrada.\n");
        return 0;
    }
    else{
        printf("Arquivo de entrada válido!\n");
        start = startingGame(argc, inputs[1]);
        result = (strcmp(inputs[0], "-a") != 0) ? mainGame(start, maps, life, inputs[1]) : autoGame(start, maps, life, inputs[1]);

        if(result == 100){
            printf("Parabéns, você ganhou o jogo!\n");
            verifyTotalLife(life);
            printMaps(maps);
        }
        else if(result == 200){
            printMaps(maps);
            printf("Você perdeu o jogo!\n");
        }
    }
    
    return 0;
}

int checkingInitial(int argc,char *argv[]){
    FILE *file = fopen(argv[2], "r");
    char default_myport[] = "4950";  
    char default_serverport[] = "4950";  

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    if(!isdigit(argv[1][0])){
        printf("isdigit %d\n", isdigit(argv[1][0]));
        return 1;
    }

    if(argc == 5 && isdigit(argv[3][0]) && isdigit(argv[4][0]) ){
        definingPorts(argv[3], argv[4]);
    }
    else{
        definingPorts(default_myport, default_serverport);
    }

    return 0;
}

char** verify_argv(int argc, char *argv[]){
    char** inputs = malloc(argc * sizeof(char*));
    inputs[0] = "";
    int j = 1;

    for(int i = 1; i < argc; i++){
        if(argv[i][0] == '-' && argv[i][1] == 'a'){
            inputs[0] = argv[i];
        }
        else{
            inputs[j++] = argv[i];
        }
    }

    return inputs;
}