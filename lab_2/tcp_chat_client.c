#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main()
{
    int sockfd, connfd, pid, recvmsg, sendmsg;
    char buffRecv[MAX] = "";
    char buffSend[MAX] = "";
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // Chat part
    pid = fork();

    while (strncmp("exit", buffRecv, 4) != 0)
    {

        if (pid == 0)
        { // Child process for receive the message

            recvmsg = recv(sockfd, buffRecv, MAX, 0);
            printf("From Server: %s\n", buffRecv);
            if (strncmp("exit", buffRecv, 4) == 0)
            {
                printf("Client Exit...\n");
                break;
            }
            bzero(buffRecv, MAX);
            recvmsg = 0;
        }
        else
        {
            bzero(buffSend, sizeof(buffSend));
            printf("Enter the string : ");
            sendmsg = 0;
            while ((buffSend[sendmsg++] = getchar()) != '\n')
                ;
            sendmsg = send(sockfd, buffSend, MAX, 0);

            // if msg contains "Exit" then server exit and chat ended.
            if (strncmp("exit", buffSend, 4) == 0)
            {
                printf("Client Exit...\n");
                break;
            }
        }
    }

    // close the socket
    close(sockfd);
    exit(0);
}
