#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080
#define MAX_SIZE 256
#define MAX_REQUESTS 5
int comp_increasing(const void *a, const void *b)
{
    return *(char *)a - *(char *)b;
}
int comp_decreasing(const void *a, const void *b)
{
    return *(char *)b - *(char *)a;
}
int main()
{
    int sockfd, connfd, pid, rcvmsg, sndmsg;
    socklen_t len;
    char send_buff[MAX_SIZE];
    char recv_buff[MAX_SIZE];
    struct sockaddr_in servaddr, clientaddr;
    memset(send_buff, 0, sizeof(send_buff));
    memset(recv_buff, 0, sizeof(recv_buff));

    //socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if (listen(sockfd, MAX_REQUESTS) != 0)
    {
        perror("Listen failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Server listening\n");
    }
    len = sizeof(clientaddr);
    connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);

    //finally Chat
    pid = fork();
    while (strcmp(recv_buff, "exit") != 0)
    {
        if (pid == 0)
        {
            //handle receive message
            rcvmsg = recv(connfd, recv_buff, MAX_SIZE, 0);
            puts(recv_buff);
            if (strncmp(recv_buff, "exit", 4) == 0)
                break;
            memset(recv_buff, 0, sizeof(recv_buff));
        }
        else
        {
            //send message
            memset(send_buff, 0, sizeof(send_buff));
            printf("Enter string:\n");
            sndmsg = 0;
            fgets(send_buff, MAX_SIZE, stdin);
            send_buff[strcspn(send_buff, "\n")] = 0;
            if (strcmp(send_buff, "exit") == 0)
                break;
            sndmsg = send(connfd, &send_buff, MAX_SIZE, 0);
        }
    }
    close(sockfd);
    close(connfd);
    return 0;
}