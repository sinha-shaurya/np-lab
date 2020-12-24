#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8000
#define SIZE 1024
#define MAX_REQ 5
int main()
{
    int sockfd, csockfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    char buffer[SIZE];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1)
    {
        perror("Could not bind");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, MAX_REQ) != 0)
    {
        printf("Cannot listen");
    }
    else
    {
        printf("Server online");
    }
    memset(buffer, 0, sizeof(buffer));
    for (;;)
    {
        len = sizeof(clientaddr);
        csockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);
        if (csockfd != -1)
        {
            printf("Connected to client\n");
        }
        //start loop
        while(1)
        {
            memset(buffer,0,sizeof(buffer));
            recv(csockfd,buffer,SIZE,0);
            puts(buffer);
            send(csockfd,buffer,SIZE,0);
            if(strncmp(buffer,"exit",4)==0)
                break;
        }
        close(csockfd);
    }
    close(sockfd);
    return 0;
}