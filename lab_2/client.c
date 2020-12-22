#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8000
#define MAX_SIZE 1024
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int n = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    //send data to server
    char buffer[MAX_SIZE];
    memset(buffer, 0, sizeof(buffer));
    printf("Enter string\n");
    scanf("%s", buffer);
    send(sockfd, buffer, MAX_SIZE, 0);
    int pid = fork();
    if (pid == 0)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(sockfd, buffer, MAX_SIZE, 0);
        puts(buffer);
    }
    else
    {
        char buff[MAX_SIZE];
        memset(buff, 0, sizeof(buff));
        recv(sockfd, buff, MAX_SIZE, 0);
        puts(buff);
    }
    close(sockfd);
    return 0;
}