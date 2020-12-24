#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8000
#define SIZE 1024
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t len;
    char buffer[SIZE];
    memset(buffer, 0, sizeof(buffer));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1)
    {
        perror("Cannot connect");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Enter data:");
        fgets(buffer, SIZE, stdin);
        send(sockfd,buffer,SIZE,0);
        if(strncmp(buffer,"exit",4)==0)
        {
            close(sockfd);
            printf("Disconnected");
            exit(EXIT_SUCCESS);
        }
        memset(buffer,0,sizeof(buffer));
        recv(sockfd,buffer,SIZE,0);
        puts(buffer);
    }
    return 0;
}