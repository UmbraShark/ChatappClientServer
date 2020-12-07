/*
 *By hamza LAHLAL Dec 2020
 *argv[0] filename
 *argv[1] portNo
 */
#include <stdio.h>
#include <sys/types.h>     //definitions of a number of data used in system calls
#include <sys/socket.h>   //definitions for structure needed for sockets
#include <stdlib.h>
#include <netinet/in.h>   //constants and structures needed for internet domain addresses
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error (const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        puts("[*]Port No not provided, Program terminated.\n");
        exit(1);
    }
    int sockfd, newsockfd, portNo,n;
    char  buff[1024] ;
    struct sockaddr_in serv_addr,client_addr;
    socklen_t client_len;
    puts("[*]Creation socket...");
    //creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("[*]ERROR socket...");
    }
    //clearing server address
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portNo = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNo);
    //inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if(bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR bind...");
    }
    puts("[*]listening...");
    //listening to connections (Max 5)
    listen(sockfd,5);
    client_len = sizeof(client_addr);
    puts("[*]Attempting to Connections...");
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr,&client_len);
    printf("[*]Connection by client %s\n ",inet_ntoa(client_addr.sin_addr));
    if(newsockfd < 0){
        error("[*]ERROR accept...");
    }
    while (1){
        bzero(buff ,1024);
        //receinving message
        n = read(newsockfd, buff,1024);
        if(n < 0){
            error("[*]Error read...");
        }
        printf("Client : %s\n",buff);
        printf("=>");
        //clearing buffer
        bzero(buff,1024);
        //reading bytes from stream
        fgets(buff ,1024,stdin);
        n = write(newsockfd, buff, strlen(buff));
        if(n < 0){
            error("[*]Error  write...");
        }


        if (strncmp("bye", buff, 3) == 0){
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}