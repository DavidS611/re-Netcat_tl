#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 4096

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    if (argc < 4) {
        fprintf(stderr, "Usage: %s hostname port message\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    bzero(buffer,BUFFER_SIZE);
    strcpy(buffer, argv[3]);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket");
    }
    bzero(buffer,BUFFER_SIZE);
    n = read(sockfd,buffer,BUFFER_SIZE);
    if (n < 0) {
        error("ERROR reading from socket");
    }
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}

