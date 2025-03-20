/*
** talker.c -- um demonstrativo de cliente com datagrama 
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
#include <netdb.h>
#include "main.h"

#define MAXLINHA 100

int lelinha(char s[], int lim);
extern int serverport;

char *talker(char ip[], char message[], float time)
{
    int sockfd;
    struct sockaddr_in seu_endereco; // informação do endereço do conector
    struct hostent *he;
    int numbytes;
    int tam;
    char s[MAXLINHA];
    float timer = 0.00;
    char *return_message = malloc(MAXLINHA*sizeof(char));

    if ((he = gethostbyname(ip)) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    seu_endereco.sin_family = AF_INET;     // ordem do byte do host
    seu_endereco.sin_port = htons(serverport); // short, network byte order
    seu_endereco.sin_addr = *((struct in_addr *)he->h_addr);
    memset(seu_endereco.sin_zero, '\0', sizeof seu_endereco.sin_zero);

    if(message[0] == '\0' && ((tam = lelinha(s, MAXLINHA)) > 0)){
            message = uppercase(s);
    }

    while (timer < time) {
        usleep(1000);

        timer = timer + 0.001;
        if ((numbytes = sendto(sockfd, message, strlen(message), 0,
                (struct sockaddr *)&seu_endereco, sizeof seu_endereco)) == -1) {
            perror("sendto");
            close(sockfd);
            exit(1);
        }
        //printf("Enviado %d bytes para %s\n", numbytes, inet_ntoa(seu_endereco.sin_addr));
    }

    close(sockfd);

    strcpy(return_message, message);

    return return_message;
}

int lelinha(char s[], int lim)
{
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}