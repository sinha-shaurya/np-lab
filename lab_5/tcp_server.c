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
#define MAX_REQ 4
int main()
{
    int sockfd, csockfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len;
    pid_t childpid;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1)
    {
        perror("Cannot bind");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, MAX_REQ) != 0)
    {
        printf("Cannot listen");
    }
    else
    {
        printf("Server online\n");
    }

    char buffer[SIZE];
    memset(buffer, 0, sizeof(buffer));

    //infinite loops for multiple connections
    while (1)
    {
        len = sizeof(clientaddr);
        csockfd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
        printf("Connection accepted from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        //child process will close connection
        if ((childpid = fork()) != 0)
        {
            close(sockfd);

            while (1)
            {
                recv(csockfd, buffer, SIZE, 0);
                if (strncmp(buffer, "exit", 4) == 0)
                {
                    break;
                }
                else
                {
                    printf("Client:\n");
                    puts(buffer);
                    send(csockfd, buffer, SIZE, 0);
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }
        }
    close(csockfd);
    return 0;
}