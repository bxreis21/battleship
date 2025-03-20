/*
** listener.c -- uma demonstração de servidor com datagrama
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include "main.h"

#define MAXBUFLEN 100
extern int myport;   // a porta na qual os usuários vão se conectar

char *listener()
{   
    int sockfd;
    struct sockaddr_in meu_endereco;    // a informação de meu endereço
    struct sockaddr_in seu_endereco; // Informação do endereço de quem se conecta
    socklen_t tam_endereco;
    int numbytes;
    char *buf;

    fflush(stdout);

    buf = malloc(MAXBUFLEN*sizeof(char));


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    meu_endereco.sin_family = AF_INET;         // host byte order
    meu_endereco.sin_port = htons(myport);     // short, network byte order
    meu_endereco.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(meu_endereco.sin_zero, '\0', sizeof meu_endereco.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&meu_endereco, sizeof meu_endereco) == -1) {
        perror("bind");
        close(sockfd);  // Adiciona close(sockfd) aqui para fechar o socket em caso de erro
        exit(1);
    }

    tam_endereco = sizeof seu_endereco;
    //while (1) {

    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
        (struct sockaddr *)&seu_endereco, &tam_endereco)) == -1) {
        perror("recvfrom");
        close(sockfd);  // Adiciona close(sockfd) aqui para fechar o socket em caso de erro
        exit(1);
    }

    buf[numbytes] = '\0';

    close(sockfd);

    return buf;
}