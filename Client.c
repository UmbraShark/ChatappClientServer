/*
 *By hamza LAHLAL Dec 2020
 *argv[0] filename
 *argv[1] server ip_address
 *argv[2] PortNo
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

void error (const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    int sockfd, portNo, n;
    char  buff[1024] ;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    //checking if portNo by client is provided
    if(argc < 3){
        fprintf(stderr,"[*]usage %s hostname port\n",argv[0]);
        exit(1);
    }
    portNo = atoi(argv[2]);
    puts("[*]Creation socket...");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("[*]ERROR socket...");
    }
    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr,"[*]ERROR , no such host");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *) server -> h_addr , (char *) &serv_addr.sin_addr.s_addr, server->h_length);//copying from start server to serv_addr
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNo);
    puts("[*]Connection to the server...");
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("[*]ERROR connect...");
    }
    puts("[*]Connection succeed...");
    while (1){
        bzero(buff ,1024);
        fgets(buff ,1024,stdin);
        n = write(sockfd, buff, strlen(buff));
        if(n < 0){
            error("[*]Error write...");
        }
        bzero(buff,1024);
        n = read(sockfd, buff,1024);
        if(n < 0){
            error("[*]Error read...");
        }
        printf("Server : %s\n",buff);
        printf("=>");

        if (strncmp("bye", buff, 3) == 0){
            break;
        }
    }
    close(sockfd);
    return 0;
}